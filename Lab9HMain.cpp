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
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "DashSprite.h"
#include "Switches.h"

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
BeamSprite beam1;
Switches UpDownPause;
uint32_t spData;
bool stopped, lock;

// games  engine runs at 30Hz
void TIMG12_IRQHandler(void){uint32_t pos,msg;
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    dash.tick();
    beam1.tick();
    if (dash.getLane() == beam1.lane && beam1.getBottomEdge() <= 20) {
      dash.inBeam();
    }
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
  for (int i = 0; i < 120; i++) {
    for (int j = 0; j < 53; j++) {
      buffer[i * 53 + j] = bgPix(lane*53 + j, i);
    }
  }
}

void runnerToBuffer(void) {
  for (uint16_t i = 0; i < 3180; i++) {
    if (dash.image[i] != 0xF81F) buffer[i + 53*dash.vertPos] = dash.image[i];
  }
}

void handleRunnerLeaveLane(void) {
  int8_t prev = dash.prevLane();
  if (prev >= 0) {
    bgToBuffer(prev);
    ST7735_DrawBitmap(prev*53, 127, buffer, 53, 120);
    dash.acknowledgeLaneChange();
  }
}

// use main2 to observe graphics
int main(void){ // main2
  srand(TIMG12->COUNTERREGS.CTR);
  TimerG12_Init();
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
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

  uint8_t beamLane = beam1.lane;
  bgToBuffer(beamLane);
  uint8_t tmpLane;
  int8_t prev;
  while(1){
    //check lane update flags, draw buffers based on position of runner, beam, etc.
    if(dash.dead) __disable_irq();
    if (stopped) {
      UpDownPause.Poll();
    }
    if (UpDownPause.pausePressed && !stopped && !lock) {__disable_irq(); stopped = true; lock = true;}
    else if (UpDownPause.pausePressed && stopped && !lock) {__enable_irq(); stopped = false; lock = true;}
    else if (!UpDownPause.pausePressed && lock) lock = false;
    tmpLane = dash.getLane();
    beamLane = beam1.lane;
    if (beam1.getBottomEdge() >= -140) {
      bgToBuffer(beamLane);
      DrawBeamToBuffer(beam1);
      if (tmpLane == beamLane) {
        runnerToBuffer();
        ST7735_DrawBitmap(beam1.getBeamLeftSide(), 127, buffer, 53, 120);
      } else {
        ST7735_DrawBitmap(beam1.getBeamLeftSide(), 127, buffer, 53, 120);
        bgToBuffer(tmpLane);
        runnerToBuffer();
        ST7735_DrawBitmap(tmpLane*53, 127, buffer, 53, 120);
      }
      if (dash.prevLane() != beamLane) handleRunnerLeaveLane();
    } 
    else {
      handleRunnerLeaveLane();
      bgToBuffer(tmpLane);
      runnerToBuffer();
      ST7735_DrawBitmap(tmpLane*53, 127, buffer, 53, 120);
    }
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
