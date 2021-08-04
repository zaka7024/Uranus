#pragma once

#include "Uranus/Core/Core.h"
#include "Asset.h"

namespace Uranus {

	class AssetsManager {
	public:
		static const Ref<Asset> Lockup(std::filesystem::path filepath);
		static void AddAsset(const Ref<Asset>& asset);
	};

}