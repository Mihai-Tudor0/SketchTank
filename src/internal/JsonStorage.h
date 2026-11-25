#ifndef SKETCHTANK_JSON_STORAGE_H
#define SKETCHTANK_JSON_STORAGE_H

#include <string>
#include <vector>

#include "Aquarium.h"
#include "PixelCanvas.h"

namespace internal::JsonStorage {

[[nodiscard]] Aquarium loadCatalog(const std::string& catalogPath);
[[nodiscard]] std::vector<PixelCanvas> loadSketches(const std::string& sketchesPath, const Aquarium& aquarium);

}  // namespace internal::JsonStorage

#endif
