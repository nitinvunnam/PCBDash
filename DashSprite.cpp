#include "DashSprite.h"
#include "Bitmaps.h"


DashSprite::DashSprite(uint8_t lane) {
    state = 0;
    timeInState = 0;
    this->lane = lane;
    image = DashRunLeftBMP;
    vertPos = 0;
}

void DashSprite::tick() {
    if (state < 2) {
        if (timeInState == 10) {
            state = state ? 0 : 1;
            if (state) image = DashRunRightBMP;
            else image = DashRunLeftBMP;
            timeInState = 0;
        } else timeInState++;
    }
}

void DashSprite::changeLane(uint8_t lane) {
    if (lane != this->lane) {
        checkPrevLane = true;
        prev_lane = this->lane;
        this->lane = lane;
    }
}

int8_t DashSprite::prevLane() {
    if (!checkPrevLane) return -1;
    return prev_lane;
}

uint8_t DashSprite::getLane() {
    return lane;
}

void DashSprite::acknowledgeLaneChange() {
    checkPrevLane = false;
}