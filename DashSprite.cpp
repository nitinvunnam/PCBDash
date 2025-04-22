#include "DashSprite.h"
#include "Bitmaps.h"
#include "Switches.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#define fullHealth (1<<26)
#define midHealth (1<<27)
#define zeroHealth (1<<24)

DashSprite::DashSprite() {
    state = 0;
    timeInState = 0;
    lane = 0;
    image = DashRunLeftBMP;
    vertPos = 0;
    dead = false;
    win = false;
    timeInMidHealth = 0;
    flashOff = false;
    in_beam = false;
    in_beam_flag = false;
    tripFlag = false;
    otherTrip = false;
}

void DashSprite::initHealth() {
    health = 2;
    IOMUX->SECCFG.PINCM[PA26INDEX] = 0x00000081;
    IOMUX->SECCFG.PINCM[PA27INDEX] = 0x00000081;
    IOMUX->SECCFG.PINCM[PB24INDEX] = 0x00000081;
    GPIOA->DOE31_0 |= (fullHealth + midHealth);
    GPIOB->DOE31_0 |= zeroHealth;
}

void DashSprite::healthOff() {
    GPIOA->DOUTCLR31_0 |= (fullHealth + midHealth);
    GPIOB->DOUTCLR31_0 |= zeroHealth;
}

void DashSprite::tick() {
    flashOff = false;
    if (state < 2) {
        if (timeInState == 10) {
            state = state ? 0 : 1;
            timeInState = 0;
        }
        else timeInState++;
        if (state && (health==2 || (timeInMidHealth/8)%2)) image = DashRunRightBMP;
        else if (!state && (health==2 || (timeInMidHealth/8)%2)) image = DashRunLeftBMP;
        else flashOff = true;
    }
    else if (state == 2) {
        if (timeInState > 30) {
            state = 0;
            timeInState = 0;
            image = DashRunLeftBMP;
        }
        else {
            vertPos = 30 - (((timeInState - 15)*(timeInState - 15))>>3);
            timeInState++;
        }
    }

    if (health == 1 && timeInMidHealth > 240) health--;
    else if (health == 1 && timeInMidHealth == 0) health++;

    if (!in_beam && timeInMidHealth>0) {
        in_beam_flag = false;
        timeInMidHealth--;
    }
    in_beam = false;

    if (!otherTrip) tripFlag = false;
    otherTrip = false;

    if (!health) dead = true;

    if (health == 2) {
        GPIOA->DOUTCLR31_0 |= midHealth;
        GPIOB->DOUTCLR31_0 |= zeroHealth;
        GPIOA->DOUTSET31_0 |= fullHealth;
    } else if (health == 1) {
        GPIOA->DOUTSET31_0 |= midHealth;
        GPIOB->DOUTCLR31_0 |= zeroHealth;
        GPIOA->DOUTCLR31_0 |= fullHealth;
    } else {
        GPIOA->DOUTCLR31_0 |= (fullHealth + midHealth);
        GPIOB->DOUTSET31_0 |= zeroHealth;
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

void DashSprite::jump() {
    if (state != 2) {
        state = 2;
        timeInState = 0;
        image = DashJumpBMP;
    }
}

void DashSprite::inBeam() {
    if (health == 2) {
        health--;
        timeInMidHealth += 200;
    }
    else if (health == 1 && in_beam_flag) timeInMidHealth++;
    else if (health == 1) health--;
    in_beam_flag = true;
    in_beam = true;
}

void DashSprite::trip() {
    if (!tripFlag) {
        health--;
        timeInMidHealth += 150;
    }
    tripFlag = true;
    otherTrip = true;
}