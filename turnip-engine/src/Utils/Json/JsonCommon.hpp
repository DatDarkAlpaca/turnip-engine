#pragma once
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

namespace glm
{
    inline void to_json(nlohmann::json& j, const glm::vec2& vec)
    {
        j = { { "x", vec.x }, { "y", vec.y } };
    };

    inline void from_json(const nlohmann::json& j, glm::vec2& vec)
    {
        vec.x = j.at("x").get<float>();
        vec.y = j.at("y").get<float>();
    }
}