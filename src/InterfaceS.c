//драйвер работы с UART контроллера AT90S2313
#include "ioavr.h"
#include "ina90.h"
#include "MyDef.h"
#include "DataCenter.h"
#include "DataCenter1.h"

//extern void StUART(void);

#define StartTimer0 TCNT0=5;TCCR0=0x04; //запуск таймера 0
#define InitTimer0 TIFR&=0xFD;TIMSK|=(1<<TOIE0); //инициализация таймера0

char Sost1=0;  //переменная ненулевое значение которой говорит об ошибке

unsigned char Rec_buf_data_uart1 [MaxLenghtRecBuf] ; //буфер принимаемых данных
unsigned char Tr_buf_data_uart1 [MaxLenghtTrBuf1] ; //буфер передаваемых данных

unsigned char RecByteCounter1=0; //счетчик принятых байт
signed char TrByteCounter1=0;  //счетчик передаваемых данных
signed char NumberTransByte1=0; //кол-во передаваемых байтов
unsigned char DataPause1=0x00; //определяет время между принятыми байтами
unsigned char StartRec1=0;   //разрешает прием сообщения

void StartTrans1(char SizeMessage);


extern void SetStopBit(void);
extern unsigned char InquiryKeyboard(void);
extern unsigned char PressKey;
unsigned char TempPressKey, NewPressKey;
int TimerPressKey;

char Sost=0;  //переменная ненулевое значение которой говорит об ошибке

unsigned char Rec_buf_data_uart [MaxLenghtRecBuf] ; //буфер принимаемых данных
unsigned char Tr_buf_data_uart [MaxLenghtTrBuf] ; //буфер передаваемых данных

unsigned char RecByteCounter=0; //счетчик принятых байт
signed char TrByteCounter=0;  //счетчик передаваемых данных
signed char NumberTransByte=0; //кол-во передаваемых байтов
unsigned char DataPause=0x00; //определяет время между принятыми байтами
unsigned char StartRec=0;   //разрешает прием сообщения

uchar cCurrLen0=0;  //счетчик принятых байт
uchar cMaxLen0=0; //кол-во ожидаемых байт в посылке

//переменные общения с EEPROM
unsigned char ew,er;
unsigned char eWrite, eRead;
unsigned char* eMassiveWrite;
unsigned char* eMassiveRead;
unsigned int eAddressWrite, eAddressRead;

unsigned char PCready=0;
unsigned char PCtime=20;
unsigned char PCbyte=0;

bool bUartRcReady1=false; //true - говорит о том , что посылка принята, пора обрабатывать
unsigned char cNumRecByte=0; //ко-во принятых байт

//старт UART
void StartUART(void)
{
//НАчальная установка UART
//  StUART();
//настройки для UART1
//настройки для UART0
  UBRR0L=207;
//  UBRR0L=103;

  DDRE&=~(1<<PORTE0);
  DDRE|=(1<<PORTE1);
  PORTE&=~(1<<PORTE0);
  PORTE|=(1<<PORTE1);
  UCSR0C=(1<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);
  UCSR0B=(1<<RXEN0)|(1<<TXEN0);
  EnableReceive;
  InitTimer0;
  StartTimer0;
  Tr_buf_data_uart[0]=0x55;
  Tr_buf_data_uart[1]=0xAA;
}

#pragma vector=USART0_RXC_vect //адрес прерывания приема UART
__interrupt void UART_RX_interrupt(void)
{
  	unsigned char temp_rec=UDR0;
  	if  (UCSR0A&(1<<FE1)) return;  //FE-ошибка кадра, OVR - переполнение данных (флаги)

	switch (cCurrLen0)
	{
		//прокерка 1-ого синхробайта
		case 0: if (temp_rec == 0x55) cCurrLen0 = 1; break;
		//проверка 2-ого синхробайта
		case 1: if (temp_rec == 0xAA) cCurrLen0 = 2; else cCurrLen0 = 0; break;
		//байт команды
		case 2:
		{
		  Rec_buf_data_uart[2] = temp_rec;
		  cCurrLen0 = 3;
		}break;
		//кол-во байт данных
		case 3:
		{
		  Rec_buf_data_uart[3]=temp_rec;
		  if (temp_rec > (MaxLenghtRecBuf - 5)) cCurrLen0 = 0;  //проверка на макс кол-во данных
		  else cCurrLen0 = 4;
		  cMaxLen0 = cCurrLen0 + temp_rec; //кол-во принимаемых данных
		}break;
		default:
		{//данные и КС
		  Rec_buf_data_uart[cCurrLen0] = temp_rec;
		  if (cCurrLen0 >= cMaxLen0){
			bUartRcReady1=true; // приняли все
			cCurrLen0=0;
			DisableReceive;  //остановим прием
			cNumRecByte = cMaxLen0 + 1;
			Sost|=DataRead;
		  }else cCurrLen0++;
		}
	}//end switch(cCurrLen0)
}


#pragma vector=USART0_UDRE_vect //адрес прерывания передачи в буфер UART
__interrupt void UART_UDRE_interrupt(void)
{
//  char temp_tr;
  if (TrByteCounter<NumberTransByte)   //если мы еще передали не все байты, то продолжаем передовать
    {
      UDR0=Tr_buf_data_uart[TrByteCounter];
      ++TrByteCounter;
    }
  else    //если же мы все передали, то останавливаем передачу и сбрасываем используемые переменные
    {
      StopTrans;
      NumberTransByte=0;
      TrByteCounter=0;
      EnableReceive;
      Sost&=~(CurSost);
    }
}

#pragma vector=TIMER0_OVF_vect  //адрес прерывания таймера/счетчика 0 по переполнению
__interrupt void Timer0_overflowed_interrupt(void)
{
  volatile char temp_t0;
  _WDR(); //сброс сторожевого таймера
  TCNT0=5;

// опрос клавиатуры, каждую 1 мс
  if ((PressKey==0xF0)&&(PCready==0)) {
    NewPressKey=InquiryKeyboard();
    if ((NewPressKey==TempPressKey)||(NewPressKey==0xF0)) {
     TimerPressKey++;
     if (TimerPressKey>400) {TimerPressKey=400; TempPressKey=0xF0;}
    }
    else TimerPressKey=0;
    if ((NewPressKey!=TempPressKey)&&(NewPressKey!=0xF0)) {PressKey=NewPressKey; TempPressKey=NewPressKey;}
  }

  if (DataPause1>=MaxPause1) //если насчитали 3 "пустых" бита, то ...
    {
      if ((StartRec1==1)||(Sost1&HardError))  //если прием сообщения уже шел, считаем что прием завершен
        {                                   //если обнаружена ошибка, то останавливаем прием
          DisableReceive1; //остановим прием
          Sost1|=DataRead;
          DataModBus1(RecByteCounter1);
          StartRec1=0;
          RecByteCounter1=0;
          temp_t0=UDR1;
          Sost1&=~(CurSost);
        }
      else    //если же сообщение не принималось, остановим счет, этого уже вполне достаточно
        {
        DataPause1=MaxPause;
        }
    }
  else
    {
      DataPause1++;
    }
//
//запись в EEPROM
  if (((EECR&(1<<EEWE))==0)&&(eWrite==1))
    {
      EEAR=eAddressWrite;
      EEDR=eMassiveWrite[ew];
      EECR|=(1<<EEMWE);
      EECR|=(1<<EEWE);
      ew++;
      eAddressWrite++;
      if (eMassiveWrite[ew-1]==0)
        {
          eWrite=0;
          ew=0;
        }
    }
//чтение из EEPROM
  if (((EECR&(1<<EEWE))==0)&&(eRead==1))
    {
      EEAR=eAddressRead;
      EECR|=(1<<EERE);
      eMassiveRead[er]=EEDR;
      if (eMassiveRead[er]==0xFF)
        {
          eMassiveRead[er]=0x00;
        }
      eAddressRead++;
      er++;
      if (eMassiveRead[er-1]==0)
        {
          eRead=0;
          er=0;
        }
    }

}//end_Timer0_overflowed_interrupt

//разрешение передачи по UART, с указанием ко-ва передаваемых байтов
void StartTrans(char SizeMessage)
{
  NumberTransByte=SizeMessage;
  GoTrans;
}

//опрос состояния порта UART
char GetSostPort(void)
{
  char temp;

  temp=Sost; //прочитать регистр ошибок
  if (TestReceive) temp|=EnRead;  //разрешение приема
  if (TestTrans) temp|=EnWrite;   //разрешение передачи
  return(temp);
}

//сброс состояния UART
void ClearPortError(void)
{
  Sost=0;
}

///*******************888
//старт UART
void StartUART1(void)
{
//НАчальная установка UART
//  StUART();
//настройки для UART1
//  UBRR1L=207; //при 16МГц скорость будет 4800бод
  UBRR1L=51;
  DDRD&=~(1<<PORTD2);
  DDRD|=(1<<PORTD3);
  PORTD&=~(1<<PORTD2);
  PORTD|=(1<<PORTD3);
//установим два стоп-бита и формат посылки 8 разрядов
  UCSR1C=(1<<USBS1)|(1<<UCSZ11)|(1<<UCSZ10);
  UCSR1B=(1<<RXEN1)|(1<<TXEN1); //разрешили прием/передачу UART
  EnableReceive1;
  Tr_buf_data_uart1[0]=0x55;
  Tr_buf_data_uart1[1]=0xAA;
}

#pragma vector=USART1_RXC_vect //адрес прерывания приема UART
__interrupt void UART1_RX_interrupt(void)
{
  volatile char temp_rec;
  if  (UCSR1A&(1<<FE1))   //FE-ошибка кадра, OVR - переполнение данных (флаги)
    Sost1|=HardError; //возникли аппаратные ошибки
  if ((StartRec1==0)&&(DataPause1>=MaxPause)) //если это первый байт, то разрешаем прием
    {
      StartRec1=1;
      RecByteCounter1=0;
      DataPause1=0;
      Sost1|=CurSost;
     }
  if ((StartRec1==1)&&(DataPause1<MaxPause))//если прием уже идет, проверяем интервал ожидания, для определения окончания посылки
    {
      if (RecByteCounter1>(MaxLenghtRecBuf-1)) //если буфер переполнен, то пишем ошибку
        {
          Sost1|=HardError;
          temp_rec=UDR1;
        }
      else
        {
          Rec_buf_data_uart1[RecByteCounter1]=UDR1;
          ++RecByteCounter1;
        }
      DataPause1=0;  //начинаем считать расстояние между принятыми байтами
    }

}

#pragma vector=USART1_UDRE_vect //адрес прерывания передачи в буфер UART
__interrupt void UART1_UDRE_interrupt(void)
{
  if (TrByteCounter1<NumberTransByte1)   //если мы еще передали не все байты, то продолжаем передовать
    {
      UDR1=Tr_buf_data_uart1[TrByteCounter1];
      ++TrByteCounter1;
    }
  else    //если же мы все передали, то останавливаем передачу и сбрасываем используемые переменные
    {
      StopTrans1;
      NumberTransByte1=0;
      TrByteCounter1=0;
      EnableReceive1;
      Sost1&=~(CurSost);
    }
}

//разрешение передачи по UART, с указанием ко-ва передаваемых байтов
void StartTrans1(char SizeMessage)
{
  NumberTransByte1=SizeMessage;
  GoTrans1;
}

//опрос состояния порта UART
char GetSostPort1(void)
{
  char temp;

  temp=Sost1; //прочитать регистр ошибок
  if (TestReceive1) temp|=EnRead;  //разрешение приема
  if (TestTrans1) temp|=EnWrite;   //разрешение передачи
  return(temp);
}

//сброс состояния UART
void ClearPortError1(void)
{
  Sost1=0;
}


