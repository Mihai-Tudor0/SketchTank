#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "Aquarium.h"

namespace {

int readChoice(const std::string& prompt) {
    std::cout << prompt;
    int value = 0;
    if (!(std::cin >> value)) {
        return -1;
    }
    return value;
}

void runShopAndPlace(Aquarium& tank, const std::vector<PixelCanvas>& sketches) {
    const auto itemIds = tank.catalogItemIds();
    std::cout << "\n--- Magazin SketchTank (Monede: " << tank.coins()
              << ", Nivel: " << tank.level() << ", XP: " << tank.xp()
              << ", Creaturi: " << tank.inhabitantCount() << ") ---\n";
    for (std::size_t i = 0; i < itemIds.size(); ++i) {
        std::cout << (i + 1) << ". " << tank.blueprintById(itemIds[i]) << '\n';
    }
    const int itemIdx = readChoice("Alege obiect (1-" + std::to_string(itemIds.size()) + "): ") - 1;
    if (itemIdx < 0 || static_cast<std::size_t>(itemIdx) >= itemIds.size()) {
        std::cout << "Selectie invalida.\n";
        return;
    }

    const std::string& itemId = itemIds[static_cast<std::size_t>(itemIdx)];
    if (!tank.blueprintById(itemId).isUnlocked()) {
        if (!tank.canUnlockItem(itemId)) {
            std::cout << "Monede sau nivel insuficient.\n";
            return;
        }
        if (sketches.empty() || static_cast<std::size_t>(itemIdx) >= sketches.size()) {
            std::cout << "Schita indisponibila.\n";
            return;
        }
        PixelCanvas chosenSketch = sketches[0];
        chosenSketch = sketches[static_cast<std::size_t>(itemIdx)];
        std::cout << "Schita deblocata (copia #" << chosenSketch.copyGeneration() << ").\n";
        tank.unlockItem(itemId, chosenSketch);
    }

    const auto palettes = tank.paletteIds();
    std::cout << "\nAlege paleta de culori:\n";
    for (std::size_t i = 0; i < palettes.size(); ++i) {
        std::cout << (i + 1) << ". " << tank.paletteById(palettes[i]) << '\n';
    }
    const int palIdx = readChoice("Paleta (1-" + std::to_string(palettes.size()) + "): ") - 1;
    if (palIdx < 0 || static_cast<std::size_t>(palIdx) >= palettes.size()) {
        std::cout << "Paleta invalida.\n";
        return;
    }

    std::cout << "Nume creatura/decor (- pentru automat): ";
    std::string nickname;
    std::cin >> nickname;
    if (nickname == "-") {
        nickname.clear();
    }

    const std::string& paletteId = palettes[static_cast<std::size_t>(palIdx)];
    if (tank.placeItem(itemId, paletteId, nickname)) {
        std::cout << "Adaugat in acvariu cu paleta " << tank.paletteById(paletteId).name() << "!\n"
                  << tank.renderTankScene();
    }
}

}  // namespace

int main() {
    try {
        Aquarium tank = Aquarium::loadFromCatalogFile("assets/catalog.json");
        const std::vector<PixelCanvas> sketches = tank.loadStarterSketches("assets/starter_sketches.json");

        std::cout << "=== SketchTank Demo (M1) ===\n" << tank.renderTankScene();
        while (true) {
            std::cout << "\n1. Acvariu  2. Magazin & Plasare  3. Hraneste  4. Curata  5. Pas urmator  6. Stare completa  0. Iesire\n";
            const int option = readChoice("Optiune: ");
            if (option <= 0) {
                break;
            }
            if (option == 1) {
                std::cout << tank.renderTankScene();
            } else if (option == 2) {
                runShopAndPlace(tank, sketches);
            } else if (option == 3) {
                const int earned = tank.simulateStep(true, false);
                std::cout << "Pestii au fost hraniti (+" << earned << " monede).\n" << tank.renderTankScene();
            } else if (option == 4) {
                const int earned = tank.simulateStep(false, true);
                std::cout << "Acvariul a fost curatat (+" << earned << " monede).\n" << tank.renderTankScene();
            } else if (option == 5) {
                const int earned = tank.simulateStep(false, false);
                std::cout << "A trecut 1 pas (+" << earned << " monede).\n" << tank.renderTankScene();
            } else if (option == 6) {
                std::cout << tank;
            }
        }
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Eroare: " << ex.what() << '\n';
        return 1;
    }
}
