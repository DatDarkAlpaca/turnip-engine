#pragma once
#include <filesystem>
#include "ConfigData.hpp"

namespace tur
{
    class IConfigParser
    {
    public:
        virtual ~IConfigParser() = default;

    public:
        virtual ConfigData Load(const std::filesystem::path& filepath) = 0;

        virtual void Save(const ConfigData& config, const std::filesystem::path& filepath) = 0;
    };
}