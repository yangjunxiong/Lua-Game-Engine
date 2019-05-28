#include "pch.h"
#include "Attributed.h"
#include "Stack.h"

using namespace std;

namespace GameEngine
{
	RTTI_DEFINITIONS(Attributed);

	Attributed::Attributed(IdType type)
	{
		Populate(type);
	}

	Attributed::Attributed(const Attributed& other) :
		Scope(other)
	{
		RedirectStorage(other.TypeIdInstance());
	}

	Attributed::Attributed(Attributed&& other) :
		Scope(move(other))
	{
		RedirectStorage(other.TypeIdInstance());
	}

	Attributed& Attributed::operator=(const Attributed& other)
	{
		if (this != &other)
		{
			Scope::operator=(other);  // Scope::operator= can't use polymorphic Clear
			RedirectStorage(other.TypeIdInstance());
		}
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& other)
	{
		if (this != &other)
		{
			Scope::operator=(move(other));    // Scope::operator= can't use polymorphic Clear
			RedirectStorage(other.TypeIdInstance());
		}
		return *this;
	}

	bool Attributed::operator==(const Attributed& other) const
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
				if (it->first == "this"s)  // Don't compare this, otherwise will have infinite recursion
				{
					continue;
				}
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

	bool Attributed::operator!=(const Attributed& other) const
	{
		return !operator==(other);
	}

	bool Attributed::IsAttribute(const std::string& name) const
	{
		return Find(name) != nullptr;
	}

	bool Attributed::IsPrescribedAttribute(const std::string& name) const
	{
		bool isPrescribed = name == "this"s;
		if (!isPrescribed)
		{
			const auto signatureList = AttributedTypeManager::GetSignature(TypeIdInstance());
			if (signatureList != nullptr)
			{
				for (const auto& signature : *signatureList)
				{
					if (signature.mName == name)
					{
						assert(IsAttribute(name));
						isPrescribed = true;
						break;
					}
				}
			}
		}

		return isPrescribed;
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string& name) const
	{
		return IsAttribute(name) && !IsPrescribedAttribute(name);
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& name)
	{
		if (IsPrescribedAttribute(name))
		{
			throw std::exception("Try to append auxiliary attribute to a prescribed field");
		}
		return Append(name).first;
	}

	Vector<Attributed::PairType*> Attributed::Attributes()
	{
		return mDatumPointers;
	}

	Vector<const Attributed::PairType*> Attributed::Attributes() const
	{
		return PromoteToConst(mDatumPointers);
	}

	Vector<Attributed::PairType*> Attributed::PrescribedAttributes()
	{
		auto signatures = AttributedTypeManager::GetSignature(TypeIdInstance());
		size_t endIndex = signatures == nullptr ? 1 : signatures->Size() + 1;
		Vector<Attributed::PairType*> result(endIndex);
		for (size_t i = 0; i < endIndex; ++i)
		{
			result.PushBack(mDatumPointers[i]);
		}
		return result;
	}

	Vector<const Attributed::PairType*> Attributed::PrescribedAttributes() const
	{
		return PromoteToConst(const_cast<Attributed*>(this)->PrescribedAttributes());
	}

	Vector<Attributed::PairType*> Attributed::AuxiliaryAttributes()
	{
		auto signatures = AttributedTypeManager::GetSignature(TypeIdInstance());
		size_t startIndex = signatures == nullptr ? 1 : signatures->Size() + 1;
		Vector<Attributed::PairType*> result(mDatumPointers.Size() - startIndex);
		for (size_t i = startIndex; i < mDatumPointers.Size(); ++i)
		{
			result.PushBack(mDatumPointers[i]);
		}
		return result;
	}

	Vector<const Attributed::PairType*> Attributed::AuxiliaryAttributes() const
	{
		return PromoteToConst(const_cast<Attributed*>(this)->AuxiliaryAttributes());
	}

	void Attributed::RemoveAuxiliaryAttribute(const std::string& key)
	{
		if (IsAuxiliaryAttribute(key))
		{
			auto it = mTable.Find(key);
			mDatumPointers.Remove(mDatumPointers.Find(&*it));
			mTable.Remove(it);
		}
	}

	void Attributed::Clear()
	{
		Scope::Clear();
		Populate(TypeIdInstance());
	}

	gsl::owner<Scope*> Attributed::Clone() const
	{
		return new Attributed(*this);
	}

	void Attributed::Populate(IdType type)
	{
		// Append "this"
		(*this)["this"] = this;

		// Populate other prescribed attributes
		const auto signatureList = AttributedTypeManager::GetSignature(type);
		if (signatureList != nullptr)
		{
			for (const auto& signature : *signatureList)
			{
				assert(Find(signature.mName) == nullptr);                   // Should never have duplicated names
				assert(signature.mType != Datum::DatumType::Unknown);       // User should provide valid type
				Datum& datum = Append(signature.mName).first;
				datum.SetType(signature.mType);
				if (signature.mOffset > 0)                                  // External storage
				{
					assert(signature.mType != Datum::DatumType::Table);     // External storage shouldn't have nested scope
					datum.SetStorage(reinterpret_cast<uint8_t*>(this) + signature.mOffset, signature.mSize);
				}
				else                                                        // Internal storage
				{
					if (signature.mType == Datum::DatumType::Table)
					{
						for (size_t i = 0; i < signature.mSize; ++i)
						{
							AppendScope(signature.mName);
						}
					}
					else
					{
						datum.SetSize(signature.mSize);
					}
				}
			}
		}
	}

	void Attributed::RedirectStorage(IdType type)
	{
		(*this)["this"] = this;
		const auto signatureList = AttributedTypeManager::GetSignature(type);
		if (signatureList != nullptr)
		{
			for (const auto& signature : *signatureList)
			{
				if (signature.mOffset > 0)  // Only redirect external storage
				{
					Find(signature.mName)->SetStorage(reinterpret_cast<uint8_t*>(this) + signature.mOffset, signature.mSize);
				}
			}
		}
	}

	Vector<const Attributed::PairType*> Attributed::PromoteToConst(const Vector<PairType*>& vector) const
	{
		Vector<const PairType*> result(vector.Size());
		for (const auto& pointer : vector)
		{
			result.PushBack(pointer);
		}
		return result;
	}

	Attributed::Signature::Signature(const std::string& name, Datum::DatumType type, size_t size, size_t offset) :
		mName(name), mType(type), mSize(size), mOffset(offset)
	{}

#pragma region Manager
	HashMap<RTTI::IdType, AttributedTypeManager::TypeInfo> AttributedTypeManager::sTypeMap;

	AttributedTypeManager::TypeInfo::TypeInfo(const Vector<Attributed::Signature>& signature, RTTI::IdType parentType) :
		mSignatures(signature),
		mParentType(parentType)
	{}

	void AttributedTypeManager::RegisterType(RTTI::IdType type, const Vector<Attributed::Signature>& signatures, RTTI::IdType parentType)
	{
		assert(type != parentType);
		sTypeMap.Insert(make_pair(type, TypeInfo(signatures, parentType)));
	}

	const Vector<Attributed::Signature>* AttributedTypeManager::GetSignature(RTTI::IdType type)
	{
		auto it = sTypeMap.Find(type);
		if (it != sTypeMap.end())
		{
			return &it->second.mFinalSignatures;
		}

		return nullptr;
	}

	void AttributedTypeManager::FinalizeSignature()
	{
		for (auto& pair : sTypeMap)
		{
			auto& type = pair.first;
			auto& info = pair.second;

			// Push all types in the hierarchy to the stack, top is ancestor, bottom is descendent
			Stack<Vector<Attributed::Signature>*> typeStack;
			HashMap<RTTI::IdType, TypeInfo>::Iterator it = sTypeMap.Find(type);
			size_t signatureCount = 0;
			while (it != sTypeMap.end())
			{
				typeStack.Push(&it->second.mSignatures);
				signatureCount += it->second.mSignatures.Size();
				it = sTypeMap.Find(it->second.mParentType);
			}

			// Pop from top to build the entire signature list
			info.mFinalSignatures.Clear();
			info.mFinalSignatures.Reserve(signatureCount);
			while (!typeStack.IsEmpty())
			{
				auto& partialSignature = typeStack.Peak();
				for (const auto& sig : *partialSignature)
				{
					info.mFinalSignatures.PushBack(sig);
				}
				typeStack.Pop();
			}
		}
	}

	void AttributedTypeManager::UnregisterType(RTTI::IdType type)
	{
		sTypeMap.Remove(type);
	}

	void AttributedTypeManager::UnregisterAllTypes()
	{
		sTypeMap.Clear();
	}
#pragma endregion
}