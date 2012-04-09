//������� ������ � UART ����������� AT90S2313
#include "ioavr.h"
#include "ina90.h"
#include "MyDef.h"
#include "DataCenter.h"
#include "DataCenter1.h"

//extern void StUART(void);

#define StartTimer0 TCNT0=5;TCCR0=0x04; //������ ������� 0
#define InitTimer0 TIFR&=0xFD;TIMSK|=(1<<TOIE0); //������������� �������0

char Sost1=0;  //���������� ��������� �������� ������� ������� �� ������

unsigned char Rec_buf_data_uart1 [MaxLenghtRecBuf] ; //����� ����������� ������
unsigned char Tr_buf_data_uart1 [MaxLenghtTrBuf1] ; //����� ������������ ������

unsigned char RecByteCounter1=0; //������� �������� ����
signed char TrByteCounter1=0;  //������� ������������ ������
signed char NumberTransByte1=0; //���-�� ������������ ������
unsigned char DataPause1=0x00; //���������� ����� ����� ��������� �������
unsigned char StartRec1=0;   //��������� ����� ���������

void StartTrans1(char SizeMessage);


extern void SetStopBit(void);
extern unsigned char InquiryKeyboard(void);
extern unsigned char PressKey;
unsigned char TempPressKey, NewPressKey;
int TimerPressKey;

char Sost=0;  //���������� ��������� �������� ������� ������� �� ������

unsigned char Rec_buf_data_uart [MaxLenghtRecBuf] ; //����� ����������� ������
unsigned char Tr_buf_data_uart [MaxLenghtTrBuf] ; //����� ������������ ������

unsigned char RecByteCounter=0; //������� �������� ����
signed char TrByteCounter=0;  //������� ������������ ������
signed char NumberTransByte=0; //���-�� ������������ ������
unsigned char DataPause=0x00; //���������� ����� ����� ��������� �������
unsigned char StartRec=0;   //��������� ����� ���������

uchar cCurrLen0=0;  //������� �������� ����
uchar cMaxLen0=0; //���-�� ��������� ���� � �������

//���������� ������� � EEPROM
unsigned char ew,er;
unsigned char eWrite, eRead;
unsigned char* eMassiveWrite;
unsigned char* eMassiveRead;
unsigned int eAddressWrite, eAddressRead;

unsigned char PCready=0;
unsigned char PCtime=20;
unsigned char PCbyte=0;

bool bUartRcReady1=false; //true - ������� � ��� , ��� ������� �������, ���� ������������
unsigned char cNumRecByte=0; //��-�� �������� ����

//����� UART
void StartUART(void)
{
//��������� ��������� UART
//  StUART();
//��������� ��� UART1
//��������� ��� UART0
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

#pragma vector=USART0_RXC_vect //����� ���������� ������ UART
__interrupt void UART_RX_interrupt(void)
{
  	unsigned char temp_rec=UDR0;
  	if  (UCSR0A&(1<<FE1)) return;  //FE-������ �����, OVR - ������������ ������ (�����)

	switch (cCurrLen0)
	{
		//�������� 1-��� �����������
		case 0: if (temp_rec == 0x55) cCurrLen0 = 1; break;
		//�������� 2-��� �����������
		case 1: if (temp_rec == 0xAA) cCurrLen0 = 2; else cCurrLen0 = 0; break;
		//���� �������
		case 2:
		{
		  Rec_buf_data_uart[2] = temp_rec;
		  cCurrLen0 = 3;
		}break;
		//���-�� ���� ������
		case 3:
		{
		  Rec_buf_data_uart[3]=temp_rec;
		  if (temp_rec > (MaxLenghtRecBuf - 5)) cCurrLen0 = 0;  //�������� �� ���� ���-�� ������
		  else cCurrLen0 = 4;
		  cMaxLen0 = cCurrLen0 + temp_rec; //���-�� ����������� ������
		}break;
		default:
		{//������ � ��
		  Rec_buf_data_uart[cCurrLen0] = temp_rec;
		  if (cCurrLen0 >= cMaxLen0){
			bUartRcReady1=true; // ������� ���
			cCurrLen0=0;
			DisableReceive;  //��������� �����
			cNumRecByte = cMaxLen0 + 1;
			Sost|=DataRead;
		  }else cCurrLen0++;
		}
	}//end switch(cCurrLen0)
}


#pragma vector=USART0_UDRE_vect //����� ���������� �������� � ����� UART
__interrupt void UART_UDRE_interrupt(void)
{
//  char temp_tr;
  if (TrByteCounter<NumberTransByte)   //���� �� ��� �������� �� ��� �����, �� ���������� ����������
    {
      UDR0=Tr_buf_data_uart[TrByteCounter];
      ++TrByteCounter;
    }
  else    //���� �� �� ��� ��������, �� ������������� �������� � ���������� ������������ ����������
    {
      StopTrans;
      NumberTransByte=0;
      TrByteCounter=0;
      EnableReceive;
      Sost&=~(CurSost);
    }
}

#pragma vector=TIMER0_OVF_vect  //����� ���������� �������/�������� 0 �� ������������
__interrupt void Timer0_overflowed_interrupt(void)
{
  volatile char temp_t0;
  _WDR(); //����� ����������� �������
  TCNT0=5;

// ����� ����������, ������ 1 ��
  if ((PressKey==0xF0)&&(PCready==0)) {
    NewPressKey=InquiryKeyboard();
    if ((NewPressKey==TempPressKey)||(NewPressKey==0xF0)) {
     TimerPressKey++;
     if (TimerPressKey>400) {TimerPressKey=400; TempPressKey=0xF0;}
    }
    else TimerPressKey=0;
    if ((NewPressKey!=TempPressKey)&&(NewPressKey!=0xF0)) {PressKey=NewPressKey; TempPressKey=NewPressKey;}
  }

  if (DataPause1>=MaxPause1) //���� ��������� 3 "������" ����, �� ...
    {
      if ((StartRec1==1)||(Sost1&HardError))  //���� ����� ��������� ��� ���, ������� ��� ����� ��������
        {                                   //���� ���������� ������, �� ������������� �����
          DisableReceive1; //��������� �����
          Sost1|=DataRead;
          DataModBus1(RecByteCounter1);
          StartRec1=0;
          RecByteCounter1=0;
          temp_t0=UDR1;
          Sost1&=~(CurSost);
        }
      else    //���� �� ��������� �� �����������, ��������� ����, ����� ��� ������ ����������
        {
        DataPause1=MaxPause;
        }
    }
  else
    {
      DataPause1++;
    }
//
//������ � EEPROM
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
//������ �� EEPROM
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

//���������� �������� �� UART, � ��������� ��-�� ������������ ������
void StartTrans(char SizeMessage)
{
  NumberTransByte=SizeMessage;
  GoTrans;
}

//����� ��������� ����� UART
char GetSostPort(void)
{
  char temp;

  temp=Sost; //��������� ������� ������
  if (TestReceive) temp|=EnRead;  //���������� ������
  if (TestTrans) temp|=EnWrite;   //���������� ��������
  return(temp);
}

//����� ��������� UART
void ClearPortError(void)
{
  Sost=0;
}

///*******************888
//����� UART
void StartUART1(void)
{
//��������� ��������� UART
//  StUART();
//��������� ��� UART1
//  UBRR1L=207; //��� 16��� �������� ����� 4800���
  UBRR1L=51;
  DDRD&=~(1<<PORTD2);
  DDRD|=(1<<PORTD3);
  PORTD&=~(1<<PORTD2);
  PORTD|=(1<<PORTD3);
//��������� ��� ����-���� � ������ ������� 8 ��������
  UCSR1C=(1<<USBS1)|(1<<UCSZ11)|(1<<UCSZ10);
  UCSR1B=(1<<RXEN1)|(1<<TXEN1); //��������� �����/�������� UART
  EnableReceive1;
  Tr_buf_data_uart1[0]=0x55;
  Tr_buf_data_uart1[1]=0xAA;
}

#pragma vector=USART1_RXC_vect //����� ���������� ������ UART
__interrupt void UART1_RX_interrupt(void)
{
  volatile char temp_rec;
  if  (UCSR1A&(1<<FE1))   //FE-������ �����, OVR - ������������ ������ (�����)
    Sost1|=HardError; //�������� ���������� ������
  if ((StartRec1==0)&&(DataPause1>=MaxPause)) //���� ��� ������ ����, �� ��������� �����
    {
      StartRec1=1;
      RecByteCounter1=0;
      DataPause1=0;
      Sost1|=CurSost;
     }
  if ((StartRec1==1)&&(DataPause1<MaxPause))//���� ����� ��� ����, ��������� �������� ��������, ��� ����������� ��������� �������
    {
      if (RecByteCounter1>(MaxLenghtRecBuf-1)) //���� ����� ����������, �� ����� ������
        {
          Sost1|=HardError;
          temp_rec=UDR1;
        }
      else
        {
          Rec_buf_data_uart1[RecByteCounter1]=UDR1;
          ++RecByteCounter1;
        }
      DataPause1=0;  //�������� ������� ���������� ����� ��������� �������
    }

}

#pragma vector=USART1_UDRE_vect //����� ���������� �������� � ����� UART
__interrupt void UART1_UDRE_interrupt(void)
{
  if (TrByteCounter1<NumberTransByte1)   //���� �� ��� �������� �� ��� �����, �� ���������� ����������
    {
      UDR1=Tr_buf_data_uart1[TrByteCounter1];
      ++TrByteCounter1;
    }
  else    //���� �� �� ��� ��������, �� ������������� �������� � ���������� ������������ ����������
    {
      StopTrans1;
      NumberTransByte1=0;
      TrByteCounter1=0;
      EnableReceive1;
      Sost1&=~(CurSost);
    }
}

//���������� �������� �� UART, � ��������� ��-�� ������������ ������
void StartTrans1(char SizeMessage)
{
  NumberTransByte1=SizeMessage;
  GoTrans1;
}

//����� ��������� ����� UART
char GetSostPort1(void)
{
  char temp;

  temp=Sost1; //��������� ������� ������
  if (TestReceive1) temp|=EnRead;  //���������� ������
  if (TestTrans1) temp|=EnWrite;   //���������� ��������
  return(temp);
}

//����� ��������� UART
void ClearPortError1(void)
{
  Sost1=0;
}


