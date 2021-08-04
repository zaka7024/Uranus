#pragma once

#include "Uranus/Core/Core.h"

#include <filesystem>

namespace Uranus {

	enum class AssetType {
		Texture,
		Audio,
		Script
	};

	class Asset {
	public:
		Asset() = default;
		virtual std::filesystem::path& GetAssetFilePath() = 0;

	protected:
		std::filesystem::path _Filepath;
		AssetType _AssetType;
	};
}
