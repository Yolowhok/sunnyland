#pragma once

#include "assets/Texture.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace assets {

class AssetManager {
public:
    explicit AssetManager(std::string rootDirectory = "assets");

    Texture& getTexture(const std::string& relativePath);

private:
    Texture loadTexture(const std::string& fullPath);

    std::string root_;
    std::unordered_map<std::string, std::unique_ptr<Texture>> cache_;
};

} // namespace assets
