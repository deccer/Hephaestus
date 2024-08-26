#pragma once

#include <vector>
#include <entt/entt.hpp>

struct TParentComponent {
    std::vector<entt::entity> Children;
};

struct TChildOfComponent {
    entt::entity Parent;
};