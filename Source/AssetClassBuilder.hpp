#pragma once

#include <memory>

#include "AssetClass.hpp"

class AssetClassBuilder
{
public:
    AssetClassBuilder();
    ~AssetClassBuilder() = default;

    std::shared_ptr<AssetClass> GetAssetClass();

private:
    std::shared_ptr<AssetClass> mAssetClass;

    bool mConstructedSuccessfully {false};
};