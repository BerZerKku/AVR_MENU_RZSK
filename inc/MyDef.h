#ifndef __MY_DEF
#define __MY_DEF

//для двухчастотки
//версия прошивки, если первый ноль - то убираем
#define Insertion 0x0129

//Взять старший байт int
#define Hi(a) (unsigned char) (a>>8)
//Взять младший байт Int
#define Lo(a) (unsigned char) (a)

#define uchar unsigned char
#define uint unsigned int

#define SetBit(Port,Bit) Port|=(1<<Bit)
#define ClrBit(Port,Bit) Port&=~(1<<Bit)
#define TestBit(Port,Bit) Port&(1<<Bit)

//макрос записи в старший байт инта байта
#define LoadHiByte(ints,chars) (((ints)&0x00ff)|((chars)<<8))
#define LoadLoByte(ints,chars) (((ints)&0xff00)|((chars)))
//флаг показывает разрешение приема UART
#define EnRead 0x02
// флаг показывает разрешение передачи UART
#define EnWrite 0x04
//флаг показывает что идет прием или передача сообщения
#define CurSost 0x10
//флаг показывает, что сообщение принято
#define DataRead  0x08

#define AddressDevice 0x01

//размер буфера принимаемых по UART данных
#define MaxLenghtRecBuf 64
//размер буфера передаваемых по UART данных
#define MaxLenghtTrBuf 64
#define MaxLenghtTrBuf1 255
//5 //максимальное расстояние между байтами *1000мкс
#define MaxPause 30
#define MaxPause1 5
//наличие аппаратных ошибок приема
#define HardError 0x01
  //запретить прерывание приема
  #define DisableReceive ClrBit(UCSR0B,RXEN0); ClrBit(UCSR0B,RXCIE0)
  //разрешить прерывание приема
  #define EnableReceive SetBit(UCSR0B,RXEN0); SetBit(UCSR0B,RXCIE0)
  //проверка на разрешение прерывания приема
  #define TestReceive TestBit(UCSR0B,RXCIE0)
  //разрешить прерывание по освобождению буфера передачи, начать передачу
  #define GoTrans SetBit(UCSR0B,TXEN0); SetBit(UCSR0B,UDRIE0)
  //запретить прерывание по освобождению буфера передачи, остановка передачи
  #define StopTrans ClrBit(UCSR0B,TXEN0); ClrBit(UCSR0B,UDRIE0)
  //проверка на разрешение прерывания передачи
  #define TestTrans TestBit(UCSR0B,UDRIE0)
  //запретить прерывание приема
  #define DisableReceive1 ClrBit(UCSR1B,RXEN1); ClrBit(UCSR1B,RXCIE1)
  //разрешить прерывание приема
  #define EnableReceive1 SetBit(UCSR1B,RXEN1); SetBit(UCSR1B,RXCIE1)
  //проверка на разрешение прерывания приема
  #define TestReceive1 TestBit(UCSR1B,RXCIE1)
  //разрешить прерывание по освобождению буфера передачи, начать передачу
  #define GoTrans1 SetBit(UCSR1B,TXEN1); SetBit(UCSR1B,UDRIE1)
  //запретить прерывание по освобождению буфера передачи, остановка передачи
  #define StopTrans1 ClrBit(UCSR1B,TXEN1); ClrBit(UCSR1B,UDRIE1)
  //проверка на разрешение прерывания передачи
  #define TestTrans1 TestBit(UCSR1B,UDRIE1)
//остановка таймера 0
#define StopT0 TCCR0=0

//для вывода на индикатор
#define DataTimeNew 1
#define GlobalCurrentStateNew 2
#define CurrentStateNew 3
#define Menu1all 4

//*100мс , время ожидания запроса с ПК
#define PC_wait 15

struct strArchive{
  unsigned char NumDev; //кол-во устройств
  unsigned char CurrDev;  //выбранное устройство
  unsigned char Dev[5]; //массив номеров устройств: 0 байт нужен для выода инфы на дисплей =0х00 всегда!
                       //значния: 0 - события , 1 - прд, 2- прм, 3- защ
  int RecCount; //кол-во записей в текущем архиве
  char CurCount;  //адрес самой старой записи архива
  unsigned char Data[16];
};

#endif
