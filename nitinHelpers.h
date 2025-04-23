#include "BeamSprite.h"

extern bool componentPlaced[6];
extern bool isSpanish;
extern uint16_t buffer[5565];
extern uint8_t selected;
extern uint16_t gameCounter;
#define PCBGreen 0x4D0B

typedef enum {
  homeState,     // initial screen with “Language” & “Start”
  languageMenuState,// the language‑toggle confirmation page
  gameState,      // the actual game screen
  gameOver,
  infoPlace,
  placeState,
  winState
} AppState;


typedef enum { RECT, CIRCLE } Shape_t;

typedef struct {
  Shape_t shape;
  int16_t x, y;   
  int16_t w, h;
} Element;


const Element elements[6] = {
  { RECT,   40,  23, 25, 15 },  // resistor1
  { RECT,   40,  31, 25, 15 },  // resistor2
  { RECT,   61,  81, 7, 13 },  // button1
  { RECT,   61,  87, 7, 13 },   // button2
  { CIRCLE, 138,  26,  0,  0 },  // LED hole2
  { CIRCLE, 131,  26,  0,  0 }  // LED hole1
};


void drawHomeScreen(void);
void drawGameOver(void);
void outlineButton(int16_t x, int16_t y, int16_t w, int16_t h);
void drawMainButtons(void);
void drawSpanishButtons(void);
void PcbOutline(uint8_t index);
void PCBScreen(void);
void drawComponent(uint8_t index);
void drawEndScreen(void);