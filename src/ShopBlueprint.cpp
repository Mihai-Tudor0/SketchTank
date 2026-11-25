#include "ShopBlueprint.h"

#include <stdexcept>
#include <utility>

std::string categoryToString(const ItemCategory category) {
    switch (category) {
        case ItemCategory::Fish:
            return "Peste";
        case ItemCategory::Decoration:
            return "Decor";
        case ItemCategory::Cleaner:
            return "Curatator";
    }
    return "Peste";
}

ShopBlueprint::ShopBlueprint(std::string itemId,
                             std::string displayName,
                             const ItemCategory category,
                             const int price,
                             const int passiveIncome,
                             const int requiredLevel,
                             const bool unlocked)
    : itemId_(std::move(itemId)),
      displayName_(std::move(displayName)),
      category_(category),
      price_(price),
      passiveIncome_(passiveIncome),
      requiredLevel_(requiredLevel),
      unlocked_(unlocked) {
    if (itemId_.empty() || displayName_.empty() || price_ < 0 || requiredLevel_ < 1) {
        throw std::invalid_argument("Invalid ShopBlueprint parameters.");
    }
}

bool ShopBlueprint::unlockWithSketch(const PixelCanvas& sketch) {
    if (sketch.filledPixelCount() <= 0) {
        return false;
    }
    baseSketch_ = sketch;
    unlocked_ = true;
    return true;
}

PixelCanvas ShopBlueprint::makeColoredSprite(const ColorPalette& palette) const {
    if (!unlocked_ || !baseSketch_.has_value()) {
        throw std::logic_error("ShopBlueprint must be unlocked before creating a sprite.");
    }
    PixelCanvas sprite = *baseSketch_;
    sprite.recolor(palette);
    return sprite;
}

const std::string& ShopBlueprint::id() const { return itemId_; }
const std::string& ShopBlueprint::name() const { return displayName_; }
ItemCategory ShopBlueprint::category() const { return category_; }
int ShopBlueprint::price() const { return price_; }
int ShopBlueprint::passiveIncome() const { return passiveIncome_; }
int ShopBlueprint::requiredLevel() const { return requiredLevel_; }
bool ShopBlueprint::isUnlocked() const { return unlocked_; }

std::ostream& operator<<(std::ostream& os, const ShopBlueprint& item) {
    os << item.displayName_ << " [" << categoryToString(item.category_) << "] - "
       << item.price_ << " monede, +" << item.passiveIncome_ << "/pas, lvl " << item.requiredLevel_
       << (item.unlocked_ ? " [DEBLOCAT]" : " [BLOCAT]");
    if (item.baseSketch_.has_value()) {
        os << '\n' << *item.baseSketch_;
    }
    return os;
}
