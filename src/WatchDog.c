#include "ioavr.h"
#include "ina90.h"


void WatchDogInit(void)
{
  _WDR(); //����� ����������� �������
  WDTCR|=1<<WDCE;
  WDTCR&=0xF8;  //����������� ������� ������������ ����������� �������, ������������� ����-���� � 16��
  WDTCR|=1<<WDCE;
  WDTCR|=1<<WDE;
}
