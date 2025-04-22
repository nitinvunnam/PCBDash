#include "nitinHelpers.h"
void drawHomeScreen(void){
  ST7735_FillScreen(0x4D0B);
  ST7735_DrawBitmap(40, 85, PcbStartScreen, 80, 80);
}

void drawGameOver(void){
  ST7735_FillScreen(0x4D0B);
  if(isSpanish){
    ST7735_SetCursor(3,3);
    ST7735_OutString((char*)"Fin Del Juego. \xADMejor", ST7735_BLACK, PCBGreen);
    ST7735_SetCursor(3,4);
    ST7735_OutString((char*)"Suerte la Pr\xA2xima Vez!", ST7735_BLACK, PCBGreen);
    ST7735_SetCursor(3,10);
    ST7735_OutString((char*)"(Presione al Derecho)", ST7735_BLACK, PCBGreen);
    for(int i = 0; i < 2430; i++){
      if(ReiniciarButton[i] != 0xFFFF){
      buffer[i] = ReiniciarButton[i];
      }
      else{
        buffer[i] = 0x4D0B;
      }
    }

    ST7735_DrawBitmap(40, 85, buffer, 81, 30);
  }
  else{

    ST7735_SetCursor(5, 3);
    ST7735_OutString((char*)"GAME OVER, BETTER", ST7735_BLACK, PCBGreen);
    ST7735_SetCursor(6, 4);
    ST7735_OutString((char*)"LUCK NEXT TIME!", ST7735_BLACK, PCBGreen);
    ST7735_SetCursor(4, 10);
    ST7735_OutString((char*)"(Press to the Right)", ST7735_BLACK, PCBGreen);
    for(int i = 0; i < 2270; i++){
      if(RestartButton[i] != 0xFFFF){
      buffer[i] = RestartButton[i];
      }
      else{
        buffer[i] = 0x4D0B;
      }
    }

    ST7735_DrawBitmap(43, 85, buffer, 74, 30);
  }

}

void outlineButton(int16_t x, int16_t y, int16_t w, int16_t h){
  for(uint16_t i = 0; i < 3; i++){
    ST7735_DrawFastHLine(x, y + i , w, 0x35B6);
    ST7735_DrawFastHLine(x, y + h - 7 - i, w, 0x35B6);
    ST7735_DrawFastVLine(x + i, y, h - 7, 0x35B6);
    ST7735_DrawFastVLine(x + w - 1 - i, y , h - 7, 0x35B6);
  }
}

void drawMainButtons(void){
  for(uint16_t i = 0; i < 1980; i++){
    if(StartButton[i] != 0xFFFF){
      buffer[i] = StartButton[i];
    }
    else{
      buffer[i] = 0x4D0B;
    }
  }
  ST7735_DrawBitmap(92, 120, buffer, 66, 30);

  for(uint16_t i = 0; i < 2700; i++){
    if(LanguageButton[i] != 0xFFFF){
      buffer[i] = LanguageButton[i];
    }
    else{
      buffer[i] = 0x4D0B;
    }
  }
  ST7735_DrawBitmap(1, 120, buffer , 90, 30);
}

void drawSpanishButtons(void){
  for(uint16_t i = 0; i < 1890; i++){
    if(IniciarButton[i] != 0xFFFF){
      buffer[i] = IniciarButton[i];
    }
    else{
      buffer[i] = 0x4D0B;
    }
  }
  ST7735_DrawBitmap(90, 120, buffer, 70, 27);


  for(uint16_t i = 0; i < 2376; i++){
    if(IdiomasButton[i] != 0xFFFF){
      buffer[i] = IdiomasButton[i];
    }
    else{
      buffer[i] = 0x4D0B;
    }
  }
  ST7735_DrawBitmap(1, 120, buffer , 88, 27);
}

void PcbOutline(uint8_t index){
  Element current = elements[index];

  if(current.shape == RECT){
    ST7735_DrawFastHLine(current.x, current.y , current.w, 0x35B6);
    ST7735_DrawFastHLine(current.x, current.y + current.h - 7, current.w, 0x35B6);
    ST7735_DrawFastVLine(current.x, current.y, current.h - 7, 0x35B6);
    ST7735_DrawFastVLine(current.x + current.w - 1, current.y , current.h - 7, 0x35B6);
  }
  else{
    for(int i = 0; i < 2; i++){
      ST7735_DrawSmallCircle(current.x + i, current.y, 0x35B6);
    }
  }

}


void PCBScreen(void){
  ST7735_FillScreen(0xFFFF);
  ST7735_DrawBitmap(19, 108, PCB, 121, 90);
  PcbOutline(selected);
}