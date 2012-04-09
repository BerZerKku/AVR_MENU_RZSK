#include "ioavr.h"
#include "ina90.h"


void WatchDogInit(void)
{
  _WDR(); //сброс сторожевого таймера
  WDTCR|=1<<WDCE;
  WDTCR&=0xF8;  //коэффициент деления предделителя сторожевого таймера, соответствует тайм-ауту в 16мс
  WDTCR|=1<<WDCE;
  WDTCR|=1<<WDE;
}
