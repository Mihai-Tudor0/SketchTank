#include "internal/JsonStorage.h"

#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

namespace internal::JsonStorage {

namespace {

ItemCategory parseCategory(const std::string& raw) {
    if (raw == "decoration") {
        return ItemCategory::Decoration;
    }
    if (raw == "cleaner") {
        return ItemCategory::Cleaner;
    }
    return ItemCategory::Fish;
}

nlohmann::json readJsonFile(const std::string& path) {
    std::ifstream input(path);
    if (!input.is_open()) {
        throw std::runtime_error("Cannot open JSON file: " + path);
    }
    return nlohmann::json::parse(input);
}

}  // namespace

Aquarium loadCatalog(const std::string& catalogPath) {
    const nlohmann::json doc = readJsonFile(catalogPath);
    const auto& tank = doc.at("tank");

    std::vector<ColorPalette> palettes;
    for (const auto& entry : doc.at("palettes")) {
        palettes.emplace_back(
            entry.at("id").get<std::string>(),
            entry.at("name").get<std::string>(),
            entry.at("hexColors").get<std::vector<std::string>>());
    }

    std::vector<ShopBlueprint> catalog;
    for (const auto& entry : doc.at("shopItems")) {
        catalog.emplace_back(
            entry.at("id").get<std::string>(),
            entry.at("name").get<std::string>(),
            parseCategory(entry.at("category").get<std::string>()),
            entry.at("price").get<int>(),
            entry.at("passiveIncome").get<int>(),
            entry.value("requiredLevel", 1),
            entry.value("unlocked", false));
    }

    return {
        tank.at("name").get<std::string>(),
        tank.at("width").get<int>(),
        tank.at("height").get<int>(),
        tank.at("startingCoins").get<int>(),
        tank.at("startingCleanliness").get<int>(),
        std::move(palettes),
        std::move(catalog),
    };
}

std::vector<PixelCanvas> loadSketches(const std::string& sketchesPath, const Aquarium& aquarium) {
    const nlohmann::json doc = readJsonFile(sketchesPath);
    std::vector<PixelCanvas> sketches;
    for (const auto& entry : doc.at("sketches")) {
        sketches.emplace_back(
            entry.at("title").get<std::string>(),
            entry.at("cells").get<std::vector<std::vector<int>>>(),
            aquarium.paletteById(entry.at("paletteId").get<std::string>()));
    }
    return sketches;
}

}  // namespace internal::JsonStorage
