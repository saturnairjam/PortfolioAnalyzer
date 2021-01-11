#pragma once

#include <map>
#include <memory>

#include "rapidjson/document.h"

#include "AssetClass.hpp"

class AssetClassBuilder
{
public:
    AssetClassBuilder(const rapidjson::Document& dom);

    ~AssetClassBuilder() = default;

    std::shared_ptr<AssetClass> GetAssetClass();

private:
    std::shared_ptr<AssetClass> mAssetClass;

    bool mConstructedSuccessfully {false};
};