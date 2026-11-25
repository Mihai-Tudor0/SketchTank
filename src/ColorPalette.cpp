#include "ColorPalette.h"

#include <cctype>
#include <stdexcept>
#include <utility>

ColorPalette::ColorPalette(std::string id, std::string name, std::vector<std::string> hexColors)
    : id_(std::move(id)), name_(std::move(name)), hexColors_(std::move(hexColors)) {
    if (id_.empty() || name_.empty() || hexColors_.empty()) {
        throw std::invalid_argument("ColorPalette requires non-empty id, name, and colors.");
    }
    for (const std::string& hex : hexColors_) {
        if (!isValidHex(hex)) {
            throw std::invalid_argument("Invalid hex color in ColorPalette: " + hex);
        }
    }
}

const std::string& ColorPalette::id() const { return id_; }
const std::string& ColorPalette::name() const { return name_; }
int ColorPalette::colorCount() const { return static_cast<int>(hexColors_.size()); }

std::string ColorPalette::ansiBlock(const int colorIndex) const {
    if (colorIndex <= 0 || colorIndex > colorCount()) {
        return "  ";
    }
    const std::string& hex = hexColors_[static_cast<std::size_t>(colorIndex - 1)];
    const int r = std::stoi(hex.substr(1, 2), nullptr, 16);
    const int g = std::stoi(hex.substr(3, 2), nullptr, 16);
    const int b = std::stoi(hex.substr(5, 2), nullptr, 16);
    return "\x1b[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m\xe2\x96\x88\xe2\x96\x88\x1b[0m";
}

std::string ColorPalette::formatLegend() const {
    std::string out;
    for (int i = 1; i <= colorCount(); ++i) {
        out += ansiBlock(i);
    }
    return out;
}

bool ColorPalette::isValidHex(const std::string& hex) {
    if (hex.size() != 7 || hex[0] != '#') {
        return false;
    }
    for (std::size_t i = 1; i < hex.size(); ++i) {
        if (std::isxdigit(static_cast<unsigned char>(hex[i])) == 0) {
            return false;
        }
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const ColorPalette& palette) {
    os << palette.name_ << " (" << palette.id_ << ") " << palette.formatLegend();
    return os;
}
