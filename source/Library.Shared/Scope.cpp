#include "pch.h"
#include "Scope.h"
#include <assert.h>

namespace GameEngine
{
	RTTI_DEFINITIONS(Scope);

	Scope::Scope(size_t capacity) :
		mTable(capacity),
		mDatumPointers(capacity)
	{}

	Scope::Scope(const Scope& other)
	{
		DeepCopy(other);
	}

	Scope::Scope(Scope&& other)
	{
		MoveFrom(std::move(other));
	}

	Scope& Scope::operator=(const Scope& other)
	{
		if (this != &other)
		{
			_Clear();
			DeepCopy(other);
		}
		return *this;
	}

	Scope& Scope::operator=(Scope&& other)
	{
		if (this != &other)
		{
			_Clear();
			MoveFrom(std::move(other));
		}
		return *this;
	}

	Datum& Scope::operator[](const std::string& key)
	{
		return Append(key).first;
	}

	const Datum& Scope::operator[](const std::string& key) const
	{
		const Datum* datum = Find(key);
		if (datum != nullptr)
		{
			return *datum;
		}
		else
		{
			throw std::exception("Can't find entry with given key");
		}
	}

	Datum& Scope::operator[](size_t index)
	{
		if (index < mDatumPointers.Size())
		{
			return mDatumPointers[index]->second;
		}
		throw std::exception("Index out of range");
	}

	const Datum& Scope::operator[](size_t index) const
	{
		return const_cast<Scope*>(this)->operator[](index);
	}

	bool Scope::operator==(const Scope& other) const
	{
		if (this == &other)
		{
			return true;
		}
		else if (mTable.Size() == other.mTable.Size())
		{
			// Walk through each datum in this table and find a equal one in the other
			bool equal = true;
			for (const auto& pair : mTable)
			{
				auto it = other.mTable.Find(pair.first);
				if (it == other.mTable.end() || pair.second != it->second)
				{
					equal = false;
					break;
				}
			}
			return equal;
		}
		else
		{
			return false;
		}
	}

	bool Scope::operator!=(const Scope& other) const
	{
		return !operator==(other);
	}

	Scope::~Scope()
	{
		// Delete allocated memory
		_Clear();

		// Notify parent
		if (mParent != nullptr)
		{
			mParent->Orphan(*this);
		}
	}

	std::string Scope::ToString() const
	{
		std::string str;
		str.append("{\n");
		for (size_t i = 0; i < mDatumPointers.Size(); ++i)
		{
			auto pair = mDatumPointers[i];
			str.append("\"");
			str.append(pair->first);
			str.append("\": [");
			for (size_t j = 0; j < pair->second.Size(); ++j)
			{
				if (pair->second.Type() == Datum::DatumType::Pointer && pair->second == this)
				{
					str.append("this");
				}
				else
				{
					str.append(pair->second.ToString(j));
				}
				if (j < pair->second.Size() - 1)
				{
					str.append(", ");
				}
			}
			str.append("]");
			if (i < mDatumPointers.Size() - 1)
			{
				str.append(",");
			}
			str.append("\n");
		}
		str.append("}");
		return str;
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		Scope* other = rhs != nullptr ? rhs->As<Scope>() : nullptr;
		return other != nullptr && this->operator==(*other);
	}

	gsl::owner<Scope*> Scope::Clone() const
	{
		return new Scope(*this);
	}

	Datum* Scope::Find(const std::string& key)
	{
		auto it = mTable.Find(key);
		return it == mTable.end() ? nullptr : &it->second;
	}

	const Datum* Scope::Find(const std::string& key) const
	{
		return const_cast<Scope*>(this)->Find(key);
	}

	std::pair<Datum*, Scope*> Scope::Search(const std::string& path)
	{
		// Default search behavior for path with only one entry
		if (path.find("/") == std::string::npos)
		{
			TableIteratorType it = mTable.Find(path);
			if (it != mTable.end())
			{
				return std::make_pair(&it->second, this);
			}
			else
			{
				return mParent != nullptr ? mParent->Search(path) : std::make_pair(nullptr, nullptr);
			}
		}

		// Absolute path starts from the root
		Datum* resultDatum = nullptr;
		Scope* currentScope = this;
		if (path._Starts_with("/"))
		{
			currentScope = GetRoot();
		}

		// Recursively get to the target datum
		size_t start = 0;
		std::string token;
		for (size_t i = 0; i < path.size(); ++i)
		{
			if (path[i] == '/')
			{
				if (start != i)  // At least one char between two slashes
				{
					token = path.substr(start, i - start);
					if (!ParseSearchToken(token, resultDatum, currentScope))
					{
						return { nullptr, nullptr };
					}
				}
				start = i + 1;
			}
		}

		// Last element can't be handled in the loop
		if (start != path.size())
		{
			token = path.substr(start, path.size() - start);
			if (!ParseSearchToken(token, resultDatum, currentScope))
			{
				return { nullptr, nullptr };
			}
		}

		return { resultDatum, currentScope };
	}

	std::pair<const Datum*, const Scope*> Scope::Search(const std::string& path) const
	{
		return const_cast<Scope*>(this)->Search(path);
	}

	Scope* Scope::GetRoot()
	{
		Scope* current = this;
		while (current->GetParent() != nullptr)
		{
			current = current->GetParent();
		}
		return current;
	}

	const Scope* Scope::GetRoot() const
	{
		return const_cast<Scope*>(this)->GetRoot();
	}

	std::pair<Datum&, bool> Scope::Append(const std::string& key)
	{
		if (key.empty())
		{
			throw std::exception("No empty key is allowed");
		}

		auto [it, inserted] = mTable.Insert(std::make_pair(key, Datum()));
		if (inserted)
		{
			mDatumPointers.PushBack(&*it);
		}
		return std::pair<Datum&, bool>(it->second, inserted);
	}
	
	Scope& Scope::AppendScope(const std::string& key)
	{
		// Insert new datum or get existing one
		auto [datum, inserted] = Append(key);
		if (!inserted && datum.Type() != Datum::DatumType::Unknown && datum.Type() != Datum::DatumType::Table)
		{
			throw std::exception("Try to append scope to a existing datum with different type");
		}

		// Append new scope to datum
		Scope* child = new Scope();
		child->mParent = this;
		datum.PushBack(*child);
		return *child;
	}

	void Scope::Adopt(Scope& child, const std::string& key)
	{
		// Cycle check
		if (this == &child || IsDescendentOf(child))
		{
			throw std::exception("Can't adopt self or ancestor as child, will form a cycle");
		}

		// Check if key is valid and create one if there is no entry with key.
		// If there exists one, no matter what its type is, no datum will be created
		// If there doesn't exist one, a datum will be created, and it will pass the check, so no side effect
		auto [datum, inserted] = Append(key);
		if (!inserted && datum.Type() != Datum::DatumType::Unknown && datum.Type() != Datum::DatumType::Table)
		{
			throw std::exception("Given key has an entry with different type");
		}

		// Notify child's old parent, if has one
		if (child.mParent != nullptr)
		{
			child.mParent->Orphan(child);
		}

		// Finally can adopt
		child.mParent = this;
		datum.PushBack(child);
	}

	Scope* Scope::GetParent() const
	{
		return mParent;
	}

	size_t Scope::Size() const
	{
		return mTable.Size();
	}

	std::pair<std::string, Datum*> Scope::FindScope(const Scope* scope)
	{
		std::string key;
		Datum* result = nullptr;
		if (scope != nullptr)
		{
			ForEachChildScope([&key, &result](const std::string& constKey, Datum& datum, size_t, Scope&)
			{
				key = constKey;
				result = &datum;
			}, scope, true);
		}
		
		return std::make_pair(key, result);
	}

	std::pair<std::string, const Datum*> Scope::FindScope(const Scope* scope) const
	{
		return const_cast<Scope*>(this)->FindScope(scope);
	}

	std::string Scope::FindKey(const Datum& datum) const
	{
		for (const auto& pair : mTable)
		{
			if (&pair.second == &datum)
			{
				return pair.first;
			}
		}

		return std::string();
	}

	bool Scope::IsDescendentOf(Scope& other)
	{
		Scope* current = mParent;
		while (current != nullptr)
		{
			if (current == &other)
			{
				return true;
			}
			current = current->mParent;
		}

		return false;
	}

	void Scope::Orphan(Scope& child)
	{
		ForEachChildScope([](const std::string&, Datum& datum, size_t index, Scope&)
		{
			datum.RemoveAt(index);
		}, &child, true);
		child.mParent = nullptr;
	}

	void Scope::Clear()
	{
		_Clear();
	}

	void Scope::DeepCopy(const Scope& other)
	{
		mTable = other.mTable;
		mDatumPointers = other.mDatumPointers;

		// Reset pointers
		for (size_t i = 0; i < mDatumPointers.Size(); ++i)
		{
			const std::string& key = mDatumPointers[i]->first;
			auto& pair = *mTable.Find(key);
			mDatumPointers[i] = &pair;
		}

		// Duplicate children
		ForEachChildScope([this](const std::string&, Datum& datum, size_t index, Scope& scope)
		{
			Scope* dupChild = scope.Clone();
			dupChild->mParent = this;
			datum.Set(*dupChild, index);
		});
	}

	void Scope::MoveFrom(Scope&& other)
	{
		mTable = std::move(other.mTable);
		mDatumPointers = std::move(other.mDatumPointers);

		// Fix other Scope so it's still valid and usable if it still can be referenced by parent
		// Kinda weird situation, should be an error already if someone tries to move a non-transient scope to another and still uses the moved one
		if (other.mParent != nullptr)
		{
			other.mTable.Resize(mTable.Size());
		}

		// Adopt children
		ForEachChildScope([this](const std::string&, Datum&, size_t, Scope& scope)
		{
			scope.mParent = this;
		});
	}

	void Scope::ForEachChildScope(const std::function<void(const std::string&, Datum&, size_t, Scope&)>& action, const Scope* address, bool oneShot)
	{
		for (auto& pair : mTable)
		{
			if (pair.second.Type() == Datum::DatumType::Table)
			{
				Datum& datum = pair.second;
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					Scope& scope = datum.AsTable(i);
					if (address == nullptr || address == &scope)
					{
						action(pair.first, datum, i, scope);
						if (oneShot)
						{
							return;
						}
					}
				}
			}
		}
	}

	void Scope::_Clear()
	{
		// Delete all children scopes
		ForEachChildScope([this](const std::string&, Datum&, size_t, Scope& child)
		{
			assert(child.mParent == this);
			child.mParent = nullptr;
			delete &child;
		});

		// Clear table, don't free memory now
		mDatumPointers.Clear();
		mTable.Clear();
	}

	bool Scope::ParseSearchToken(std::string& token, Datum*& outDatum, Scope*& outScope)
	{
		if (token == "..")
		{
			outScope = outScope->GetParent();
			if (outScope == nullptr)
			{
				return false;
			}
		}
		else if (token != ".")
		{
			// Break token up in case it contains []
			size_t bracketStart = 0;
			size_t bracketEnd = 0;
			std::string key;
			std::string bracket;
			if ((bracketStart = token.find("[")) != std::string::npos && (bracketEnd = token.find("]")) != std::string::npos)
			{
				key = token.substr(0, bracketStart);
				bracket = token.substr(bracketStart + 1, bracketEnd - bracketStart - 1);
			}
			else
			{
				key = move(token);
			}

			// Try to find the datum with given key
			outDatum = outScope->Find(key);
			if (outDatum == nullptr)
			{
				return false;
			}

			// Change current scope
			if (outDatum->Type() == Datum::DatumType::Table && !bracket.empty())
			{
				try
				{
					size_t index = std::stoi(bracket);
					if (outDatum->Size() > index)
					{
						outScope = &outDatum->AsTable(index);
					}
					else
					{
						return false;
					}
				}
				catch (std::invalid_argument e)
				{
					bool found = false;
					for (size_t index = 0; index < outDatum->Size(); ++index)
					{
						Scope& scope = outDatum->AsTable(index);
						if (scope.Find("Name") != nullptr && scope["Name"].AsString() == bracket)
						{
							outScope = &outDatum->AsTable(index);
							found = true;
							break;
						}
					}
					if (!found)
					{
						return false;
					}
				}
			}
		}

		return true;
	}
}