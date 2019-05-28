#pragma once

using namespace GameEngine;
using namespace std;

template <typename T>
T& Stack<T>::Push(const T& item)
{
	return *mList.PushFront(item);
}

template <typename T>
void Stack<T>::Pop()
{
	mList.PopFront();
}

template <typename T>
T& Stack<T>::Peak()
{
	return mList.Front();
}

template <typename T>
const T& Stack<T>::Peak() const
{
	return mList.Front();
}

template <typename T>
size_t Stack<T>::Size() const
{
	return mList.Size();
}

template <typename T>
bool Stack<T>::IsEmpty() const
{
	return mList.IsEmpty();
}

template <typename T>
void Stack<T>::Clear()
{
	mList.Clear();
}