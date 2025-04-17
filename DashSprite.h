#ifndef DASHSPRITE_H
#define DASHSPRITE_H
#include "BeamSprite.h"

class DashSprite {
    private:
        uint8_t state;
        uint8_t timeInState;
        bool checkPrevLane;
        uint8_t lane;
        uint8_t prev_lane;
    public:
        const uint16_t * image;
        uint8_t vertPos;
        DashSprite(uint8_t lane);
        void tick();
        void changeLane(uint8_t lane);
        int8_t prevLane();
        uint8_t getLane();
        void acknowledgeLaneChange();
};

#endif