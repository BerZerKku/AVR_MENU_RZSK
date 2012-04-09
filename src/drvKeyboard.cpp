#include "ioavr.h"
#include "ina90.h"
#include "MyDef.h"
#include "comp_a90.h"

#include "driverLCD.h"

unsigned char InquiryKeyboard(void){
 unsigned  char TempKey;

 DDRC=0x01;
 for(TempKey=0; TempKey<10; TempKey++);
 TempKey=PINC;
 DDRC=0x02;
 if (TempKey==0xFE){
  for(TempKey=0; TempKey<10; TempKey++);
  TempKey=PINC;
  DDRC=0x04;
  if (TempKey==0xFD){
    for(TempKey=0; TempKey<10; TempKey++);
    TempKey=PINC;
    DDRC=0x08;
    if (TempKey==0xFB){
      for(TempKey=0; TempKey<10; TempKey++);
      TempKey=PINC;
      DDRC=0x00;
    }
  }
 }

 switch(TempKey){
 case 0x7E: TempKey='C'; break;
 case 0xDE: TempKey='#'; break;
 case 0xBE: TempKey='0'; break;
 case 0xEE: TempKey='M'; break;
 case 0x7D: TempKey='D'; break;
 case 0xDD: TempKey='9'; break;
 case 0xBD: TempKey='8'; break;
 case 0xED: TempKey='7'; break;
 case 0x7B: TempKey='U'; break;
 case 0xDB: TempKey='6'; break;
 case 0xBB: TempKey='5'; break;
 case 0xEB: TempKey='4'; break;
 case 0x77: TempKey='E'; break;
 case 0xD7: TempKey='3'; break;
 case 0xB7: TempKey='2'; break;
 case 0xE7: TempKey='1'; break;
 default: TempKey=0xF0;
 }
 return TempKey;
};
