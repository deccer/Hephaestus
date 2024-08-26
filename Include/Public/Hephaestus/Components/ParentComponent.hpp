#pragma once

#include <vector>
#include <entt/entt.hpp>

struct SParentComponent {
    std::vector<entt::entity> Children;
};

struct SChildOfComponent {
    entt::entity Parent;
};