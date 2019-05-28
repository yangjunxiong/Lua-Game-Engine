#pragma once

#include "SList.h"

namespace GameEngine
{
	/// <summary>
	/// Stack is just a wrapper of SList. It only exposes the front of SList
	/// </summary>
	template <typename T>
	class Stack final
	{
	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		Stack() = default;

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="other">Other Stack to copy from</param>
		Stack(const Stack& other) = default;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other">Other Stack to move from</param>
		Stack(Stack&& other) = default;

		/// <summary>
		/// Copy assignment
		/// </summary>
		/// <param name="other">Other Stack to copy from</param>
		/// <returns>This stack after copying</returns>
		Stack& operator=(const Stack& other) = default;

		/// <summary>
		/// Move assignment
		/// </summary>
		/// <param name="other">Other Stack to move from</param>
		/// <returns>This stack after moving</returns>
		Stack& operator=(Stack&& other) = default;

		/// <summary>
		/// Destructor that does nothing
		/// </summary>
		~Stack() = default;

		/// <summary>
		/// Add an item to the top of stack
		/// </summary>
		/// <param name="item">Item to push</param>
		/// <returns>Pushed item in the stack</returns>
		T& Push(const T& item);

		/// <summary>
		/// Remove the item at the top
		/// </summary>
		void Pop();

		/// <summary>
		/// Get the item at the top
		/// </summary>
		/// <returns>The item at the top</returns>
		T& Peak();

		/// <summary>
		/// Get the item at the top
		/// </summary>
		/// <returns>The item at the top</returns>
		const T& Peak() const;

		/// <summary>
		/// Get the size of stack
		/// </summary>
		/// <returns>Size of stack</returns>
		size_t Size() const;

		/// <summary>
		/// Get if the stack is empty
		/// </summary>
		/// <returns>True if the stack is empty</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Remove all items in the stack
		/// </summary>
		void Clear();

	private:
		/// <summary>
		/// The SList that actually stores the data
		/// </summary>
		SList<T> mList;
	};
}

#include "Stack.inl"