#pragma once

#include <Hephaestus/Scene.hpp>

class TDefaultScene : public TScene {
public:
    ~TDefaultScene() override;

    auto Load() -> bool override;
};