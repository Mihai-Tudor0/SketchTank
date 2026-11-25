#ifndef SKETCHTANK_COLOR_PALETTE_H
#define SKETCHTANK_COLOR_PALETTE_H

#include <ostream>
#include <string>
#include <vector>

class ColorPalette {
public:
    ColorPalette(std::string id, std::string name, std::vector<std::string> hexColors);

    [[nodiscard]] const std::string& id() const;
    [[nodiscard]] const std::string& name() const;
    [[nodiscard]] int colorCount() const;
    [[nodiscard]] std::string ansiBlock(int colorIndex) const;
    [[nodiscard]] std::string formatLegend() const;

    friend std::ostream& operator<<(std::ostream& os, const ColorPalette& palette);

private:
    [[nodiscard]] static bool isValidHex(const std::string& hex);

    std::string id_;
    std::string name_;
    std::vector<std::string> hexColors_;
};

#endif
