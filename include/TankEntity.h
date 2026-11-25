#ifndef SKETCHTANK_TANK_ENTITY_H
#define SKETCHTANK_TANK_ENTITY_H

#include <ostream>
#include <string>

#include "PixelCanvas.h"
#include "ShopBlueprint.h"

class TankEntity {
public:
    TankEntity(std::string nickname, ItemCategory category, int passiveIncome,
               const PixelCanvas& sprite, int posX, int posY, int tankWidth, int tankHeight);

    int advanceStep(int tankWidth, int tankHeight, bool fed, int tankCleanliness);

    [[nodiscard]] int posX() const;
    [[nodiscard]] int posY() const;
    [[nodiscard]] int spriteWidth() const;
    [[nodiscard]] int spriteHeight() const;
    [[nodiscard]] std::string renderedAnsiAt(int row, int col) const;
    [[nodiscard]] int autoCleanPower() const;

    friend std::ostream& operator<<(std::ostream& os, const TankEntity& entity);

private:
    void clampToTank(int tankWidth, int tankHeight);

    std::string nickname_;
    ItemCategory category_{ItemCategory::Fish};
    int passiveIncome_{0};
    PixelCanvas sprite_;
    int posX_{0};
    int posY_{0};
    int velocityX_{1};
    int velocityY_{1};
    bool facingRight_{true};
    int hunger_{0};
};

#endif
