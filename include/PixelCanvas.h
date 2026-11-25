#ifndef SKETCHTANK_PIXEL_CANVAS_H
#define SKETCHTANK_PIXEL_CANVAS_H

#include <ostream>
#include <string>
#include <vector>

#include "ColorPalette.h"

class PixelCanvas {
public:
    static constexpr int transparentColor = 0;

    PixelCanvas(std::string title, std::vector<std::vector<int>> cells, ColorPalette palette);
    PixelCanvas(const PixelCanvas& other);
    PixelCanvas& operator=(const PixelCanvas& other);
    ~PixelCanvas();

    void recolor(const ColorPalette& newPalette);

    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] int copyGeneration() const;
    [[nodiscard]] const ColorPalette& palette() const;
    [[nodiscard]] int pixelAt(int row, int col) const;
    [[nodiscard]] int filledPixelCount() const;
    [[nodiscard]] std::string renderAnsi() const;

    friend std::ostream& operator<<(std::ostream& os, const PixelCanvas& canvas);

private:
    void validateGrid() const;

    std::string title_;
    std::vector<std::vector<int>> cells_;
    ColorPalette palette_;
    int copyGeneration_{0};
};

#endif
