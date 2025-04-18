#include "BeamSprite.h"

#define tickRate 0

void BeamSprite::init(){
    depth = 120;
    tickDivider = 0;
    this->lane = rand() % 3;
    uint8_t r = rand() % 3;
    if (r==0) {
        beam = beamcolor1;
    }
    else if (r==1) {
        beam = beamcolor2;
    }
    else {
        beam = beamcolor3;
    }
}

BeamSprite::BeamSprite() {init();}

void BeamSprite::tick() {
    if (lane >= 0) {
        if (tickDivider >= tickRate) {
            tickDivider = 0;
            depth--;
            if (depth <= -15) {
                for (int i = 0; i < 20; i++) {
                    backBuff[i] = bgPix(lane * 53 + 15 + i, 105 - (-14 - depth));
                }
            }
        } else tickDivider++;

        if (depth < -120) lane = -1;
    } else {
        tickDivider++;
        if (tickDivider>10) init();
    }
}
