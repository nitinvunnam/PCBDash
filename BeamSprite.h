#ifndef BEAMSPRITE_H
#define BEAMSPRITE_H
#include <stdlib.h>
#include <stdint.h>
#include "Bitmaps.h"
#include "../inc/Timer.h"
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#define MAX(a,b) ((a) > (b) ? (a) : (b))

class BeamSprite {
    private:
        uint8_t tickDivider;
        int16_t depth; // this goes from -120 to 120
        void init(uint8_t tickRate, int8_t lane);
        uint8_t tickRate;
    public:
        int8_t lane;
        bool active;
        bool respawnFlag;
        uint16_t color0;
        uint16_t color1;
        BeamSprite(int8_t lane);
        BeamSprite(uint8_t tickRate, int8_t lane);
        void tick();
        uint16_t getBeamLeftSide(){return lane * 53;};
        int16_t getBottomEdge(){return depth - 20;};
        uint16_t getBeamLen(){return 140-MAX(0, depth);};
};

#endif

