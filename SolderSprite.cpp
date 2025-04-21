#include "SolderSprite.h"

#define tickRate 1

SolderSprite::SolderSprite(int8_t lane) {
  vertPos = 100;
  this->lane = lane;
  tickDivider = 0;
  active = false;
  at_top = false;
  respawnFlag = false;
}

void SolderSprite::tick() {
  if (lane >= 0) {
    active = true;
    if (tickDivider >= tickRate) {
      vertPos--;
      tickDivider = 0;
    } else tickDivider++;

    if (vertPos < -50) {
      active = false;
      lane = -1;
    }
    at_top = (vertPos > 50);
  } else {
    tickDivider++;
    if (tickDivider > 50) respawnFlag = true;
  }
}