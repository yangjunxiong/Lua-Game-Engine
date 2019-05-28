#pragma once
#include <string>
#include <exception>
#include "gsl/gsl"
#include "HashMap.h"
#include "RTTI.h"

namespace GameEngine
{
	/// <summary>
	/// The factory abstruct class.
	/// Expose necessary methods for users to create objects with name.
	/// Before using this, must use DECLARE_FACTORY macro to create concrete factory and instantiate one (and only one) concrete factory instance.
	/// </summary>
	/// <example>
	/// DECLARE_FACTORY(Foo, BaseType);              // Define concrete factory type
	/// FooFactory factory;                          // This line will register concrete factory
	/// Foo foo = Factory<BaseType>::Create("Foo");
	/// </example>
	template <typename T>
	class Factory
	{
	public:
		/// <summary>
		/// Default desctructor, do nothing
		/// </summary>
		virtual ~Factory<T>() = default;

		/// <summary>
		/// Find the concrete factory instance for the class with given name.
		/// </summary>
		/// <param name="name">Name of class</param>
		/// <returns>The instance of concrete factory of given class, or nullptr if not found</returns>
		static const Factory<T>* Find(const std::string& name);

		/// <summary>
		/// Create an instance of class with given name.
		/// </summary>
		/// <param name="name">Name of class</param>
		/// <returns>A default-constructed instance of given class, or nullptr if the factory for such class doesn't exist.</returns>
		static gsl::owner<T*> Create(const std::string& name);

		/// <summary>
		/// Create an instance of class for this factory.
		/// Note this is not static method and must be called on concrete factory instance.
		/// </summary>
		/// <returns>An instance of class for this factory.</returns>
		virtual gsl::owner<T*> Create() const = 0;

		/// <summary>
		/// Get the class name for this factory.
		/// Note this is not static method and must be called on concrete factory instance.
		/// For example, if this factory type is "FooFactory", then class name is "Foo".
		/// </summary>
		/// <returns>Name of class for this factory</returns>
		virtual const std::string& ClassName() const = 0;

	protected:
		/// <summary>
		/// Register a concrete factory instance to the generic factory map
		/// </summary>
		/// <param name="factory">The instance of concrete factory to add</param>
		/// <exception cref="std::exception">A concrete factory instance with same class name already exists</exception>
		static void Add(const Factory<T>& factory);

		/// <summary>
		/// Remove a concrete factory instance from the generic factory map
		/// </summary>
		/// <param name="factory">The instance of concrete factory to be removed</param>
		static void Remove(const Factory<T>& factory);

	private:
		/// <summary>
		/// The map from class name to corresponding concrete factory instance
		/// </summary>
		static HashMap<std::string, const Factory<T>*> sFactoryMap;
	};

	#define DECLARE_FACTORY(_type, _baseType)													 \
		class _type##Factory final : public GameEngine::Factory<_baseType>						 \
		{																						 \
		public:																					 \
			_type##Factory() { GameEngine::Factory<_baseType>::Add(*this); }					 \
			~_type##Factory() { GameEngine::Factory<_baseType>::Remove(*this); }				 \
			_type##Factory(const _type##Factory& other) = delete;								 \
			_type##Factory(_type##Factory&& other) = delete;									 \
			_type##Factory& operator=(const _type##Factory& other) = delete;					 \
			_type##Factory& operator=(_type##Factory&& other) = delete;							 \
			virtual gsl::owner<_baseType*> Create() const override { return new _type(); }		 \
			virtual const std::string& ClassName() const override { return mName; }			     \
		private:																				 \
			const std::string mName = #_type;													 \
		};
}

#include "Factory.inl"
