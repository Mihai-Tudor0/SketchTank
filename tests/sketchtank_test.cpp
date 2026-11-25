#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

#include "Aquarium.h"
#include "ColorPalette.h"
#include "PixelCanvas.h"
#include "ShopBlueprint.h"
#include "TankEntity.h"

int main() {
    Aquarium tank = Aquarium::loadFromCatalogFile("assets/catalog.json");
    const std::vector<PixelCanvas> sketches = tank.loadStarterSketches("assets/starter_sketches.json");
    assert(sketches.size() == 4);
    assert(tank.coins() == 30);
    assert(tank.level() == 1);

    // Rule of Three verification on PixelCanvas
    PixelCanvas copy1(sketches[0]);
    copy1.recolor(tank.paletteById("coral_sunset"));
    assert(copy1.copyGeneration() == sketches[0].copyGeneration() + 1);
    PixelCanvas copy2 = sketches[1];
    copy2 = copy1;
    assert(copy2.copyGeneration() == copy1.copyGeneration() + 1);
    assert(copy2.pixelAt(1, 3) == sketches[0].pixelAt(1, 3));

    // Unlock and place fish + decoration
    assert(tank.canUnlockItem("common_fish"));
    const bool unlockedFish = tank.unlockItem("common_fish", sketches[0]);
    assert(unlockedFish);
    const bool placedFish = tank.placeItem("common_fish", "golden_reef", "Nemo");
    assert(placedFish);
    assert(tank.inhabitantCount() == 1);

    const bool unlockedAlgae = tank.unlockItem("seagrass", sketches[1]);
    assert(unlockedAlgae);
    const bool placedAlgae = tank.placeItem("seagrass", "emerald_kelp", "");
    assert(placedAlgae);
    assert(tank.inhabitantCount() == 2);

    // Simulate steps and verify passive income & XP
    const int earned = tank.simulateStep(true, false);
    assert(earned == 6);
    assert(tank.coins() == 11);
    assert(tank.xp() > 0);

    // Stream insertion on Aquarium
    std::ostringstream oss;
    oss << tank;
    assert(oss.str().find("Nemo") != std::string::npos);

    std::cout << "All SketchTank tests passed!\n";
    return 0;
}
