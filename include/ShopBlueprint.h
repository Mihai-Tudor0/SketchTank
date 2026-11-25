#ifndef SKETCHTANK_SHOP_BLUEPRINT_H
#define SKETCHTANK_SHOP_BLUEPRINT_H

#include <optional>
#include <ostream>
#include <string>

#include "ColorPalette.h"
#include "PixelCanvas.h"

enum class ItemCategory { Fish, Decoration, Cleaner };

[[nodiscard]] std::string categoryToString(ItemCategory category);

class ShopBlueprint {
public:
    ShopBlueprint(std::string itemId,
                  std::string displayName,
                  ItemCategory category,
                  int price,
                  int passiveIncome,
                  int requiredLevel = 1,
                  bool unlocked = false);

    bool unlockWithSketch(const PixelCanvas& sketch);
    [[nodiscard]] PixelCanvas makeColoredSprite(const ColorPalette& palette) const;

    [[nodiscard]] const std::string& id() const;
    [[nodiscard]] const std::string& name() const;
    [[nodiscard]] ItemCategory category() const;
    [[nodiscard]] int price() const;
    [[nodiscard]] int passiveIncome() const;
    [[nodiscard]] int requiredLevel() const;
    [[nodiscard]] bool isUnlocked() const;

    friend std::ostream& operator<<(std::ostream& os, const ShopBlueprint& item);

private:
    std::string itemId_;
    std::string displayName_;
    ItemCategory category_{ItemCategory::Fish};
    int price_{0};
    int passiveIncome_{0};
    int requiredLevel_{1};
    bool unlocked_{false};
    std::optional<PixelCanvas> baseSketch_;
};

#endif
