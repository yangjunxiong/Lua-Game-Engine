#pragma once

namespace GameEngine
{
	template <typename T>
	HashMap<std::string, const Factory<T>*> Factory<T>::sFactoryMap;

	template <typename T>
	const Factory<T>* Factory<T>::Find(const std::string& name)
	{
		auto it = sFactoryMap.Find(name);
		return it == sFactoryMap.end() ? nullptr : (it->second);
	}

	template <typename T>
	gsl::owner<T*> Factory<T>::Create(const std::string& name)
	{
		auto it = sFactoryMap.Find(name);
		if (it != sFactoryMap.end())
		{
			return it->second->Create();
		}

		return nullptr;
	}

	template <typename T>
	void Factory<T>::Add(const Factory<T>& factory)
	{
		if (sFactoryMap.ContainsKey(factory.ClassName()))
		{
			throw std::exception("Add factory with existing name");
		}
		sFactoryMap[factory.ClassName()] = &factory;
	}

	template <typename T>
	void Factory<T>::Remove(const Factory<T>& factory)
	{
		sFactoryMap.Remove(factory.ClassName());
	}
}