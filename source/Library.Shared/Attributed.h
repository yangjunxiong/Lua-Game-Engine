#pragma once
#include "vector.h"
#include "Datum.h"
#include "HashMap.h"
#include "Scope.h"

namespace GameEngine
{
	/// <summary>
	/// An Attributed is a Scope representation of any object in game. It allows auto creation of Scope for any type that is registered in AttributedTypeManager.
	/// An registered type has prescribed attributes that are universal for all instances of that type.
	/// An instance of object can have auxiliary attributes that are unique to that instance.
	/// </summary>
	CLASS(NoLuaAuthority)
	class Attributed : public Scope
	{
		using PairType = Scope::TablePairType;
		RTTI_DECLARATIONS(Attributed, Scope);

	public:
		/// <summary>
		/// A description of one prescribed attribute. AttributedTypeManager stores a list of Signatures for each registered type to automatically create
		/// Scope whenever a registered class is constructed.
		/// </summary>
		struct Signature final
		{
			/// <summary>
			/// Only constructor
			/// </summary>
			/// <param name="name">Name of the attribute</param>
			/// <param name="type">Type of the attribute</param>
			/// <param name="size">How many elements does this attribute have</param>
			/// <param name="offset">The offset in bytes from start of instance base address to the address of member variable</param>
			Signature(const std::string& name, Datum::DatumType type, size_t size, size_t offset = 0);

			/// <summary>
			/// Name of attribute
			/// </summary>
			std::string mName;

			/// <summary>
			/// Type of attribute
			/// </summary>
			Datum::DatumType mType;

			/// <summary>
			/// Number of elements in the attribute
			/// </summary>
			size_t mSize;

			/// <summary>
			/// The offset in bytes from start of instance base address to the address of member variable.
			/// If offset is 0, the attribute is treated as internal storage
			/// </summary>
			size_t mOffset;
		};

		/// <summary>
		/// Desctructor, does nothing
		/// </summary>
		virtual ~Attributed() = default;

		/// <summary>
		/// Check if this instance has an attribute with given name
		/// </summary>
		/// <param name="name">The name to search</param>
		/// <returns>True if this instance has an attribute with such name, False otherwise</returns>
		bool IsAttribute(const std::string& name) const;

		/// <summary>
		/// Check if this instance has an prescribed attribute with given name
		/// </summary>
		/// <param name="name">The name to search</param>
		/// <returns>True if this instance has an prescribed attribute with such name, False otherwise</returns>
		bool IsPrescribedAttribute(const std::string& name) const;

		/// <summary>
		/// Check if this instance has an auxiliary attribute with given name
		/// </summary>
		/// <param name="name">The name to search</param>
		/// <returns>True if this instance has an auxiliary attribute with such name, False otherwise</returns>
		bool IsAuxiliaryAttribute(const std::string& name) const;

		/// <summary>
		/// Append a new auxiliary attribute with given name. If such auxiliary attribute exists, just return that one
		/// </summary>
		/// <param name="name">Name of attribute</param>
		/// <returns>The appended auxiliary attribute</returns>
		/// <exception cref="std::exception">One prescribed attribute has same name</exception>
		/// <exception cref="std::exception">Name is empty</exception>
		Datum& AppendAuxiliaryAttribute(const std::string& name);

		/// <summary>
		/// Get all attributes sorted by the order of insertion
		/// </summary>
		/// <returns>Vector of attribute pointers sorted by the order of insertion</returns>
		Vector<PairType*> Attributes();

		/// <summary>
		/// Get all attributes sorted by the order of insertion
		/// </summary>
		/// <returns>Vector of const attribute pointers sorted by the order of insertion</returns>
		Vector<const PairType*> Attributes() const;

		/// <summary>
		/// Get all prescribed attributes sorted by the order of insertion
		/// </summary>
		/// <returns>Vector of prescribed attribute pointers sorted by the order of insertion</returns>
		Vector<PairType*> PrescribedAttributes();

		/// <summary>
		/// Get all prescribed attributes sorted by the order of insertion
		/// </summary>
		/// <returns>Vector of const prescribed attribute pointers sorted by the order of insertion</returns>
		Vector<const PairType*> PrescribedAttributes() const;

		/// <summary>
		/// Get all auxiliary attributes sorted by the order of insertion
		/// </summary>
		/// <returns>Vector of auxiliary attribute pointers sorted by the order of insertion</returns>
		Vector<PairType*> AuxiliaryAttributes();

		/// <summary>
		/// Get all auxiliary attributes sorted by the order of insertion
		/// </summary>
		/// <returns>Vector of const auxiliary attribute pointers sorted by the order of insertion</returns>
		Vector<const PairType*> AuxiliaryAttributes() const;

		void RemoveAuxiliaryAttribute(const std::string& key);

		/// <summary>
		/// Clear the scope table, delete all auxiliary attributes and rebuild prescribed attributes to default state.
		/// </summary>
		virtual void Clear() override;

		/// <summary>
		/// Create a clone of this Attributed object with current data states
		/// </summary>
		/// <returns></returns>
		virtual gsl::owner<Scope*> Clone() const override;

	protected:
		/// <summary>
		/// Constructor that takes a type to automatically polulate prescribed attributes.
		/// Derived classes should pass in the type here
		/// </summary>
		/// <param name="type">Type of object</param>
		Attributed(IdType type);

		/// <summary>
		/// Copy constructor. Deep copy the contents in another Attributed to this one.
		/// Note that for prescribed attributes with external storage, pointers always point to member variables in this object, and the actual data is not copied in this function.
		/// </summary>
		/// <param name="other">The other Attributed to copy from</param>
		Attributed(const Attributed& other);

		/// <summary>
		/// Move constructor. Move the contents from another Attributed to this one
		/// Note that for prescribed attributes with external storage, pointers always point to member variables in this object, and the actual data is not moved in this function.
		/// </summary>
		/// <param name="other">The other Attributed to move from</param>
		Attributed(Attributed&& other);

		/// <summary>
		/// Copy assignment operator. Clear this object and deep copy the contents in another one.
		/// Note that for prescribed attributes with external storage, pointers always point to member variables in this object, and the actual data is not copied in this function.
		/// </summary>
		/// <param name="other">The other Attributed to copy from</param>
		/// <returns>This object after copying</returns>
		Attributed& operator=(const Attributed& other);

		/// <summary>
		/// Move assignment operator. Clear this object and deep move the contents in another one.
		/// Note that for prescribed attributes with external storage, pointers always point to member variables in this object, and the actual data is not moved in this function.
		/// </summary>
		/// <param name="other">The other Attributed to move from</param>
		/// <returns>This object after moving</returns>
		Attributed& operator=(Attributed&& other);

		/// <summary>
		/// Compare this object with another Attributed object. Use comparison from Scope but exclude "this" attribute, otherwise there will be infinite recursion
		/// </summary>
		/// <param name="other">The other Attributed object to compare with</param>
		/// <returns>True if two objects are equal, False otherwise</returns>
		bool operator==(const Attributed& other) const;

		/// <summary>
		/// Compare this object with another Attributed object. Use comparison from Scope but exclude "this" attribute, otherwise there will be infinite recursion
		/// </summary>
		/// <param name="other">The other Attributed object to compare with</param>
		/// <returns>True if two objects are not equal, False otherwise</returns>
		bool operator!=(const Attributed& other) const;

	private:
		/// <summary>
		/// Create prescribed attributes according to given class type.
		/// Prescribed attributes always start with "this", if the type has no corresponding signature table, no more prescribed attributes will be created
		/// </summary>
		/// <param name="type">Type of this object</param>
		void Populate(IdType type);

		/// <summary>
		/// After copying or moving, set prescribed attributes with external storage to let them point to member variables of this object
		/// </summary>
		/// <param name="type">Type of this object</param>
		void RedirectStorage(IdType type);

		/// <summary>
		/// Covert a vector of attributes to a vector of const attributes
		/// </summary>
		/// <param name="vector">The source vector of attributes</param>
		/// <returns>The copy of source vector with type changed to const attribute</returns>
		inline Vector<const PairType*> PromoteToConst(const Vector<PairType*>& vector) const;
	};

	/// <summary>
	/// A helper singleton class that stores all prescribed attributes information for all types
	/// </summary>
	class AttributedTypeManager final
	{
		struct TypeInfo
		{
			Vector<Attributed::Signature> mSignatures;
			Vector<Attributed::Signature> mFinalSignatures;
			RTTI::IdType mParentType;
			TypeInfo(const Vector<Attributed::Signature>& signature, RTTI::IdType parentType);
		};

	public:
		/// <summary>
		/// Add a type and its signature table
		/// </summary>
		/// <param name="type">Type of class</param>
		/// <param name="signatures">Vector of prescribed attribute signatures</param>
		static void RegisterType(RTTI::IdType type, const Vector<Attributed::Signature>& signatures, RTTI::IdType parentType);

		/// <summary>
		/// Retreive a signature table with given type
		/// </summary>
		/// <param name="type">Type of class</param>
		/// <returns>Vector of prescribed attribute signatures</returns>
		static const Vector<Attributed::Signature>* GetSignature(RTTI::IdType type);

		/// <summary>
		/// Remove a signature table with given type
		/// </summary>
		/// <param name="type">Type of class</param>
		static void UnregisterType(RTTI::IdType type);

		/// <summary>
		/// Remove all registered types, clear entire signature table
		/// </summary>
		static void UnregisterAllTypes();

		/// <summary>
		/// After all types are registered, generate the complete signature list for each type and store them as cache
		/// </summary>
		static void FinalizeSignature();

	private:
		/// <summary>
		/// Private constructor to implement singleton pattern
		/// </summary>
		AttributedTypeManager();

		/// <summary>
		/// A map that stores type->signature table
		/// </summary>
		static HashMap<RTTI::IdType, TypeInfo> sTypeMap;
	};
}