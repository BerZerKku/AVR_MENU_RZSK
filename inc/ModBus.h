#include "MyDef.h"

class BazaModBus{
  char i_mod;
  unsigned int* DataMass;
  unsigned char ReadWrite;  // 1 - ������, 2- ������
  unsigned int AddressRegister, DataRegister;
  unsigned char NumCom, NumByte;
  unsigned char* ePass;
  unsigned int* JournalMass;
  unsigned int NumRecNow;

private:
  void writedatatime(unsigned char* mass);
  void writeglobalcurrentstate(unsigned char* mass);
  void writecurrentstate(unsigned char* mass);
  void writemeasureparam(unsigned char* mass);
  void writeparampost(unsigned int numcom, unsigned char* mass);
  void writeparamprm(unsigned int numcom, unsigned char* mass);
  void writeparamprd(unsigned int numcom, unsigned char* mass);
  void writeparamgl(unsigned int numcom, unsigned char* mass);
  void writenumberrecording(unsigned char* mass, unsigned int address);
  void writerecordingarchive(unsigned char* mass);
public:
  //�����������
  BazaModBus(unsigned int* DataMa, unsigned int* JournalMa, unsigned char* Passw){
    DataMass = DataMa;
    JournalMass = JournalMa;
    for(i_mod=0; i_mod<250;i_mod++) DataMass[i_mod] = 0xFFFF;
    for(i_mod=0; i_mod<129; i_mod++) JournalMass[i_mod]=0xEEEE;
    ReadWrite=1;
    //DataMass[0x0016]=((Passw[0]-0x30)<<12)+((Passw[1]-0x30)<<8)+((Passw[2]-0x30)<<4)+((Passw[3]-0x30)&0x0F);
    DataMass[1]=Insertion; //������ ��������, ���� [������� ����]=����, [������� ����]=�����

  }
  //������ � ������ : numcom - ����� �������, mass - ������
  void writeinf(unsigned int numcom, unsigned char* mass);
  //mass - ����, shift - � ����� �������
  void readregister(unsigned char* mass, unsigned char shift, unsigned int address);
  //mass - ������, shift - � ����� �������, address - ��������� �����, numer - ���-�� ����
  void readregisters(unsigned char* mass, unsigned char shift, unsigned int address, unsigned char number);
// ������� ��������� ����������� ���������
  unsigned char status(unsigned char stat); // ���� stat=0 , �� ������������ �������� ReadWrite; =1 - ReadWrite=1, ������ ���������; =2 - ������� ��������
  void writetoAT(unsigned int AddressReg, unsigned int DataReg);
  unsigned char trans(unsigned char num); // num=1, ������������ ������� ��� ������������ ���������, num=2, ������������ ���-�� ������������ ������
  void writeregister(unsigned char* mass); //���������� ������� �� ������������ ���������, ��� ��������� ���������
  void NewPass(unsigned char* mass);
  void NumberRecordingArchive(unsigned int numrecA);
  void ClearJournalMass(void);
  void readarchive(unsigned char* mass, unsigned char shift, unsigned int address); //��������� ������
};

