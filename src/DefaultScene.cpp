#include <Hephaestus/DefaultScene.hpp>

#include <Hephaestus/Assets/Assets.hpp>

#include <spdlog/spdlog.h>

TDefaultScene::~TDefaultScene() {

}

auto TDefaultScene::Load() -> bool {

    auto scannedAssetResult = ScanAsset("/home/deccer/Code/scenes/Bistro/scene.gltf", "SM_Bistro");
    if (!scannedAssetResult) {
        spdlog::error(scannedAssetResult.error());
        return false;
    }

    auto scannedAsset = *scannedAssetResult;

    return true;
}