#ifndef __MY_DEF
#define __MY_DEF

//��� ������������
//������ ��������, ���� ������ ���� - �� �������
#define Insertion 0x0129

//����� ������� ���� int
#define Hi(a) (unsigned char) (a>>8)
//����� ������� ���� Int
#define Lo(a) (unsigned char) (a)

#define uchar unsigned char
#define uint unsigned int

#define SetBit(Port,Bit) Port|=(1<<Bit)
#define ClrBit(Port,Bit) Port&=~(1<<Bit)
#define TestBit(Port,Bit) Port&(1<<Bit)

//������ ������ � ������� ���� ���� �����
#define LoadHiByte(ints,chars) (((ints)&0x00ff)|((chars)<<8))
#define LoadLoByte(ints,chars) (((ints)&0xff00)|((chars)))
//���� ���������� ���������� ������ UART
#define EnRead 0x02
// ���� ���������� ���������� �������� UART
#define EnWrite 0x04
//���� ���������� ��� ���� ����� ��� �������� ���������
#define CurSost 0x10
//���� ����������, ��� ��������� �������
#define DataRead  0x08

#define AddressDevice 0x01

//������ ������ ����������� �� UART ������
#define MaxLenghtRecBuf 64
//������ ������ ������������ �� UART ������
#define MaxLenghtTrBuf 64
#define MaxLenghtTrBuf1 255
//5 //������������ ���������� ����� ������� *1000���
#define MaxPause 30
#define MaxPause1 5
//������� ���������� ������ ������
#define HardError 0x01
  //��������� ���������� ������
  #define DisableReceive ClrBit(UCSR0B,RXEN0); ClrBit(UCSR0B,RXCIE0)
  //��������� ���������� ������
  #define EnableReceive SetBit(UCSR0B,RXEN0); SetBit(UCSR0B,RXCIE0)
  //�������� �� ���������� ���������� ������
  #define TestReceive TestBit(UCSR0B,RXCIE0)
  //��������� ���������� �� ������������ ������ ��������, ������ ��������
  #define GoTrans SetBit(UCSR0B,TXEN0); SetBit(UCSR0B,UDRIE0)
  //��������� ���������� �� ������������ ������ ��������, ��������� ��������
  #define StopTrans ClrBit(UCSR0B,TXEN0); ClrBit(UCSR0B,UDRIE0)
  //�������� �� ���������� ���������� ��������
  #define TestTrans TestBit(UCSR0B,UDRIE0)
  //��������� ���������� ������
  #define DisableReceive1 ClrBit(UCSR1B,RXEN1); ClrBit(UCSR1B,RXCIE1)
  //��������� ���������� ������
  #define EnableReceive1 SetBit(UCSR1B,RXEN1); SetBit(UCSR1B,RXCIE1)
  //�������� �� ���������� ���������� ������
  #define TestReceive1 TestBit(UCSR1B,RXCIE1)
  //��������� ���������� �� ������������ ������ ��������, ������ ��������
  #define GoTrans1 SetBit(UCSR1B,TXEN1); SetBit(UCSR1B,UDRIE1)
  //��������� ���������� �� ������������ ������ ��������, ��������� ��������
  #define StopTrans1 ClrBit(UCSR1B,TXEN1); ClrBit(UCSR1B,UDRIE1)
  //�������� �� ���������� ���������� ��������
  #define TestTrans1 TestBit(UCSR1B,UDRIE1)
//��������� ������� 0
#define StopT0 TCCR0=0

//��� ������ �� ���������
#define DataTimeNew 1
#define GlobalCurrentStateNew 2
#define CurrentStateNew 3
#define Menu1all 4

//*100�� , ����� �������� ������� � ��
#define PC_wait 15

struct strArchive{
  unsigned char NumDev; //���-�� ���������
  unsigned char CurrDev;  //��������� ����������
  unsigned char Dev[5]; //������ ������� ���������: 0 ���� ����� ��� ����� ���� �� ������� =0�00 ������!
                       //�������: 0 - ������� , 1 - ���, 2- ���, 3- ���
  int RecCount; //���-�� ������� � ������� ������
  char CurCount;  //����� ����� ������ ������ ������
  unsigned char Data[16];
};

#endif
