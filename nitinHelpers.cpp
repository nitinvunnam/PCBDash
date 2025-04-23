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
    ST7735_SetCursor(1,7);
    ST7735_OutString((char*)"Presione al Derecho Para", ST7735_BLACK, PCBGreen);
    ST7735_SetCursor(5,8);
    ST7735_OutString((char*)" Tratar Otra Vez", ST7735_BLACK, PCBGreen);
    // for(int i = 0; i < 2430; i++){
    //   if(ReiniciarButton[i] != 0xFFFF){
    //   buffer[i] = ReiniciarButton[i];
    //   }
    //   else{
    //     buffer[i] = 0x4D0B;
    //   }
    // }

    // ST7735_DrawBitmap(40, 85, buffer, 81, 30);
  }
  else{

    ST7735_SetCursor(5, 3);
    ST7735_OutString((char*)"GAME OVER, BETTER", ST7735_BLACK, PCBGreen);
    ST7735_SetCursor(6, 4);
    ST7735_OutString((char*)"LUCK NEXT TIME!", ST7735_BLACK, PCBGreen);
    ST7735_SetCursor(4, 7);
    ST7735_OutString((char*)"Press to the Right", ST7735_BLACK, PCBGreen);
    ST7735_SetCursor(7, 8);
    ST7735_OutString((char*)"to Try Again", ST7735_BLACK, PCBGreen);
    // for(int i = 0; i < 2270; i++){
    //   if(RestartButton[i] != 0xFFFF){
    //   buffer[i] = RestartButton[i];
    //   }
    //   else{
    //     buffer[i] = 0x4D0B;
    //   }
    // }

    // ST7735_DrawBitmap(43, 85, buffer, 74, 30);
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
  ST7735_DrawBitmap(35, 108, PCB, 121, 90);
  for(int i = 0; i < 6; i++){
    if(componentPlaced[i]){
      drawComponent(i);
    }
  }
  PcbOutline(selected);
}

void drawComponent(uint8_t index){
  switch(index){
    case 0:
      ST7735_FillRect(49, 26, 7, 4, 0x4C7B);
      break;
    case 1:
      ST7735_FillRect(49, 33, 7, 4, 0x4C7B);
      break;
    case 2:
      ST7735_FillRect(63, 82, 4, 4, 0x52AA);
      break;
    case 3:
      ST7735_FillRect(63, 88, 4, 4, 0x52AA);
      break;
    case 4:
      ST7735_FillSmallCircle(138, 26, 0x001F);
      break;
    case 5:
      ST7735_FillSmallCircle(131, 26, 0x001F);
      break;
  }
}

void drawEndScreen(void){
  ST7735_FillScreen(0x4D0B);
  if(isSpanish){
    ST7735_SetCursor(6,3);
    ST7735_OutString((char*)"\xAD", ST7735_BLACK, PCBGreen);
    ST7735_OutString((char*)"Eres Campe\xA2n!", ST7735_BLACK, PCBGreen);
    ST7735_SetCursor(4,4);
    ST7735_OutString((char*)"\xADGracias Por Jugar!", ST7735_BLACK, PCBGreen);
    ST7735_SetCursor(1,7);
    ST7735_OutString((char*)"Presione al Derecho Para", ST7735_BLACK, PCBGreen);
    ST7735_SetCursor(6,8);
    ST7735_OutString((char*)"Jugar Otra Vez", ST7735_BLACK, PCBGreen);
    // for(int i = 0; i < 2430; i++){
    //   if(ReiniciarButton[i] != 0xFFFF){
    //   buffer[i] = ReiniciarButton[i];
    //   }
    //   else{
    //     buffer[i] = 0x4D0B;
    //   }
    // }

    // ST7735_DrawBitmap(40, 85, buffer, 81, 30);
  }
  else{
    ST7735_SetCursor(5, 3);
    ST7735_OutString((char*)"CONGRATS! YOU WON!", ST7735_BLACK, PCBGreen);
    ST7735_SetCursor(3, 4);
    ST7735_OutString((char*)"Thank you for playing.", ST7735_BLACK, PCBGreen);
    ST7735_SetCursor(5, 7);
    ST7735_OutString((char*)"Press to the Right", ST7735_BLACK, PCBGreen);
    ST7735_SetCursor(7, 8);
    ST7735_OutString((char*)"to Play Again", ST7735_BLACK, PCBGreen);

    // for(int i = 0; i < 2270; i++){
    //   if(RestartButton[i] != 0xFFFF){
    //   buffer[i] = RestartButton[i];
    //   }
    //   else{
    //     buffer[i] = 0x4D0B;
    //   }
    // }

    // ST7735_DrawBitmap(43, 85, buffer, 74, 30);
  }
}