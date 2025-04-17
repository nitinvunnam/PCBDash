#include "BeamSprite.h"

BeamSprite::BeamSprite(uint8_t lane){
    depth = 120;
    this->lane = lane;
    srand(TIMG12->COUNTERREGS.CTR);
    uint8_t r = rand() % 3;
    if (r==0) {
        beam = beam1;
    }
    else if (r==1) {
        beam = beam2;
    }
    else {
        beam = beam3;
    }
}

void BeamSprite::tick() {
    depth--;
    if (depth <= -15) {
        for (int i = 0; i < 20; i++) {
            backBuff[i] = bgPix(lane * 53 + 15 + i, 105 - (-14 - depth));
        }
    }
}
