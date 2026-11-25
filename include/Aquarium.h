#ifndef SKETCHTANK_AQUARIUM_H
#define SKETCHTANK_AQUARIUM_H

#include <cstddef>
#include <ostream>
#include <string>
#include <vector>

#include "ColorPalette.h"
#include "PixelCanvas.h"
#include "ShopBlueprint.h"
#include "TankEntity.h"

class Aquarium {
public:
    Aquarium(std::string tankName, int width, int height, int startingCoins,
             int startingCleanliness, std::vector<ColorPalette> palettes,
             std::vector<ShopBlueprint> catalog);

    [[nodiscard]] static Aquarium loadFromCatalogFile(const std::string& catalogPath);
    [[nodiscard]] std::vector<PixelCanvas> loadStarterSketches(const std::string& sketchesPath) const;

    bool unlockItem(const std::string& itemId, const PixelCanvas& sketch);
    bool placeItem(const std::string& itemId, const std::string& paletteId, const std::string& nickname);
    int simulateStep(bool feedFish, bool cleanTank);

    [[nodiscard]] bool canUnlockItem(const std::string& itemId) const;
    [[nodiscard]] std::string renderTankScene() const;
    [[nodiscard]] int coins() const;
    [[nodiscard]] int level() const;
    [[nodiscard]] int xp() const;
    [[nodiscard]] std::size_t inhabitantCount() const;
    [[nodiscard]] const ColorPalette& paletteById(const std::string& paletteId) const;
    [[nodiscard]] const ShopBlueprint& blueprintById(const std::string& itemId) const;
    [[nodiscard]] std::vector<std::string> paletteIds() const;
    [[nodiscard]] std::vector<std::string> catalogItemIds() const;

    friend std::ostream& operator<<(std::ostream& os, const Aquarium& aquarium);

private:
    [[nodiscard]] std::size_t findPaletteIndex(const std::string& paletteId) const;
    [[nodiscard]] std::size_t findBlueprintIndex(const std::string& itemId) const;

    std::string tankName_;
    int width_{38};
    int height_{14};
    int coins_{30};
    int cleanliness_{100};
    int level_{1};
    int xp_{0};
    int tickCount_{0};
    std::vector<ColorPalette> palettes_;
    std::vector<ShopBlueprint> catalog_;
    std::vector<TankEntity> inhabitants_;
};

#endif
