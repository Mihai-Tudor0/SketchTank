#include "PixelCanvas.h"

#include <stdexcept>
#include <utility>

PixelCanvas::PixelCanvas(std::string title, std::vector<std::vector<int>> cells, ColorPalette palette)
    : title_(std::move(title)), cells_(std::move(cells)), palette_(std::move(palette)) {
    validateGrid();
}

PixelCanvas::PixelCanvas(const PixelCanvas& other)
    : title_(other.title_),
      cells_(other.cells_),
      palette_(other.palette_),
      copyGeneration_(other.copyGeneration_ + 1) {}

PixelCanvas& PixelCanvas::operator=(const PixelCanvas& other) {
    if (this == &other) {
        return *this;
    }
    title_ = other.title_;
    cells_ = other.cells_;
    palette_ = other.palette_;
    copyGeneration_ = other.copyGeneration_ + 1;
    return *this;
}

PixelCanvas::~PixelCanvas() = default;

void PixelCanvas::recolor(const ColorPalette& newPalette) {
    for (auto& row : cells_) {
        for (int& cell : row) {
            if (cell > newPalette.colorCount()) {
                cell = ((cell - 1) % newPalette.colorCount()) + 1;
            }
        }
    }
    palette_ = newPalette;
}

int PixelCanvas::width() const { return cells_.empty() ? 0 : static_cast<int>(cells_[0].size()); }
int PixelCanvas::height() const { return static_cast<int>(cells_.size()); }
int PixelCanvas::copyGeneration() const { return copyGeneration_; }
const ColorPalette& PixelCanvas::palette() const { return palette_; }

int PixelCanvas::pixelAt(const int row, const int col) const {
    if (row < 0 || row >= height() || col < 0 || col >= width()) {
        throw std::out_of_range("Pixel coordinates out of range.");
    }
    return cells_[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)];
}

int PixelCanvas::filledPixelCount() const {
    int count = 0;
    for (const auto& row : cells_) {
        for (const int cell : row) {
            if (cell != transparentColor) {
                ++count;
            }
        }
    }
    return count;
}

std::string PixelCanvas::renderAnsi() const {
    std::string out;
    for (const auto& row : cells_) {
        for (const int cell : row) {
            out += palette_.ansiBlock(cell);
        }
        out += '\n';
    }
    return out;
}

void PixelCanvas::validateGrid() const {
    if (title_.empty() || cells_.empty() || cells_[0].empty()) {
        throw std::invalid_argument("PixelCanvas requires non-empty title and grid.");
    }
    const std::size_t cols = cells_[0].size();
    for (const auto& row : cells_) {
        if (row.size() != cols) {
            throw std::invalid_argument("PixelCanvas rows must have uniform width.");
        }
        for (const int cell : row) {
            if (cell < transparentColor || cell > palette_.colorCount()) {
                throw std::invalid_argument("PixelCanvas color index out of palette range.");
            }
        }
    }
}

std::ostream& operator<<(std::ostream& os, const PixelCanvas& canvas) {
    os << canvas.title_ << " [" << canvas.width() << "x" << canvas.height()
       << ", gen=" << canvas.copyGeneration() << "] | " << canvas.palette_ << '\n'
       << canvas.renderAnsi();
    return os;
}
