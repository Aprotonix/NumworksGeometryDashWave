
#include "string.hpp"

void renderDigit(uint16_t x, uint16_t y, uint16_t digit, EADK::Color color,uint16_t factor ) {

    switch (digit)
    {
        case 0: {

            EADK::Display::pushRectUniform(EADK::Rect(x, y, factor, 5*factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x+factor, y, factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x+2*factor, y, factor, 5*factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x+factor, y+4*factor, factor, factor), color);
            break;
        }
        case 1: {
            EADK::Display::pushRectUniform(EADK::Rect(x + factor, y, factor, 5*factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y+factor, factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y+4*factor, factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x+2*factor, y+4*factor, factor, factor), color);
            break;
        }
        case 2: {
            EADK::Display::pushRectUniform(EADK::Rect(x, y, 3*factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x+2*factor, y+factor, factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y+2*factor, 3*factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y+3*factor, factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y+4*factor, 3*factor, factor), color);
            break;
        }
        case 3: {
            EADK::Display::pushRectUniform(EADK::Rect(x, y, 3*factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x+2*factor, y+factor, factor, 3*factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y+2*factor, 3*factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y+4*factor, 3*factor, factor), color);
            break;
        }
        case 4: {
            EADK::Display::pushRectUniform(EADK::Rect(x, y, factor, 2*factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x+2*factor, y, factor, 5*factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y+2*factor, 3*factor, factor), color);
            break;
        }
        case 5: {
            EADK::Display::pushRectUniform(EADK::Rect(x, y, 3*factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y+factor, factor, 2*factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y+2*factor, 3*factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x+2*factor, y+3*factor, factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y+4*factor, 3*factor, factor), color);
            break;
        }
        case 6: {
            EADK::Display::pushRectUniform(EADK::Rect(x+2*factor, y, factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y+factor, factor, 3*factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y, 3*factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y+2*factor, 3*factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y+4*factor, 3*factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x+2*factor, y+3*factor, factor, factor), color);
            break;
        }
        case 7: {
            EADK::Display::pushRectUniform(EADK::Rect(x, y, 3*factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x+2*factor, y+factor, factor, 4*factor), color);
            break;
        }
        case 8: {
            EADK::Display::pushRectUniform(EADK::Rect(x, y, factor, 5*factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x+factor*2, y, factor, 5*factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x+factor, y, factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x+factor, y+2*factor, factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x+factor, y+4*factor, factor, factor), color);
            break;
        }
        case 9: {
            EADK::Display::pushRectUniform(EADK::Rect(x, y, 3*factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y+2*factor, 3*factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y+1*factor, factor, factor), color);
            EADK::Display::pushRectUniform(EADK::Rect(x+2*factor, y+1*factor, factor, factor*3), color);
            EADK::Display::pushRectUniform(EADK::Rect(x, y+4*factor, 3*factor, factor), color);
            break;
        }
        default:
            break;
    }
}


void renderNumber(uint16_t x, uint16_t y, uint16_t number, EADK::Color color, uint16_t factor) {
    if (number == 0) {
        // Cas spécial pour 0
        renderDigit(x, y, 0, color, factor);
        return;
    }

    // Compter le nombre de chiffres
    uint16_t temp = number;
    int digitsCount = 0;
    while (temp > 0) {
        temp /= 10;
        digitsCount++;
    }

    // Créer un tableau pour stocker les chiffres (de droite à gauche)
    uint8_t digits[10]; // max 10 chiffres pour un uint32_t
    temp = number;
    for (int i = digitsCount - 1; i >= 0; --i) {
        digits[i] = temp % 10;
        temp /= 10;
    }

    // Afficher chaque chiffre
    int spacing = 4* factor; // espace entre les chiffres (ajustable)
    for (int i = 0; i < digitsCount; ++i) {
        renderDigit(x + i * spacing, y, digits[i], color, factor);
    }
}