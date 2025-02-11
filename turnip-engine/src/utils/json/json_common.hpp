#pragma once
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

namespace glm
{
    inline void to_json(nlohmann::json& json, const glm::vec2& vector)
    {
        json = { { "x", vector.x }, { "y", vector.y } };
    };

    inline void from_json(const nlohmann::json& json, glm::vec2& vector)
    {
        vector.x = json.at("x").get<float>();
        vector.y = json.at("y").get<float>();
    }

    inline void to_json(nlohmann::json& json, const glm::uvec2& vector)
    {
        json = { { "x", vector.x }, { "y", vector.y } };
    };

    inline void from_json(const nlohmann::json& json, glm::uvec2& vector)
    {
        vector.x = json.at("x").get<u32>();
        vector.y = json.at("y").get<u32>();
    }
}