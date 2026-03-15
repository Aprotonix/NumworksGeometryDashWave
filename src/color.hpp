#include <cstdint>

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    // Constructeurs
    Color() : r(0), g(0), b(0) {}                  // couleur noire par défaut
    Color(uint8_t r_val, uint8_t g_val, uint8_t b_val) : r(r_val), g(g_val), b(b_val) {}
    Color(uint32_t hex) : r((hex >> 16) & 0xFF), g((hex >> 8) & 0xFF), b(hex & 0xFF) {}

    // Retourne le code hexadécimal 0xRRGGBB
    uint32_t hex() const {
        return (uint32_t(r) << 16) | (uint32_t(g) << 8) | uint32_t(b);
    }
};
