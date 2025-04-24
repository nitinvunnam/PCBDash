// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Your name
// Last Modified: 12/26/2024

#include <codecvt>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include "../inc/Clock.h"
#include "../inc/TExaS.h"
#include "../inc/SlidePot.h"
// #include "../inc/DAC5.h"
#include "BeamSprite.h"
#include "Bitmaps.h"
#include "Sound.h"
#include "DashSprite.h"
#include "Switches.h"
#include "SolderSprite.h"
#include "ComponentSprite.h"
#include "nitinHelpers.h"
#include "Sounds.h"

#define LANEBUFFWIDTH 53
#define LANEBUFFHEIGHT 105

extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);
// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}

SlidePot Sensor(1640,190); // copy calibration from Lab 7

bool componentPlaced[6] = {false, false, false, false, false, false};
uint16_t buffer[5565];
bool isSpanish = false;
uint8_t selected = 0;
bool initText;

DashSprite dash;
BeamSprite beam1(0);
BeamSprite beam2(-1);
SolderSprite solderpoop(-1);
ComponentSprite component(-1);
Switches buttons;
uint32_t spData;
bool stopped, lock;
bool gameEntered = false;

uint8_t getDiffLane(uint8_t avoidLane) {
  uint8_t r = rand() % 2;
  if (avoidLane == 0) {
    if (!r) return 1;
    else return 2;
  } else if (avoidLane == 1) {
    if (!r) return 0;
    else return 2;
  } else {
    if (!r) return 0;
    else return 1;
  }
}

// games  engine runs at 30Hz
void TIMG12_IRQHandler(void){
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    if(gameEntered) {
      GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
      GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
      dash.tick();
      beam1.tick();
      component.tick();
      if (ComponentSprite::numCollected >= 2) solderpoop.tick();
      if (ComponentSprite::numCollected >= 4) beam2.tick();
      if (beam1.respawnFlag) {
        if (!beam2.active && !solderpoop.active) beam1 = BeamSprite(0,rand()%3);
        else if (beam2.active && !solderpoop.active) beam1 = BeamSprite(rand()%2, getDiffLane(beam2.lane));
        else if (!beam2.active && solderpoop.active) beam1 = BeamSprite(0, getDiffLane(solderpoop.lane));
        else beam1 = BeamSprite(rand()%2, 3-beam2.lane-solderpoop.lane);
      } else if (beam2.respawnFlag) {
        if (!beam1.active && !solderpoop.active) beam2 = BeamSprite(0,rand()%3);
        else if (beam1.active && !solderpoop.active) beam2 = BeamSprite(rand()%2, getDiffLane(beam1.lane));
        else if (!beam1.active && solderpoop.active) beam2 = BeamSprite(0, getDiffLane(solderpoop.lane));
        else beam2 = BeamSprite(rand()%2, 3-beam1.lane-solderpoop.lane);
      } else if (solderpoop.respawnFlag && !component.at_top) {
        if (!beam1.active && !beam2.active) solderpoop = SolderSprite(rand()%3);
        else if (beam1.active && !beam2.active) solderpoop = SolderSprite(getDiffLane(beam1.lane));
        else if (!beam1.active && beam2.active) solderpoop = SolderSprite(getDiffLane(beam2.lane));
        else solderpoop = SolderSprite(3-beam1.lane-beam2.lane);
      } else if (component.respawnFlag && !solderpoop.at_top) {
        component = ComponentSprite(rand() % 3);
      }


      if ((dash.getLane() == beam1.lane && beam1.getBottomEdge() <= 20) || 
            (dash.getLane() == beam2.lane && beam2.getBottomEdge() <= 20)) {
              dash.inBeam();
              if ((currSound != beamed || ind == bigCount-1) && !(currSound == collected && ind < bigCount - 1)) Sound_Start(beamed, 1550);
      }

      if (dash.getLane() == solderpoop.lane && solderpoop.vertPos <= 15 && solderpoop.vertPos >= 10 && dash.vertPos <= 10) {
        dash.trip();
        if (ind == bigCount-1 || currSound != tripped) Sound_Start(tripped, 3210);
      }

      if (dash.getLane() == component.lane && component.vertPos <= 15 && component.vertPos >= 10 && dash.vertPos <= 10) {
        ComponentSprite::numCollected++;
        component.lane = -1;
        if (ind == bigCount-1 || currSound != collected) Sound_Start(collected, 4303);
      }

      if (ComponentSprite::numCollected >= 6) dash.win = true;
      spData = Sensor.In();
      if (spData < 1000) dash.changeLane(0);
      else if (spData < 2800 && spData > 1200) dash.changeLane(1);
      else if (spData > 3000) dash.changeLane(2);
      buttons.Poll();
      if (buttons.upPressed) dash.jump();
      
      GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    }
  }
}
uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

void DrawBeamToBuffer(BeamSprite beam) {
  // start at max of 0 and lower edge of beam for buffer
  // start at max of 0 and 0-getBottomEdge for beam
  // draw until LANEBUFFHEIGHT for buffer or 140 for beam
  uint8_t bufferInd = MAX(0, beam.getBottomEdge());
  uint8_t beamInd = MAX(0, 0-beam.getBottomEdge());
  while (bufferInd < LANEBUFFHEIGHT && beamInd < 140) {
    for (int i = 0; i < 20; i++) {
      if (beamInd < 20) buffer[bufferInd*LANEBUFFWIDTH + 15 + i] = beam.color0;
      else buffer[bufferInd*LANEBUFFWIDTH + 15 + i] = beam.color1;
    }
    beamInd++;
    bufferInd++;
  } 
}

void bgToBuffer(uint8_t lane) {
  for (uint8_t i = 0; i < LANEBUFFHEIGHT; i++) {
    for (uint8_t j = 0; j < LANEBUFFWIDTH; j++) {
      buffer[i * LANEBUFFWIDTH + j] = bgPix(lane*LANEBUFFWIDTH + j, i);
    }
  }
}

void runnerToBuffer(void) {
  if (!dash.flashOff) {
    for (uint16_t i = 0; i < 3180; i++) {
      if (dash.image[i] != 0xF81F) buffer[i + LANEBUFFWIDTH*dash.vertPos] = dash.image[i];
    }
  }
}

void solderToBuffer(void) {
  //buffer, start from max of 0 and bottom edge of sprite
  // sprite, start from max of 0 - bottom edge of sprite and 0
  //buffer, end at min of 2500 and buffer size - LANEBUFFWIDTH - 
  uint8_t bufferInd = MAX(0, solderpoop.vertPos);
  uint8_t solderInd = MAX(0, 0-solderpoop.vertPos);
  while (bufferInd < LANEBUFFHEIGHT && solderInd < 25){
    for (uint8_t i = 0; i < 25; i++) {
      if (solderPoop[solderInd*25+i] != 0xF81F) {
        buffer[bufferInd*LANEBUFFWIDTH+14+i] = solderPoop[solderInd*25 + i];
      }
    }
    solderInd++;
    bufferInd++;
  }
}

void componentToBuffer(void) {
  uint8_t bufferInd = MAX(0, component.vertPos);
  uint8_t componentInd = MAX(0, 0-component.vertPos);
  while (bufferInd < LANEBUFFHEIGHT && componentInd < 25){
    for (uint8_t i = 0; i < 25; i++) {
      if (component.image[componentInd*25+i]!=0xFFFF) {
        if (component.yellow) buffer[bufferInd*LANEBUFFWIDTH+14+i] = 12287;
        else buffer[bufferInd*LANEBUFFWIDTH+14+i] = component.image[componentInd*25+i];
      }
    }
    componentInd++;
    bufferInd++;
  }
}

uint8_t placed = 1;

uint8_t strikes = 3;

bool inInfoPlace = false;

using namespace std;
// use main2 to observe graphics
int main(void){ // main2
  TimerG12_Init();
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Sound_Init();
  ST7735_InitPrintf();
  srand(TIMG12->COUNTERREGS.CTR);
  beam1 = BeamSprite(0,rand()%3);
  Sensor.Init();
  buttons.Init();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_SetRotation(3);
  drawHomeScreen();
  drawMainButtons();
  //for start
  uint8_t languageOutlined = 0;
  uint8_t startOutlined = 0;
  bool languageScreenRedraw = false;
  lock = false;
  AppState currentState = homeState;
  uint8_t gameOverCounter = 0;
  bool gameDone = false;

  //for game
  uint16_t i;
  TimerG12_IntArm(2666666, 2);
  bool strChange = false;
  string outStr;
  string prevStr;
  uint8_t counter;
  bool inPlaceState = false;

  while(1) {
    buttons.Poll();
    buttons.Poll();Clock_Delay1ms(20);

    if (!buttons.somethingPressed) {
      lock = false;
    }

    switch(currentState){
      case homeState:
        inPlaceState = false;
        if(buttons.upPressed && !lock){
          lock = true;
          if(languageOutlined !=1){
            startOutlined = 0;
            languageOutlined = 1;
            if(isSpanish){
              drawSpanishButtons();
              outlineButton(1, 95, 90, 30);

            }
            else{
              drawMainButtons();
              outlineButton(1, 94, 90, 30);    
            }
          }
          else{
            startOutlined = 0;
            languageOutlined = 0;
            currentState = languageMenuState;
            continue;
          }

        }
        else if(buttons.pausePressed && !lock){
          lock = true;
          if(startOutlined != 1){
            languageOutlined = 0;
            startOutlined = 1;
            if(isSpanish){
              drawSpanishButtons();
              outlineButton(92, 95, 66, 30);
            }
            else{
              drawMainButtons();
              outlineButton(92, 95, 66, 30);
            }

          }
          else{
            languageOutlined = 0;
            startOutlined = 0;
            currentState = gameState;
          }
        }
        break;

      case languageMenuState:
        if(!languageScreenRedraw){
          ST7735_FillScreen(PCBGreen);
          if(isSpanish){
            ST7735_SetCursor(4, 3);
            ST7735_OutString((char*)"Presione el Bot\xA2n 1", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(3, 4);
            ST7735_OutString((char*)"(Arriba) Para Cambiar", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(9, 5);
            ST7735_OutString((char*)"al Ingl\x82s", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(6, 7);
            ST7735_OutString((char*)"(Presione Abajo", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(6, 8);
            ST7735_OutString((char*)"Para Cancelar)", ST7735_BLACK, PCBGreen);
          }
          else{
            ST7735_SetCursor(4, 4);
            ST7735_OutString((char*) "Press Button 1 (Up)", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(4, 5);
            ST7735_OutString((char*)"to Switch to Spanish", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(3, 7);
            ST7735_OutString((char*)"(Press Down to Cancel)", ST7735_BLACK, PCBGreen);
          }
          languageScreenRedraw = true;
        }
        if(buttons.upPressed && !lock){
          lock = true;
          isSpanish = !isSpanish;
          currentState = homeState;
          drawHomeScreen();
          if(isSpanish){
            drawSpanishButtons();
          }
          else{
            drawMainButtons();
          }
          languageScreenRedraw = false;
        }
        else if (buttons.downPressed && !lock) {
          lock = true;
          currentState = homeState;
          drawHomeScreen();
          if (isSpanish) drawSpanishButtons();
          else drawMainButtons();
          languageScreenRedraw = false;
        }
        break;
      
      case gameState:
        if(!gameEntered){
          gameEntered = true;
          dash.initHealth();
          ST7735_FillScreen(ST7735_BLACK);
          drawBg();
          __enable_irq();
          counter = 50;
          initText = true;
        }
        if (!buttons.somethingPressed) lock = false;

        if (dash.dead || dash.win) {
          __disable_irq();
          beam1 = BeamSprite(0, rand()%3);
          beam2 = BeamSprite(-1);
          solderpoop = SolderSprite(-1);
          component = ComponentSprite(-1);
          ComponentSprite::numCollected = 0;
          if (dash.dead) {
            currentState = gameOver;
            dash = DashSprite();
          } else {
            currentState = infoPlace;
            //transition to PCB placement state;
            
            dash = DashSprite();
          }
        }

        if (buttons.pausePressed && !stopped && !lock) {__disable_irq(); stopped = true; lock = true;}
        else if (buttons.pausePressed && stopped && !lock) {__enable_irq(); stopped = false; lock = true;}
        if (dash.in_beam_flag) {
          if (isSpanish) outStr = "\xAD\xADSalga del rayo!!";
          else outStr = "Get out of the beam!!";
          counter=0;
        }
        else if (dash.tripFlag) {
          if (isSpanish) outStr = "\xADTropezaste, cuidado!";
          else outStr = "You tripped! Careful!";
          counter=0;
        }
        else if (counter >= 50) {
          if (isSpanish) outStr = "Piezas reclamadas: ";
          else outStr = "Parts collected: ";
          outStr += to_string(ComponentSprite::numCollected);
        }
        else counter++;
        if (outStr != prevStr || initText) {
          initText = false;
          ST7735_FillRect(0,0,160,23,ST7735_BLACK);
          ST7735_SetCursor(0, 0);
          ST7735_OutString(const_cast<char*>(outStr.c_str()), ST7735_WHITE, ST7735_BLACK);
          prevStr = outStr;
        }
        for (uint8_t i = 0; i < 3; i++) {
          bgToBuffer(i);
          if (beam1.lane == i) DrawBeamToBuffer(beam1);
          else if (beam2.lane == i) DrawBeamToBuffer(beam2);
          else if (solderpoop.lane == i && solderpoop.vertPos >= 12) solderToBuffer();
          if (component.lane == i && component.vertPos >= 12) componentToBuffer();
          if (dash.getLane() == i) runnerToBuffer();
          if (solderpoop.lane == i && solderpoop.vertPos < 12) solderToBuffer();
          if (component.lane == i && component.vertPos < 12) componentToBuffer();
          ST7735_DrawBitmap(i*LANEBUFFWIDTH, 127, buffer, LANEBUFFWIDTH, LANEBUFFHEIGHT);
        }
        break;

      case gameOver:
        if(!gameDone){
          gameDone = true;
          gameEntered = false;
          inPlaceState = false;
          __enable_irq();
          drawGameOver();
        }
        else if(buttons.pausePressed && !lock){
          lock = true;
          gameDone = false;
          gameEntered = false;
          startOutlined = 0;
          languageOutlined = 0;
          gameOverCounter = 0;
          drawHomeScreen();
          if(isSpanish){
            drawSpanishButtons();
          }
          else{
            drawMainButtons();
          }
          dash.healthOff();
          currentState = homeState;
          Sound_Stop();
        }
        break;

      case infoPlace:
        if (!inInfoPlace) {
          gameDone = false;
          gameEntered = false;
          __enable_irq();
          placed = 1;
          strikes = 3;
          ST7735_FillScreen(PCBGreen);
          inPlaceState = false;
          for (uint8_t i = 0; i < 6; i++) componentPlaced[i] = false;
          if (!isSpanish) {
            ST7735_SetCursor(1, 1);
            ST7735_OutString((char*)"Now place your components", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(1, 2);
            ST7735_OutString((char*)"where they belong on the", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(2, 3);
            ST7735_OutString((char*)"PCB. Press Up and Down", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(2, 4);
            ST7735_OutString((char*)"buttons to move around", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(2, 5);
            ST7735_OutString((char*)"and press Pause button", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(2, 6);
            ST7735_OutString((char*)"to place the component", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(2,7);
            ST7735_OutString((char*)"", ST7735_BLACK, PCBGreen);
          }
          else {
            ST7735_SetCursor(2, 1);
            ST7735_OutString((char*)"Ahora pongan las piezas", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(3, 2);
            ST7735_OutString((char*)"en sus propios lugares", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(2, 3);
            ST7735_OutString((char*)"en el PCB. Presione los", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(3, 4);
            ST7735_OutString((char*)"botones Arriba y Abajo ", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(3, 5);
            ST7735_OutString((char*)"para cambiar lugares y ", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(2, 6);
            ST7735_OutString((char*)"presione el bot\xA2n pausa", ST7735_BLACK, PCBGreen);
            ST7735_SetCursor(5, 7);
            ST7735_OutString((char*)"para colocarlo", ST7735_BLACK, PCBGreen);
          }
          inInfoPlace = true;
        }
        if (buttons.pausePressed && !lock) {
          lock = true;
          inInfoPlace = false;
          currentState = placeState;
        }
        break;

      case placeState:
        if(!inPlaceState) {
          ST7735_FillScreen(0XFFFF);
          Sound_Stop();
          PCBScreen();
          inPlaceState = true;
        }
        if(placed < 7){
          ST7735_SetCursor(1,12);
          string out;
          if (isSpanish) out = "Intentos restantes: ";
          else out = "Strikes Left: ";
          out += to_string(strikes);
          ST7735_OutString(const_cast<char*>(out.c_str()), ST7735_BLACK, ST7735_WHITE);
        }

        if(buttons.upPressed && !lock){
          lock = true;
          selected = (selected + 5) % 6;
          PCBScreen();
        }

        if(buttons.downPressed && !lock){
          lock = true;
          selected = (selected + 1) % 6;
          PCBScreen();
        }

        if(placed == 1 || placed == 4){
          ST7735_DrawBitmap(6, 40, resistor, 25, 25);  
        }
        if(placed == 2 || placed == 6){
          ST7735_DrawBitmap(6, 40, button, 25, 25);  
        }
        if(placed == 3 || placed == 5){
          ST7735_DrawBitmap(6, 40, led, 25, 25);  
        }
        

        if(buttons.pausePressed && !lock){
          lock = true;
          if(placed == 1 && (selected == 0 || selected == 1)){
            placed++;
            if(selected == 0 && !(componentPlaced[0])){
              ST7735_FillRect(49, 26, 7, 4, 0x4C7B);
              componentPlaced[0] = true;
            }
            else if(selected == 1 && !(componentPlaced[1])){
              ST7735_FillRect(49, 33, 7, 4, 0x4C7B);
              componentPlaced[1] = true;
            }
            else{
              placed--;
              strikes--;
              if(strikes == 0){
                currentState = gameOver;
              }
            }
          }
          else if(placed == 2 && (selected == 2 || selected == 3)){
            placed++;
            if(selected == 2 && !(componentPlaced[2])){
              ST7735_FillRect(63, 82, 4, 4, 0x52AA);
              componentPlaced[2] = true;
            }
            else if(selected == 3 && !(componentPlaced[3])){
              ST7735_FillRect(63, 88, 4, 4, 0x52AA);
              componentPlaced[3] = true;
            }
            else{
              placed--;
              strikes--;
              if(strikes == 0){
                currentState = gameOver;
              }
            }

          }
          else if(placed == 3 && (selected == 4 || selected == 5)){
            placed++;
            if(selected == 4 && !(componentPlaced[4])){
              ST7735_FillSmallCircle(138, 26, 0x001F);
              componentPlaced[4] = true;
            }
            else if(selected == 5 && !(componentPlaced[5])){
              ST7735_FillSmallCircle(131, 26, 0x001F);
              componentPlaced[5] = true;
            }
            else{
              placed--;
              strikes--;
              if(strikes == 0){
                currentState = gameOver;
              }
            }
          }
          else if(placed == 4 && (selected == 0 || selected == 1)){
            placed++;
            if(selected == 0 && !(componentPlaced[0])){
              ST7735_FillRect(49, 26, 7, 4, 0x4C7B);
              componentPlaced[0] = true;
            }
            else if(selected == 1 && !(componentPlaced[1])){
              ST7735_FillRect(49, 33, 7, 4, 0x4C7B);
              componentPlaced[1] = true;
            }
            else{
              placed--;
              strikes--;
              if(strikes == 0){
                currentState = gameOver;
              }
            }
          }
          else if(placed == 5 && (selected == 4 || selected == 5)){
            placed++;
            if(selected == 4 && !(componentPlaced[4])){
              ST7735_FillSmallCircle(138, 26, 0x001F);
              componentPlaced[4] = true;
            }
            else if(selected == 5 && !(componentPlaced[5])){
              ST7735_FillSmallCircle(131, 26, 0x001F);
              componentPlaced[5] = true;
            }
            else{
              placed--;
              strikes--;
              if(strikes == 0){
                currentState = gameOver;
              }
            }
          }
          else if(placed == 6 && (selected == 2 || selected == 3)){
            placed++;
            if(selected == 2 && !(componentPlaced[2])){
              ST7735_FillRect(63, 82, 4, 4, 0x52AA);
              componentPlaced[2] = true;
            }
            else if(selected == 3 && !(componentPlaced[3])){
              ST7735_FillRect(63, 88, 4, 4, 0x52AA);
              componentPlaced[3] = true;
            }
            else{
              placed--;
              strikes--;
              if(strikes == 0){
                currentState = gameOver;
              }
            }
            currentState = winState;
          }
          else{
            strikes--;
            if(strikes == 0){
              currentState = gameOver;
            }
          }
        }
        break;

      case winState:
        if(!gameDone){
          gameDone = true;
          inPlaceState = false;
          Sound_Start(won, 9514);
          drawEndScreen();
        }
        else if(buttons.pausePressed && !lock){
          lock = true;
          gameDone = false;
          gameEntered = false;
          startOutlined = 0;
          languageOutlined = 0;
          gameOverCounter = 0;
          drawHomeScreen();
          if(isSpanish){
            drawSpanishButtons();
          }
          else{
            drawMainButtons();
          }
          dash.healthOff();
          currentState = homeState;
          Sound_Stop();
        }
        break;
    }
  }
}