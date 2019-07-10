#pragma once
#include "ContentManager.h"

using namespace std;

namespace GameEngine
{
	inline const std::map<std::wstring, std::shared_ptr<RTTI>>& ContentManager::LoadedAssets() const
	{
		return mLoadedAssets;
	}

	inline const std::wstring& ContentManager::RootDirectory() const
	{
		return mRootDirectory;
	}

	inline void ContentManager::SetRootDirectory(const std::wstring& rootDirectory)
	{
		mRootDirectory = rootDirectory + (StringHelper::EndsWith(rootDirectory, L"\\") ? std::wstring() : L"\\");
	}

	template<typename T>
	inline std::shared_ptr<T> ContentManager::Load(const std::wstring& assetName, bool reload, std::function<std::shared_ptr<T>(std::wstring&)> customReader)
	{
		if (reload == false)
		{
			auto it = mLoadedAssets.find(assetName);
			if (it != mLoadedAssets.end())
			{
				return static_pointer_cast<T>(it->second);
			}
		}

		uint64_t targetTypeId = T::TypeIdClass();
		auto pathName = mRootDirectory + assetName;
		auto asset = (customReader != nullptr ? customReader(pathName) : ReadAsset(targetTypeId, pathName));
		mLoadedAssets[assetName] = asset;

		return static_pointer_cast<T>(asset);
	}
}