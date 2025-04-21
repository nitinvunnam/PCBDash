#include "ComponentSprite.h"

#define tickRate 1

ComponentSprite::ComponentSprite(int8_t lane) {
  if (numCollected >=2) image = (numCollected>=4) ? led : button;
  else image = resistor;
  yellow = false;
  at_top = false;
  respawnFlag = false;
  vertPos = 100;
  tickDivider = 0;
  this->lane = lane;
}

void ComponentSprite::tick() {
  if (lane >= 0) {
    if (tickDivider >= tickRate) {
      vertPos--;
      yellow = !yellow;
      tickDivider = 0;
    } else tickDivider++;

    at_top = (vertPos > 60);
    if (vertPos < -50) lane = -1;
    
  } 
  else {
    tickDivider++;
    if (tickDivider > 100) respawnFlag = true;
  }
}

uint8_t ComponentSprite::numCollected = 0;