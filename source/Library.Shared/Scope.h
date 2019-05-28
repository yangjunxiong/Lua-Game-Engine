#pragma once
#include "RTTI.h"
#include "vector.h"
#include "HashMap.h"
#include "Datum.h"
#include <string>
#include <functional>
#include <gsl/gsl>

namespace GameEngine
{
	/// <summary>
	/// Scope is a data table that stores string -> datum pair, it also maintains the order of insertion of each entry.
	/// Scope can have child scopes stored in datum entry and therefore form a hierarchical structure.
	/// User can retreive scope entry by passing std::string as key or by passing unsigned int as index
	/// </summary>
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI);

	public:
		/// <summary>
		/// Default constructor, create a Scope with given capacity
		/// </summary>
		explicit Scope(size_t capacity = 8);

		/// <summary>
		/// Copy constructor, deep copy the contents from another Scope to this one.
		/// All children scopes will be duplicated and reparent to this one.
		/// </summary>
		/// <param name="other">The other Scope to copy from</param>
		Scope(const Scope& other);

		/// <summary>
		/// Move constructor, shallow copy the contents from another Scope to this one.
		/// All children scopes will be reparented to this one.
		/// </summary>
		/// <param name="other">The other Scope to move from</param>
		Scope(Scope&& other);

		/// <summary>
		/// Copy assignment. Clear current scope and deep copy the contents from another Scope to this one.
		/// All children scopes will be duplicated and reparent to this one.
		/// </summary>
		/// <param name="other">The other Scope to copy from</param>
		/// <returns>The Scope after copying</returns>
		Scope& operator=(const Scope& other);

		/// <summary>
		/// Move assignment. Clear current scope and shallow copy the contents from another Scope to this one.
		/// All children scopes will be reparented to this one.
		/// </summary>
		/// <param name="other">The other Scope to move from</param>
		/// <returns>The Scope after Moving</returns>
		Scope& operator=(Scope&& other);

		/// <summary>
		/// Try to get the Datum with given key. If the entry doesn't exist, a new entry will be appended. Equivalent to append()
		/// </summary>
		/// <param name="key">The key of table pair</param>
		/// <returns>The Datum reference at given key</returns>
		/// <exception cref="std::exception">key is empty string</exception>
		Datum& operator[](const std::string& key);

		/// <summary>
		/// Try to get the Datum with given key.
		/// </summary>
		/// <param name="key">The key of table pair</param>
		/// <returns>The Datum reference at given key</returns>
		/// <exception cref="std::exception">The key doesn't exist in scope</exception>
		const Datum& operator[](const std::string& key) const;

		/// <summary>
		/// Try to get the Datum with given insertion order. e.g. if index = 3, will return the third inserted Datum
		/// </summary>
		/// <param name="index">The order of insertion of the Datum</param>
		/// <returns>The Datum reference at given insertion index</returns>
		/// <exception cref="std::exception">Index equal or greater than table size</exception>
		Datum& operator[](size_t index);

		/// <summary>
		/// Try to get the Datum with given insertion order. e.g. if index = 3, will return the third inserted Datum
		/// </summary>
		/// <param name="index">The order of insertion of the Datum</param>
		/// <returns>The Datum reference at given insertion index</returns>
		/// <exception cref="std::exception">Index equal or greater than table size</exception>
		const Datum& operator[](size_t index) const;

		/// <summary>
		/// Check if two scopes are equal. This method will invoke operator== on Datum and therefore recursively compare all nested scopes.
		/// Note that the insertion order is not considered in comparison, only the contents in table are compared.
		/// Two Scopes are equal if all of their table pairs pass one-to-one equality check
		/// </summary>
		/// <param name="other">The other Scope to compare with</param>
		/// <returns>True if two Scopes are equal, False otherwise</returns>
		bool operator==(const Scope& other) const;

		/// <summary>
		/// Check if two scopes are equal. This method will invoke operator== on Datum and therefore recursively compare all nested scopes.
		/// Note that the insertion order is not considered in comparison, only the contents in table are compared.
		/// Two Scopes are equal if all of their table pairs pass one-to-one equality check
		/// </summary>
		/// <param name="other">The other Scope to compare with</param>
		/// <returns>True if two Scopes are not equal, False otherwise</returns>
		bool operator!=(const Scope& other) const;

		/// <summary>
		/// Desctructor. Free all memory owned by this Scope, will delete all nested scopes recursively
		/// </summary>
		virtual ~Scope();

		/// <summary>
		/// Convert the content of scope to a JSON string format
		/// </summary>
		/// <returns>The JSON representation of Scope</returns>
		virtual std::string ToString() const override;

		/// <summary>
		/// Check if this Scope is equivalent to another RTTI, will invoke equality operator if rhs is also a Scope
		/// </summary>
		/// <param name="rhs">The other RTTI to compare with</param>
		/// <returns>True if both are equal, False otherwise</returns>
		virtual bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Create a clone of this scope, and return its pointer. Caller is responsible to delete the object
		/// </summary>
		/// <returns>Pointer to the new clone</returns>
		virtual gsl::owner<Scope*> Clone() const;

		/// <summary>
		/// Search for the table and find the pair with given key, return the corresponding Datum pointer
		/// </summary>
		/// <param name="key">The key to search for</param>
		/// <returns>The Datum pointer to the found item, or nullptr if no such key exists</returns>
		Datum* Find(const std::string& key);

		/// <summary>
		/// Search for the table and find the pair with given key, return the corresponding Datum pointer
		/// </summary>
		/// <param name="key">The key to search for</param>
		/// <returns>The Datum pointer to the found item, or nullptr if no such key exists</returns>
		const Datum* Find(const std::string& key) const;

		/// <summary>
		/// Search for a path in current scope and all ancestor scopes, and return the corresponding Datum pointer and the Scope pointer which contains it.
		/// If the path contains "/", it will be treated similar to how file system works.
		/// "/" at the beginning means to use absolute path, "./" means under current scope, "../" means in parent scope
		/// Besides, after the datum key can follow [size_t / string] to get item from array, if you supply string here, the table must contain that field.
		/// Example: "/Sectors[0]/Entities[1]/Actions[Increment]/Target"
		/// </summary>
		/// <param name="path">The path to search for</param>
		/// <returns>Pair of Datum pointer and Scope pointer which contains the Datum. If not found, both will be nullptr</returns>
		std::pair<Datum*, Scope*> Search(const std::string& path);
		std::pair<const Datum*, const Scope*> Search(const std::string& path) const;

		/// <summary>
		/// Find the root scope in the ancestor tree, if this scope has no parent, will return itself
		/// </summary>
		/// <returns>The root scope in the ancestor tree</returns>
		Scope* GetRoot();
		const Scope* GetRoot() const;

		/// <summary>
		/// Insert a new entry to table, if there doesn't exist one with same key
		/// </summary>
		/// <param name="key">The key of table pair</param>
		/// <returns>The Datum reference at given key, and whether or not a new pair is inserted</returns>
		/// <exception cref="std::excpetion">Key is empty string</exception>
		std::pair<Datum&, bool> Append(const std::string& key);

		/// <summary>
		/// Create a new nested scope and push it back to the Datum at given key
		/// </summary>
		/// <param name="key">The key of table pair</param>
		/// <returns>The created nested Scope</returns>
		/// <exception cref="std::exception">The key is empty string</exception>
		/// <exception cref="std::exception">The Datum with same exists and it's not table type</exception>
		Scope& AppendScope(const std::string& key);

		/// <summary>
		/// Adopt a Scope and push it back to the Datum at given key. Will reparent the Scope to this one
		/// </summary>
		/// <param name="child">The child Scope to adopt</param>
		/// <param name="key">The key of Datum</param>
		/// <exception cref="std::exception">Key is empty string</exception>
		/// <exception cref="std::exception">The Datum with same exists and it's not table type</exception>
		/// <exception cref="std::exception">Adopt self or ancestor</exception>
		void Adopt(Scope& child, const std::string& key);

		/// <summary>
		/// Remove a nested Scope from parent scope
		/// </summary>
		/// <param name="child">The child Scope to remove</param>
		void Orphan(Scope& child);

		/// <summary>
		/// Get the parent Scope of this Scope
		/// </summary>
		/// <returns>The parent Scope of this Scope</returns>
		Scope* GetParent() const;

		/// <summary>
		/// Get the parent Scope of this Scope, with specific type
		/// </summary>
		/// <returns>The parent Scope of this Scope, if this Scope has no parent or parent's type is not T, will return nullptr</returns>
		template <typename T>
		T* GetParent() const;

		/// <summary>
		/// Get the size of table
		/// </summary>
		/// <returns>Size of table</returns>
		size_t Size() const;

		/// <summary>
		/// Find the nested scope with same address
		/// </summary>
		/// <param name="scope">The scope address to find</param>
		/// <returns>Pair of string to Datum pointer that contains the nested scope</returns>
		std::pair<std::string, Datum*> FindScope(const Scope* scope);

		/// <summary>
		/// Find the nested scope with same address
		/// </summary>
		/// <param name="scope">The scope address to find</param>
		/// <returns>Pair of string to Datum pointer that contains the nested scope</returns>
		std::pair<std::string, const Datum*> FindScope(const Scope* scope) const;

		/// <summary>
		/// Given a datum reference, find the key for the datum in this scope.
		/// If this datum doesn't exist in this scope, return empty string
		/// </summary>
		/// <param name="datum">The datum to search for</param>
		/// <returns>The key of given datum in the scope</returns>
		std::string FindKey(const Datum& datum) const;

		/// <summary>
		/// Check if the passed scope is ancestor of this one (this one is descendent of passed in Scope)
		/// </summary>
		/// <param name="other">The scope to check</param>
		/// <returns>True if passed in scope is ancestor of this one, false otherwise</returns>
		bool IsDescendentOf(Scope& other);

		/// <summary>
		/// Destroy all table entries, delete all nested scopes. Don't necessarily free all memory allocated by container
		/// </summary>
		virtual void Clear();

	protected:
		using TablePairType = std::pair<const std::string, Datum>;
		using TableType = HashMap<std::string, Datum>;
		using TableIteratorType = TableType::Iterator;

		/// <summary>
		/// HashMap of string->Datum pair, used to store actual table pairs
		/// </summary>
		TableType mTable;

		/// <summary>
		/// Store the pointers to HashMap pairs, use to maintain the insertion order
		/// </summary>
		Vector<TablePairType*> mDatumPointers;

	private:
		/// <summary>
		/// Pointer to parent scope
		/// </summary>
		Scope* mParent = nullptr;

		/// <summary>
		/// Deep copy the contents from another Scope
		/// </summary>
		/// <param name="other">The other Scope to copy from</param>
		void DeepCopy(const Scope& other);

		/// <summary>
		/// Move the contents from another scope
		/// </summary>
		/// <param name="other">The other Scope to move from</param>
		void MoveFrom(Scope&& other);

		/// <summary>
		/// Helper function to iterate through all nested scopes, and do some actions on matching scope(s)
		/// </summary>
		/// <param name="action">The function to perform on each nested scope</param>
		/// <param name="address">The special address of scope to look for, nullptr will include all scopes. Can be changed to predicate, but it can satisfy all use cases for now</param>
		/// <param name="oneShot">Whether to stop iteration after performing action on first matching Scope</param>
		void ForEachChildScope(const std::function<void(const std::string&, Datum&, size_t, Scope&)>& action, const Scope* address = nullptr, bool oneShot = false);
	
		/// <summary>
		/// Internal clear function, don't use public one because we don't want polymorphic behavior
		/// </summary>
		void _Clear();

		bool ParseSearchToken(std::string& token, Datum*& outDatum, Scope*& outScope);
	};

	template <typename T>
	T* Scope::GetParent() const
	{
		if (mParent != nullptr)
		{
			return mParent->As<T>();
		}
		return nullptr;
	}
}
