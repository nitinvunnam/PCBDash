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
        uint8_t health;
        uint8_t timeInMidHealth;
        bool in_beam;
        bool otherTrip;
    public:
        bool flashOff;
        bool in_beam_flag;
        bool tripFlag;
        const uint16_t * image;
        uint8_t vertPos;
        bool dead;
        bool win;
        DashSprite();
        void initHealth();
        void tick();
        void changeLane(uint8_t lane);
        int8_t prevLane();
        uint8_t getLane();
        void acknowledgeLaneChange();
        void jump();
        void inBeam();
        void trip();
        void healthOff();
};

#endif