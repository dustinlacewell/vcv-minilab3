#include "utils/padNotes.hpp"

int noteForPad(int padId) {
    switch (padId) {
        // C1
        case 1:
            return 36;
            // C2
        case 2:
            return 48;
            // C3
        case 3:
            return 60;
            // C4
        case 4:
            return 72;
            // C5
        case 5:
            return 84;
            // C6
        case 6:
            return 96;
            // C7
        case 7:
            return 108;
            // C8
        case 8:
            return 120;
            // D1
        case 9:
            return 38;
            // D2
        case 10:
            return 50;
            // D3
        case 11:
            return 62;
            // D4
        case 12:
            return 74;
            // D5
        case 13:
            return 86;
            // D6
        case 14:
            return 98;
            // D7
        case 15:
            return 110;
            // D8
        case 16:
            return 122;
        default:
            return -1;
    }
}
