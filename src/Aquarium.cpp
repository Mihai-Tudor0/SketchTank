#include "Aquarium.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

#include "internal/JsonStorage.h"

Aquarium::Aquarium(std::string tankName,
                   const int width,
                   const int height,
                   const int startingCoins,
                   const int startingCleanliness,
                   std::vector<ColorPalette> palettes,
                   std::vector<ShopBlueprint> catalog)
    : tankName_(std::move(tankName)),
      width_(width),
      height_(height),
      coins_(startingCoins),
      cleanliness_(std::clamp(startingCleanliness, 0, 100)),
      palettes_(std::move(palettes)),
      catalog_(std::move(catalog)) {
    if (tankName_.empty() || width_ < 10 || height_ < 6 || palettes_.empty() || catalog_.empty()) {
        throw std::invalid_argument("Invalid Aquarium configuration.");
    }
}

Aquarium Aquarium::loadFromCatalogFile(const std::string& catalogPath) {
    return internal::JsonStorage::loadCatalog(catalogPath);
}

std::vector<PixelCanvas> Aquarium::loadStarterSketches(const std::string& sketchesPath) const {
    return internal::JsonStorage::loadSketches(sketchesPath, *this);
}

bool Aquarium::canUnlockItem(const std::string& itemId) const {
    const ShopBlueprint& item = blueprintById(itemId);
    return !item.isUnlocked() && coins_ >= item.price() && level_ >= item.requiredLevel();
}

bool Aquarium::unlockItem(const std::string& itemId, const PixelCanvas& sketch) {
    if (!canUnlockItem(itemId)) {
        return false;
    }
    ShopBlueprint& item = catalog_[findBlueprintIndex(itemId)];
    if (!item.unlockWithSketch(sketch)) {
        return false;
    }
    coins_ -= item.price();
    xp_ += 20;
    level_ = 1 + (xp_ / 20);
    return true;
}

bool Aquarium::placeItem(const std::string& itemId, const std::string& paletteId, const std::string& nickname) {
    const ShopBlueprint& item = blueprintById(itemId);
    if (!item.isUnlocked()) {
        return false;
    }
    const ColorPalette& palette = paletteById(paletteId);
    const std::string finalName = nickname.empty()
                                      ? (item.name() + " #" + std::to_string(inhabitants_.size() + 1))
                                      : nickname;
    const PixelCanvas sprite = item.makeColoredSprite(palette);
    const int idx = static_cast<int>(inhabitants_.size());
    const int startX = (idx * 7) % std::max(1, width_ - sprite.width());
    const int startY = (item.category() == ItemCategory::Fish) ? (1 + (idx * 3) % std::max(1, height_ - sprite.height())) : height_;
    inhabitants_.emplace_back(finalName, item.category(), item.passiveIncome(), sprite, startX, startY, width_, height_);
    return true;
}

int Aquarium::simulateStep(const bool feedFish, const bool cleanTank) {
    ++tickCount_;
    if (cleanTank) {
        cleanliness_ = std::min(100, cleanliness_ + 30);
    } else {
        cleanliness_ = std::max(0, cleanliness_ - static_cast<int>(inhabitants_.size()) * 2);
    }

    int earned = 0;
    int autoClean = 0;
    for (TankEntity& entity : inhabitants_) {
        earned += entity.advanceStep(width_, height_, feedFish, cleanliness_);
        autoClean += entity.autoCleanPower();
    }
    cleanliness_ = std::min(100, cleanliness_ + autoClean);
    coins_ += earned;
    xp_ += feedFish ? 10 : 5;
    level_ = 1 + (xp_ / 20);
    return earned;
}

std::string Aquarium::renderTankScene() const {
    const bool murky = cleanliness_ < 45;
    const std::string waterBg = murky ? "\x1b[48;2;18;45;22m  \x1b[0m" : "\x1b[48;2;10;30;55m  \x1b[0m";
    std::vector<std::vector<std::string>> grid(
        static_cast<std::size_t>(height_),
        std::vector<std::string>(static_cast<std::size_t>(width_), waterBg));

    for (const TankEntity& entity : inhabitants_) {
        for (int r = 0; r < entity.spriteHeight(); ++r) {
            for (int c = 0; c < entity.spriteWidth(); ++c) {
                const std::string block = entity.renderedAnsiAt(r, c);
                if (block != "  ") {
                    const int gy = entity.posY() + r;
                    const int gx = entity.posX() + c;
                    if (gy >= 0 && gy < height_ && gx >= 0 && gx < width_) {
                        grid[static_cast<std::size_t>(gy)][static_cast<std::size_t>(gx)] = block;
                    }
                }
            }
        }
    }

    std::string out = tankName_ + " | Nivel " + std::to_string(level_) + " | Monede: " + std::to_string(coins_) +
                      " | Curatenie: " + std::to_string(cleanliness_) + "%\n+";
    out += std::string(static_cast<std::size_t>(width_ * 2), '-') + "+\n";
    for (const auto& row : grid) {
        out += '|';
        for (const auto& cell : row) {
            out += cell;
        }
        out += "|\n";
    }
    out += "+" + std::string(static_cast<std::size_t>(width_ * 2), '-') + "+\n";
    return out;
}

int Aquarium::coins() const { return coins_; }
int Aquarium::level() const { return level_; }
int Aquarium::xp() const { return xp_; }
std::size_t Aquarium::inhabitantCount() const { return inhabitants_.size(); }

std::size_t Aquarium::findPaletteIndex(const std::string& paletteId) const {
    for (std::size_t i = 0; i < palettes_.size(); ++i) {
        if (palettes_[i].id() == paletteId) {
            return i;
        }
    }
    throw std::out_of_range("Unknown palette id: " + paletteId);
}

std::size_t Aquarium::findBlueprintIndex(const std::string& itemId) const {
    for (std::size_t i = 0; i < catalog_.size(); ++i) {
        if (catalog_[i].id() == itemId) {
            return i;
        }
    }
    throw std::out_of_range("Unknown shop item id: " + itemId);
}

const ColorPalette& Aquarium::paletteById(const std::string& paletteId) const {
    return palettes_[findPaletteIndex(paletteId)];
}

const ShopBlueprint& Aquarium::blueprintById(const std::string& itemId) const {
    return catalog_[findBlueprintIndex(itemId)];
}

std::vector<std::string> Aquarium::paletteIds() const {
    std::vector<std::string> ids;
    ids.reserve(palettes_.size());
    for (const ColorPalette& p : palettes_) {
        ids.push_back(p.id());
    }
    return ids;
}

std::vector<std::string> Aquarium::catalogItemIds() const {
    std::vector<std::string> ids;
    ids.reserve(catalog_.size());
    for (const ShopBlueprint& b : catalog_) {
        ids.push_back(b.id());
    }
    return ids;
}

std::ostream& operator<<(std::ostream& os, const Aquarium& aquarium) {
    os << aquarium.renderTankScene() << "Creaturi (" << aquarium.inhabitants_.size() << "):\n";
    for (const TankEntity& entity : aquarium.inhabitants_) {
        os << " - " << entity;
    }
    return os;
}
