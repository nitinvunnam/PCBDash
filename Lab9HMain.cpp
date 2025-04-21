// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Your name
// Last Modified: 12/26/2024

#include <stdio.h>
#include <stdint.h>
#include "../inc/Clock.h"
#include "../inc/TExaS.h"
#include "../inc/SlidePot.h"
#include "../inc/DAC5.h"
#include "BeamSprite.h"
#include "Bitmaps.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "DashSprite.h"
#include "Switches.h"
#include "SolderSprite.h"
#include "ComponentSprite.h"
#include "nitinHelpers.h"

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

uint32_t M=1;
uint32_t Random32(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32()>>16)%n;
}

SlidePot Sensor(1640,190); // copy calibration from Lab 7

uint16_t buffer[6360] = {0};
DashSprite dash;
BeamSprite beam1(0);
BeamSprite beam2(-1);
SolderSprite solderpoop(-1);
ComponentSprite component(-1);
Switches UpDownPause;
uint32_t spData;
bool stopped, lock;

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
          (dash.getLane() == beam2.lane && beam2.getBottomEdge() <= 20)) dash.inBeam();
    if (dash.getLane() == solderpoop.lane && solderpoop.vertPos <= 5 && solderpoop.vertPos >= 0 && dash.vertPos <= 10) dash.trip();
    if (dash.getLane() == component.lane && component.vertPos <= 5 && component.vertPos >= -5 && dash.vertPos <= 10) {
      ComponentSprite::numCollected++;
      component.lane = -1;
    }
    if (ComponentSprite::numCollected >= 6) dash.win = true;
    spData = Sensor.In();
    if (spData < 1000) dash.changeLane(0);
    else if (spData < 2800 && spData > 1200) dash.changeLane(1);
    else if (spData > 3000) dash.changeLane(2);
    UpDownPause.Poll();
    if (UpDownPause.upPressed) dash.jump();
    // game engine goes here
    // 1) sample slide pot
    // 2) read input switches
    // 3) move sprites
    // 4) start sounds
    // 5) set semaphore
    // NO LCD OUTPUT IN INTERRUPT SERVICE ROUTINES
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
  }
}
uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};
// use main1 to observe special characters
int main1(void){ // main1
    char l;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(0x0000);            // set screen to black
  for(int myPhrase=0; myPhrase<= 2; myPhrase++){
    for(int myL=0; myL<= 3; myL++){
         ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
         ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Clock_Delay1ms(3000);
  ST7735_FillScreen(0x0000);       // set screen to black
  l = 128;
  while(1){
    Clock_Delay1ms(2000);
    for(int j=0; j < 3; j++){
      for(int i=0;i<16;i++){
        ST7735_SetCursor(7*j+0,i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7*j+4,i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }
}

void DrawBeamToBuffer(BeamSprite beam) {
  // start at max of 0 and lower edge of beam for buffer
  // start at max of 0 and 0-getBottomEdge for beam
  // draw until 120 for buffer or 140 for beam
  uint8_t bufferInd = MAX(0, beam.getBottomEdge());
  uint8_t beamInd = MAX(0, 0-beam.getBottomEdge());
  while (bufferInd < 120 && beamInd < 140) {
    for (int i = 0; i < 20; i++) {
      buffer[bufferInd*53 + 15 + i] = beam.beam[beamInd*20 + i];
    }
    beamInd++;
    bufferInd++;
  } 
  // while (bufferInd < 120) {
  //   for (int i = 0; i < 20; i++) {
  //     buffer[bufferInd*53 + 15 + i] = bgPix(15 + i + beam.getBeamLeftSide(), bufferInd);
  //   }
  //   bufferInd++;
  // }
}

void bgToBuffer(uint8_t lane) {
  for (uint8_t i = 0; i < 120; i++) {
    for (uint8_t j = 0; j < 53; j++) {
      buffer[i * 53 + j] = bgPix(lane*53 + j, i);
    }
  }
}

void runnerToBuffer(void) {
  if (!dash.flashOff) {
    for (uint16_t i = 0; i < 3180; i++) {
      if (dash.image[i] != 0xF81F) buffer[i + 53*dash.vertPos] = dash.image[i];
    }
  }
}

void solderToBuffer(void) {
  //buffer, start from max of 0 and bottom edge of sprite
  // sprite, start from max of 0 - bottom edge of sprite and 0
  //buffer, end at min of 2500 and buffer size - 53 - 
  uint8_t bufferInd = MAX(0, solderpoop.vertPos);
  uint8_t solderInd = MAX(0, 0-solderpoop.vertPos);
  while (bufferInd < 120 && solderInd < 25){
    for (uint8_t i = 0; i < 25; i++) {
      if (solderPoop[solderInd*25+i] != 0xF81F) {
        buffer[bufferInd*53+9+i] = solderPoop[solderInd*25 + i];
      }
    }
    solderInd++;
    bufferInd++;
  }
}

void componentToBuffer(void) {
  uint8_t bufferInd = MAX(0, component.vertPos);
  uint8_t componentInd = MAX(0, 0-component.vertPos);
  while (bufferInd < 120 && componentInd < 25){
    for (uint8_t i = 0; i < 25; i++) {
      if (component.image[componentInd*25+i]!=0xF81F) {
        buffer[bufferInd*53+9+i] = component.image[componentInd*25+i];
      }
    }
    componentInd++;
    bufferInd++;
  }
}

// void handleRunnerLeaveLane(void) {
//   int8_t prev = dash.prevLane();
//   int8_t beam1Lane = beam1.lane;
//   // int8_t beam2Lane = beam2.lane;
//   if (prev >= 0) {
//     for (uint8_t i = 0; i < 3; i++) {
//       if (i != beam1Lane) {
//         bgToBuffer(i);
//         ST7735_DrawBitmap(i*53, 127, buffer, 53, 120);
//         dash.acknowledgeLaneChange();
//       }
//     }
//   }
// }

// use main2 to observe graphics
int main(void){ // main2
  TimerG12_Init();
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  srand(TIMG12->COUNTERREGS.CTR);
  beam1 = BeamSprite(0,rand()%3);
  Sensor.Init();
  UpDownPause.Init();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_SetRotation(3);
  ST7735_FillScreen(ST7735_BLACK);
  drawBg();
  uint16_t i;
  dash.initHealth();
  TimerG12_IntArm(2666666, 0);
  __enable_irq();

  while(1){
    //check lane update flags, draw buffers based on position of runner, beam, etc.
    if(dash.dead || dash.win) __disable_irq();
    if (stopped) {
      UpDownPause.Poll();
    }
    if (UpDownPause.pausePressed && !stopped && !lock) {__disable_irq(); stopped = true; lock = true;}
    else if (UpDownPause.pausePressed && stopped && !lock) {__enable_irq(); stopped = false; lock = true;}
    else if (!UpDownPause.pausePressed && lock) lock = false;

    for (uint8_t i = 0; i < 3; i++) {
      bgToBuffer(i);
      if (beam1.lane == i) DrawBeamToBuffer(beam1);
      else if (beam2.lane == i) DrawBeamToBuffer(beam2);
      else if (solderpoop.lane == i && solderpoop.vertPos >= 5) solderToBuffer();
      if (component.lane == i && component.vertPos >= 5) componentToBuffer();
      if (dash.getLane() == i) runnerToBuffer();
      if (solderpoop.lane == i && solderpoop.vertPos < 5) solderToBuffer();
      if (component.lane == i && component.vertPos < 5) componentToBuffer();
      ST7735_DrawBitmap(i*53, 127, buffer, 53, 120);
    }
    // if (beam1.getBottomEdge() >= -140) {
    //   bgToBuffer(beamLane);
    //   DrawBeamToBuffer(beam1);
    //   if (tmpLane == beamLane) {
    //     runnerToBuffer();
    //     ST7735_DrawBitmap(beam1.getBeamLeftSide(), 127, buffer, 53, 120);
    //   } else {
    //     ST7735_DrawBitmap(beam1.getBeamLeftSide(), 127, buffer, 53, 120);
    //     bgToBuffer(tmpLane);
    //     runnerToBuffer();
    //     ST7735_DrawBitmap(tmpLane*53, 127, buffer, 53, 120);
    //   }
    // } 
    // else {
    //   bgToBuffer(tmpLane);
    //   runnerToBuffer();
    //   ST7735_DrawBitmap(tmpLane*53, 127, buffer, 53, 120);
    // }
    // handleRunnerLeaveLane();
  }
}

// use main3 to test switches and LEDs
int main3(void){ // main3
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  while(1){
    // write code to test switches and LEDs

  }
}
// use main4 to test sound outputs
int main4(void){ uint32_t last=0,now;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(ADC0,6,0); // ADC1 channel 6 is PB20, TExaS scope
  __enable_irq();
  while(1){
    now = Switch_In(); // one of your buttons
    if((last == 0)&&(now == 1)){
      Sound_Shoot(); // call one of your sounds
    }
    if((last == 0)&&(now == 2)){
      Sound_Killed(); // call one of your sounds
    }
    if((last == 0)&&(now == 4)){
      Sound_Explosion(); // call one of your sounds
    }
    if((last == 0)&&(now == 8)){
      Sound_Fastinvader1(); // call one of your sounds
    }
    // modify this to test all your sounds
  }
}
// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
int main5(void){ // final main
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  Sensor.Init(); // PB18 = ADC1 channel 5, slidepot
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    // initialize interrupts on TimerG12 at 30 Hz
  
  // initialize all data structures
  __enable_irq();

  while(1){
    // wait for semaphore
       // clear semaphore
       // update ST7735R
    // check for end game or level switch
  }
}
