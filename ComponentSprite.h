#include "BeamSprite.h"

class ComponentSprite {
  private:
    uint8_t tickDivider;
    uint8_t flashDivider;
  public:
    int8_t lane;
    bool respawnFlag;
    bool at_top;
    static uint8_t numCollected;
    bool yellow;
    const uint16_t * image;
    int8_t vertPos;
    ComponentSprite(int8_t lane);
    void tick();
};