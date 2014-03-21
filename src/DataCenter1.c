

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

//���������� ����������� ��� �������� ������ �� ModBus
unsigned char NumberRegister; //���������� ����������� ���������
extern unsigned char ReadArch;
unsigned int AddressStartRegister; //����� ������� ��������
unsigned int NumRecStart; //����� ������ ����������� ������
extern unsigned char NumberRec; //��-�� ����������� �������
extern unsigned char ComArch;  //������� ���������� ������
unsigned char StRegister;

extern unsigned char CurrentState[];
extern unsigned char LCD2new, gr1, gr2;

extern void FTest1(unsigned char com);

//������� ��������� ��� CRC-16
//�� ����� ��������� �� ������ ������
//� ���������� ���� ��������� (��� ��������� ���� CRC-16)
//�� ��� ���� ����������� �� ���������
unsigned char GetCRCSum1(unsigned char *buf, char bufsize)
{
  unsigned char CRCbuf;
  unsigned char k;

  CRCbuf=0;
  for (k=2; k<bufsize; k++)
    CRCbuf+=buf[k];

  return(CRCbuf);
}

//������������ ���������� ������� ������ ����� ���������� �� UART
void TransDataByte1(unsigned char ByteCommand, unsigned char ByteData)
{
      Sost1|=CurSost;
    //����� ������������� ��� ����� � �������
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
          //����� ������������� ��� ����� � �������
          Tr_buf_data_uart1[2]=ByteCommand;
          Tr_buf_data_uart1[3]=NumberTransByte;
          CRCSum1=GetCRCSum1(Tr_buf_data_uart1,4+NumberTransByte);
          Tr_buf_data_uart1[4+NumberTransByte]=CRCSum1;
          StartTrans1(5+NumberTransByte);
}



//���������� ��������� � �������
void ErrorMessage1(char code)
{
      Sost1|=CurSost;
    //����� ������������� ��� ����� � �������
      Tr_buf_data_uart1[2]=code;
      Tr_buf_data_uart1[3]=0x02; //�.�. ���������� ��� ����� ������
      CRCSum1=GetCRCSum1(Tr_buf_data_uart1,6);
      Tr_buf_data_uart1[6]=CRCSum1;
      StartTrans1(7);
}


//��������� ��������� ���������
void DataModBus1(unsigned char NumberByte)
{

      if (GetSostPort1() & HardError)  //���� ��������� ������ ������
        {
          Tr_buf_data_uart1[4]=0;
          Tr_buf_data_uart1[5]=0;
          ErrorMessage1(0xE1);
        }
      else
        {
              if ((Rec_buf_data_uart1[0]==0x55)&&(Rec_buf_data_uart1[1]==0xAA))  //�������� ���� �������������
              {
                if (Rec_buf_data_uart1[3]!=NumberByte-5) //������� �������� ���������� ���� ������ � ����������
                {
                    Tr_buf_data_uart1[4]=Rec_buf_data_uart1[3]; //���������� ���-�� ���� ������
                    Tr_buf_data_uart1[5]=NumberByte-5; //�������� ���-�� ���� ������
                    ErrorMessage1(0xE3);
                }
                else
                {
                  CRCSum1=GetCRCSum1(Rec_buf_data_uart1,NumberByte-1);
                  if (CRCSum1!=Rec_buf_data_uart1[NumberByte-1]) //������� ��������� CRC
                  {
                    Tr_buf_data_uart1[4]=Rec_buf_data_uart1[NumberByte-1];  //�������� ����������� �����
                    Tr_buf_data_uart1[5]=CRCSum1; //����������� ����������� �����
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

                        //� ������ ������� ������ � EEPROM
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
              else //���� ����� �� ���, �� ����� ������ �� ������
              {
                EnableReceive1;
              }
        }//end else (col.9)
      ClearPortError1();   //����� ������ �������� ��������� ������� ������� ������
} //end DataModBus


