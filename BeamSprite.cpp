#include "BeamSprite.h"



void BeamSprite::init(uint8_t tickRate, int8_t lane){
    active = false;
    depth = 120;
    tickDivider = 0;
    this->lane = lane;
    uint8_t color = rand() % 3;
    this->tickRate = tickRate;
    if (color==0) {
        color0 = 50173;
        color1 = 41719;
    }
    else if (color==1) {
        color0 = 57344;
        color1 = 45056;
    }
    else {
        color0 = 1021;
        color1 = 1045;
    }
}

BeamSprite::BeamSprite(int8_t lane) {init(rand() % 2, lane);}
BeamSprite::BeamSprite(uint8_t tickRate, int8_t lane) {init(tickRate, lane);}

void BeamSprite::tick() {
    if (lane >= 0) {
        active = true;
        if (tickDivider >= tickRate) {
            tickDivider = 0;
            depth--;
        } else tickDivider++;

        if (depth < -120) {
            lane = -1;
            active = false;
        }
    } else {
        tickDivider++;
        if (tickDivider>20) respawnFlag = true;
    }
}
