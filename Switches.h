#ifndef SWITCHES_H
#define SWITCHES_H
#include "BeamSprite.h"
#include "../inc/LaunchPad.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"

class Switches {
    private:
        uint32_t DInData;
    public:
        bool upPressed;
        bool downPressed;
        bool pausePressed;
        Switches() {upPressed = false; downPressed = false; pausePressed = false;};
        void Init() {
            IOMUX->SECCFG.PINCM[PB12INDEX] = 0x00040081;
            IOMUX->SECCFG.PINCM[PB16INDEX] = 0x00040081;
            IOMUX->SECCFG.PINCM[PB17INDEX] = 0x00040081;
        };
        void Poll() {
            upPressed = (1<<12) & GPIOB->DIN31_0;
            downPressed = (1<<17) & GPIOB->DIN31_0;
            pausePressed = (1<<16) & GPIOB->DIN31_0;
        }
};

#endif