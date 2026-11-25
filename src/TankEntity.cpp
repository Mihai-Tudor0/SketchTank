#include "TankEntity.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

TankEntity::TankEntity(std::string nickname,
                       const ItemCategory category,
                       const int passiveIncome,
                       const PixelCanvas& sprite,
                       const int posX,
                       const int posY,
                       const int tankWidth,
                       const int tankHeight)
    : nickname_(std::move(nickname)),
      category_(category),
      passiveIncome_(passiveIncome),
      sprite_(sprite),
      posX_(posX),
      posY_(posY) {
    if (nickname_.empty()) {
        throw std::invalid_argument("TankEntity nickname cannot be empty.");
    }
    clampToTank(tankWidth, tankHeight);
}

int TankEntity::advanceStep(const int tankWidth, const int tankHeight, const bool fed, const int tankCleanliness) {
    if (category_ == ItemCategory::Decoration) {
        posY_ = std::max(0, tankHeight - spriteHeight());
        return 0;
    }

    const int maxX = std::max(0, tankWidth - spriteWidth());
    posX_ += velocityX_;
    if (posX_ >= maxX) {
        posX_ = maxX;
        velocityX_ = -1;
        facingRight_ = false;
    } else if (posX_ <= 0) {
        posX_ = 0;
        velocityX_ = 1;
        facingRight_ = true;
    }

    if (category_ == ItemCategory::Cleaner) {
        posY_ = std::max(0, tankHeight - spriteHeight());
    } else {
        const int maxY = std::max(0, tankHeight - spriteHeight());
        posY_ += velocityY_;
        if (posY_ >= maxY || posY_ <= 0) {
            posY_ = std::clamp(posY_, 0, maxY);
            velocityY_ = -velocityY_;
        }
    }

    hunger_ = fed ? std::max(0, hunger_ - 2) : std::min(5, hunger_ + 1);
    if (hunger_ >= 4 || tankCleanliness < 35) {
        return 0;
    }
    return passiveIncome_;
}

int TankEntity::posX() const { return posX_; }
int TankEntity::posY() const { return posY_; }
int TankEntity::spriteWidth() const { return sprite_.width(); }
int TankEntity::spriteHeight() const { return sprite_.height(); }
int TankEntity::autoCleanPower() const { return category_ == ItemCategory::Cleaner ? 8 : 0; }

std::string TankEntity::renderedAnsiAt(const int row, const int col) const {
    const int mappedCol = facingRight_ ? col : (spriteWidth() - 1 - col);
    return sprite_.palette().ansiBlock(sprite_.pixelAt(row, mappedCol));
}

void TankEntity::clampToTank(const int tankWidth, const int tankHeight) {
    posX_ = std::clamp(posX_, 0, std::max(0, tankWidth - spriteWidth()));
    posY_ = std::clamp(posY_, 0, std::max(0, tankHeight - spriteHeight()));
}

std::ostream& operator<<(std::ostream& os, const TankEntity& entity) {
    os << entity.nickname_ << " (" << categoryToString(entity.category_) << ") la ("
       << entity.posX_ << ", " << entity.posY_ << "), foame=" << entity.hunger_
       << "/5\n" << entity.sprite_;
    return os;
}
