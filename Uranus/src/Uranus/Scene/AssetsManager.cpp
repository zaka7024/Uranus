#include "urpch.h"
#include "AssetsManager.h"

namespace Uranus {

	std::vector<Ref<Asset>> _Assets;

	const Ref<Asset> AssetsManager::Lockup(std::filesystem::path filepath)
	{
		for (auto& asset : _Assets) {
			if (asset->GetAssetFilePath() == filepath) {
				return asset;
			}
		}
		return Ref<Asset>();
	}

	void AssetsManager::AddAsset(const Ref<Asset>& asset)
	{
		for (auto& _asset : _Assets) {
			if (_asset->GetAssetFilePath() == asset->GetAssetFilePath()) {
				return;
			}
		}
		_Assets.push_back(asset);
	}
}
