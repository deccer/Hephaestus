#include <Hephaestus/DefaultScene.hpp>

#include <Hephaestus/Assets/Assets.hpp>

#include <spdlog/spdlog.h>

TDefaultScene::~TDefaultScene() {

}

auto TDefaultScene::Load() -> bool {

    //auto scannedAssetResult = ScanAsset("SM_Bistro", "/home/deccer/Code/scenes/Bistro/scene.gltf");
    auto scannedAssetResult = ScanAsset("SM_Bistro", "/home/deccer/Downloads/modular_ruins_c/modular_ruins_c.glb");
    if (!scannedAssetResult) {
        spdlog::error(scannedAssetResult.error());
        return false;
    }

    auto scannedAsset = *scannedAssetResult;

    return true;
}