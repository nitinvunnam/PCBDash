#include "BeamSprite.h"
extern bool isSpanish;
extern uint16_t buffer[5565];
extern uint8_t selected;
#define PCBGreen 0x4D0B

typedef enum {
  homeState,     // initial screen with “Language” & “Start”
  languageMenuState,// the language‑toggle confirmation page
  gameState,      // the actual game screen
  gameOver
} AppState;


typedef enum { RECT, CIRCLE } Shape_t;

typedef struct {
  Shape_t shape;
  int16_t x, y;   
  int16_t w, h;
} Element;


const Element elements[6] = {
  { RECT,   24,  23, 25, 15 },  // resistor1
  { RECT,   24,  31, 25, 15 },  // resistor2
  { RECT,   45,  81, 7, 13 },  // button1
  { RECT,   45,  87, 7, 13 },   // button2
  { CIRCLE, 122,  26,  0,  0 },  // LED hole2
  { CIRCLE, 115,  26,  0,  0 }  // LED hole1
};

void drawHomeScreen(void);
void drawGameOver(void);
void outlineButton(int16_t x, int16_t y, int16_t w, int16_t h);
void drawMainButtons(void);
void drawSpanishButtons(void);
void PcbOutline(uint8_t index);
void PCBScreen(void);