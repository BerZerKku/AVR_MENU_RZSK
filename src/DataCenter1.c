

#include "ioavr.h"
#include "MyDef.h"
#include "InterfaceS.h"


unsigned int CRCSum1, CRCread;
unsigned char i_dc1;
extern unsigned char ePassword[];
unsigned int PassToPC;
extern unsigned char eWrite, eRead;
extern unsigned char *eMassiveWrite, *eMassiveRead;
extern unsigned int eAddressWrite,eAddressRead;

//переменные необходимые для передачи данных по ModBus
unsigned char NumberRegister; //количество считываемых регистров
extern unsigned char ReadArch;
unsigned int AddressStartRegister; //адрес первого регистра
unsigned int NumRecStart; //номер первой считываемой записи
extern unsigned char NumberRec; //ко-во считываемых записей
extern unsigned char ComArch;  //команда считывания архива
unsigned char StRegister;

extern unsigned char CurrentState[];
extern unsigned char LCD2new, gr1, gr2;

extern void FTest1(unsigned char com);

//функция вычисляет код CRC-16
//на входе указатель на начало буфера
//и количество байт сообщения (без принятого кода CRC-16)
//но при этом синхробайты не считаются
unsigned char GetCRCSum1(unsigned char *buf, char bufsize)
{
  unsigned char CRCbuf;
  unsigned char k;

  CRCbuf=0;
  for (k=2; k<bufsize; k++)
    CRCbuf+=buf[k];

  return(CRCbuf);
}

//подпрограмма подготовки отпраки одного байта информации по UART
void TransDataByte1(unsigned char ByteCommand, unsigned char ByteData)
{
      Sost1|=CurSost;
    //байты синхронизации уже лежат в массиве
      Tr_buf_data_uart1[2]=ByteCommand;
      Tr_buf_data_uart1[3]=0x01;
      Tr_buf_data_uart1[4]=ByteData;
      CRCSum1=GetCRCSum1(Tr_buf_data_uart1,5);
      Tr_buf_data_uart1[5]=CRCSum1;
      StartTrans1(6);
}

void TransDataInf1(unsigned char ByteCommand, unsigned char NumberTransByte)
{
      Sost1|=CurSost;
          //байты синхронизации уже лежат в массиве
          Tr_buf_data_uart1[2]=ByteCommand;
          Tr_buf_data_uart1[3]=NumberTransByte;
          CRCSum1=GetCRCSum1(Tr_buf_data_uart1,4+NumberTransByte);
          Tr_buf_data_uart1[4+NumberTransByte]=CRCSum1;
          StartTrans1(5+NumberTransByte);
}



//возвращаем сообщение с ошибкой
void ErrorMessage1(char code)
{
      Sost1|=CurSost;
    //байты синхронизации уже лежат в массиве
      Tr_buf_data_uart1[2]=code;
      Tr_buf_data_uart1[3]=0x02; //т.е. посылается два байта данных
      CRCSum1=GetCRCSum1(Tr_buf_data_uart1,6);
      Tr_buf_data_uart1[6]=CRCSum1;
      StartTrans1(7);
}


//обработка принятого сообщения
void DataModBus1(unsigned char NumberByte)
{

      if (GetSostPort1() & HardError)  //если произошла ошибка приема
        {
          Tr_buf_data_uart1[4]=0;
          Tr_buf_data_uart1[5]=0;
          ErrorMessage1(0xE1);
        }
      else
        {
              if ((Rec_buf_data_uart1[0]==0x55)&&(Rec_buf_data_uart1[1]==0xAA))  //проверка байт синхронизации
              {
                if (Rec_buf_data_uart1[3]!=NumberByte-5) //сравним принятое количество байт данных с заявленным
                {
                    Tr_buf_data_uart1[4]=Rec_buf_data_uart1[3]; //заявленное кол-во байт данных
                    Tr_buf_data_uart1[5]=NumberByte-5; //принятое кол-во байт данных
                    ErrorMessage1(0xE3);
                }
                else
                {
                  CRCSum1=GetCRCSum1(Rec_buf_data_uart1,NumberByte-1);
                  if (CRCSum1!=Rec_buf_data_uart1[NumberByte-1]) //прверка принятого CRC
                  {
                    Tr_buf_data_uart1[4]=Rec_buf_data_uart1[NumberByte-1];  //принятая контрольная сумма
                    Tr_buf_data_uart1[5]=CRCSum1; //вычисленная контрольная сумма
                    ErrorMessage1(0xE2);
                  }
                  else
                  {
                    if ((Rec_buf_data_uart1[2]==0x74)||(Rec_buf_data_uart1[2]==0x73))
                    {

                      if (Rec_buf_data_uart1[2]==0x74){
                        PassToPC=(ePassword[0]-0x30)*1000+(ePassword[1]-0x30)*100+(ePassword[2]-0x30)*10+(ePassword[3]-0x30);
                        Tr_buf_data_uart1[4]=Hi(PassToPC);
                        Tr_buf_data_uart1[5]=Low(PassToPC);
                        TransDataInf1(0x72, 2);
                      }
                      if (Rec_buf_data_uart1[2]==0x73)
                      {
                        PassToPC=(Rec_buf_data_uart1[4]<<8)+Rec_buf_data_uart1[5];
                        ePassword[0]=PassToPC/1000+0x30;
                        PassToPC-=(ePassword[0]-0x30)*1000;
                        ePassword[1]=PassToPC/100+0x30;
                        PassToPC-=(ePassword[1]-0x30)*100;
                        ePassword[2]=PassToPC/10+0x30;
                        PassToPC-=(ePassword[2]-0x30)*10;
                        ePassword[3]=PassToPC+0x30;

                        TransDataInf1(0x73, 0);

                        //а теперь запишем пароль в EEPROM
                        eWrite=1;
                        eAddressWrite=0;
                        eMassiveWrite=ePassword;
                      }
                      PCtime=PC_wait;
                      EnableReceive1;
                    }
                    else
                    {
                      for (i_dc1=0; i_dc1<NumberByte; i_dc1++)
                      {
                        Tr_buf_data_uart[i_dc1]=Rec_buf_data_uart1[i_dc1];
                        Rec_buf_data_uart1[i_dc1]=0x00;
                      }
                      PCbyte=NumberByte;
                      PCready=1;
                      PCtime=PC_wait;
                    }
                  }
                }

              } //end if (col.11)
              else //если адрес не тот, то опять следим за линией
              {
                EnableReceive1;
              }
        }//end else (col.9)
      ClearPortError1();   //после начала отправки сообщения очистим регистр приема
} //end DataModBus


