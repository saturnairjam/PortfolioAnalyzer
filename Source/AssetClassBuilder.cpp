#include "AssetClassBuilder.hpp"

AssetClassBuilder::AssetClassBuilder() { }

std::shared_ptr<AssetClass> AssetClassBuilder::GetAssetClass()
{
    if (mConstructedSuccessfully)
    {
        return mAssetClass;
    }
    else
    {
        return nullptr;
    }
}