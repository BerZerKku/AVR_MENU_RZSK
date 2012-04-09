#include "MyDef.h"

class BazaModBus{
  char i_mod;
  unsigned int* DataMass;
  unsigned char ReadWrite;  // 1 - чтение, 2- запись
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
  //конструктор
  BazaModBus(unsigned int* DataMa, unsigned int* JournalMa, unsigned char* Passw){
    DataMass = DataMa;
    JournalMass = JournalMa;
    for(i_mod=0; i_mod<250;i_mod++) DataMass[i_mod] = 0xFFFF;
    for(i_mod=0; i_mod<129; i_mod++) JournalMass[i_mod]=0xEEEE;
    ReadWrite=1;
    //DataMass[0x0016]=((Passw[0]-0x30)<<12)+((Passw[1]-0x30)<<8)+((Passw[2]-0x30)<<4)+((Passw[3]-0x30)&0x0F);
    DataMass[1]=Insertion; //версия прошивки, дата [старший байт]=день, [младший байт]=месяц

  }
  //запись в массив : numcom - номер команды, mass - откуда
  void writeinf(unsigned int numcom, unsigned char* mass);
  //mass - куда, shift - с какой позиции
  void readregister(unsigned char* mass, unsigned char shift, unsigned int address);
  //mass - откуда, shift - с какой позиции, address - начальный адрес, numer - кол-во байт
  void readregisters(unsigned char* mass, unsigned char shift, unsigned int address, unsigned char number);
// Функции изменения содержимого регистров
  unsigned char status(unsigned char stat); // если stat=0 , то возвращается значение ReadWrite; =1 - ReadWrite=1, чтение регистров; =2 - запсись регистра
  void writetoAT(unsigned int AddressReg, unsigned int DataReg);
  unsigned char trans(unsigned char num); // num=1, возвращается команда для стандартного протокола, num=2, возвращается кол-во передаваемых данных
  void writeregister(unsigned char* mass); //заполнение массива по стандартному протоколу, для изменения параметра
  void NewPass(unsigned char* mass);
  void NumberRecordingArchive(unsigned int numrecA);
  void ClearJournalMass(void);
  void readarchive(unsigned char* mass, unsigned char shift, unsigned int address); //считываем архивы
};

