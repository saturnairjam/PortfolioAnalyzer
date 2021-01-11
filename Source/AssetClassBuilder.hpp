#pragma once

#include <memory>

#include "AssetClass.hpp"

class AssetClassBuilder
{
public:
    AssetClassBuilder() = default;
    ~AssetClassBuilder() = default;

    std::shared_ptr<AssetClass> GetAssetClass() { return mAssetClass; }

private:
    std::shared_ptr<AssetClass> mAssetClass;
};