#include "BeamSprite.h"


class SolderSprite {
  private:
    uint8_t tickDivider;
  public:
    bool active;
    bool respawnFlag;
    bool at_top;
    int8_t lane;
    int8_t vertPos;
    SolderSprite(int8_t lane);
    void tick();
};