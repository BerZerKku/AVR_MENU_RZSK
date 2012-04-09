#include "ioavr.h"
#include "ina90.h"
#include "MyDef.h"
#include "comp_a90.h"
#include "modbus.h"
//extern
extern unsigned char eWrite;
extern unsigned char* eMassiveWrite;
extern unsigned int eAddressWrite;
extern unsigned char ePassword[];

/*********** PUBLIC **************/

void BazaModBus::writeinf(unsigned int numcom, unsigned char* mass){
  switch(numcom){
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
    case 0x05:
    case 0x06:
    case 0x07:  {writeparampost(numcom, mass);} break; //параметры Поста
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:  {writeparamprm(numcom, mass);} break; //параметры Приемника
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
    case 0x25:  {writeparamprd(numcom, mass);} break; //параметры Передатчика
    case 0x30:  {writecurrentstate(mass);} break;  // текущее состояние
    case 0x31:  {writeglobalcurrentstate(mass);} break;  // общее текущее состояние
    case 0x32:  {writedatatime(mass);} break;  // дата/время
    case 0x34:  {writemeasureparam(mass);} break;  //измеряемые параметры
    case 0x35:
    case 0x36:
    case 0x37:
    case 0x38:
    case 0x39:
    case 0x3A:
    case 0x3B:
    case 0x3C:  {writeparamgl(numcom, mass);} break;  //общие параметры
    //архивы
    case 0xC1:  {writenumberrecording(mass, 0x00B5);} break;  //кол-во записей в архиве Поста
    case 0xD1:  {writenumberrecording(mass, 0x00B6);} break;  //кол-во записей в архиве Приемника
    case 0xE1:  {writenumberrecording(mass, 0x00B7);} break;  //кол-во записей в архиве Передатчика
    case 0xF1:  {writenumberrecording(mass, 0x00B4);} break;  //кол-во записей в архиве событий
    case 0xC2:
    case 0xD2:
    case 0xE2:
    case 0xF2:  {writerecordingarchive(mass);} break; //чтение записей из архива событий
  }
};

void BazaModBus::readregister(unsigned char* mass, unsigned char shift, unsigned int address){
  //если считываем пароль пользователя
  if (address==0x0016) NewPass(ePassword);
  if (address<256){
    mass[shift] = Hi(DataMass[address]);
    mass[shift+1] = Lo(DataMass[address]);
  }else{
    mass[shift]=0xFF;
    mass[shift+1]=0xFF;
  }
};

void BazaModBus::readarchive(unsigned char* mass, unsigned char shift, unsigned int address){
  if (address<128){
    mass[shift] = Hi(JournalMass[address+1]);
    mass[shift+1] = Lo(JournalMass[address+1]);
  }else{
    mass[shift]=0xFF;
    mass[shift+1]=0xFF;
  }
};

void BazaModBus::readregisters(unsigned char* mass, unsigned char shift, unsigned int address, unsigned char number) {
  for (i_mod=0; i_mod<number; i_mod++){
    if ((address+i_mod)<256){
      mass[shift+i_mod*2] = Hi(DataMass[address+i_mod]);
      mass[shift+1+i_mod*2] = Lo(DataMass[address+i_mod]);
    }
    else{
      mass[shift+i_mod*2]=0xFF;
      mass[shift+1+i_mod*2]=0xFF;
    }
  }
};

unsigned char BazaModBus::status(unsigned char stat){
  if (stat!=0) ReadWrite=stat;
  return ReadWrite;
};

void BazaModBus::writetoAT(unsigned int AddressReg, unsigned int DataReg){
  ReadWrite=2;
  AddressRegister=AddressReg;
  DataRegister=DataReg;
};

unsigned char BazaModBus::trans(unsigned char num){
  unsigned char temp;

  switch(num){
    case 1: temp=NumCom;break;
    case 2: temp=NumByte;break;
    default: temp=0xFF;
  }
  return temp;
};

void BazaModBus::writeregister(unsigned char* mass){
  switch(AddressRegister){
    case 0x0010:
    case 0x0011:
    case 0x0012:
    case 0x0013:
    case 0x0014:
    case 0x0015:{//изменение даты/времени
      for(i_mod=0; i_mod<6; i_mod++) mass[4+i_mod]=((DataMass[i_mod+0x10]/10)<<4)+(DataMass[i_mod+0x10]%10);
      mass[4+AddressRegister-0x10]=(char) (((DataRegister/10)<<4) + (DataRegister%10));
      NumCom=0xB2; NumByte=6;
    }break;
    case 0x0016:{ //запись пароля в EEPROM
      ePassword[0]=((DataRegister>>12)&0x0F)+0x30;
      ePassword[1]=((DataRegister>>8)&0x0F)+0x30;
      ePassword[2]=((DataRegister>>4)&0x0F)+0x30;
      ePassword[3]=(DataRegister&0x0F)+0x30;
      eWrite=1;eAddressWrite=0;eMassiveWrite=ePassword;
    }break;
    case 0x0030:
    case 0x0033:
    case 0x0036:{
      if (DataRegister==2) {NumCom=0x71; NumByte=0;}  //ввод утройств
      if (DataRegister==0) {NumCom=0x70; NumByte=0;}  //вывод устройств
    }break;
    case 0x0050:
    case 0x0051:
    case 0x0052:
    case 0x0053:
    case 0x0054:
    case 0x0055:
    case 0x0056:{
      NumCom=AddressRegister+0x31; NumByte=1;
      mass[4]=DataRegister;
    }break;
    case 0x0060:
    case 0x0061:{
      NumCom=AddressRegister+0x31; NumByte=1;
      mass[4]=DataRegister;
    }break;
    case 0x0062:
    case 0x0063:
    case 0x0064:
    case 0x0065:{
      NumCom=0x94; NumByte=2;
      mass[4]=AddressRegister-0x0061;
      mass[5]=DataRegister;
    }break;
    case 0x0066:
    case 0x0067:
    case 0x0068:
    case 0x0069:{
      NumCom=0x95; NumByte=2;
      mass[4]=AddressRegister-0x0065;
      mass[5]=DataRegister;
    }break;
 /*
    case 0x0062:
    case 0x0063:
    case 0x0064:
    case 0x0065{
      NumCom=0x94; NumByte=2;
      mass[4]=AddressRegister-0x0062+1;
      mass[5]=DataRegister;
    }break;
    case 0x0066:
    case 0x0067:
    case 0x0068:
    case 0x0069:{
      NumCom=0x95; NumByte=2;
      mass[4]=AddressREgister-0x0066+1;
      mass[5]=DataRegister;
    }break;
*/
    case 0x006A:
    case 0x006B:
    case 0x006C:
    case 0x006D:
    case 0x006E:
    case 0x006F:
    case 0x0070:
    case 0x0071:
    case 0x0072:
    case 0x0073:
    case 0x0074:
    case 0x0075:
    case 0x0076:
    case 0x0077:
    case 0x0078:
    case 0x0079:
    case 0x007A:
    case 0x007B:
    case 0x007C:
    case 0x007D:
    case 0x007E:
    case 0x007F:
    case 0x0080:
    case 0x0081:
    case 0x0082:
    case 0x0083:
    case 0x0084:
    case 0x0085:
    case 0x0086:
    case 0x0087:
    case 0x0088:
    case 0x0089:{
      NumCom=0x93; NumByte=2;
      mass[4]=AddressRegister-0x6A+1;
      mass[5]=DataRegister;
    }break;
    case 0x0090:
    case 0x0091:
    case 0x0092:{
      NumCom=AddressRegister+0x11; NumByte=1;
      mass[4]=DataRegister;
    }break;
    case 0x0093:
    case 0x0094:
    case 0x0095:
    case 0x0096:{
      NumCom=0xA4; NumByte=2;
      mass[4]=AddressRegister-0x0092;
      mass[5]=DataRegister;
    }break;
    case 0x0097:
    case 0x0098:
    case 0x0099:
    case 0x009A:{
      NumCom=0xA5; NumByte=2;
      mass[4]=AddressRegister-0x0096;
      mass[5]=DataRegister;
    }break;
/*
    case 0x0093:
    case 0x0094:
    case 0x0095:
    case 0x0096:{
      NumCom=0xA4; NumByte=2;
      mass[4]=AddressRegister-0x93+1;
      mass[5]=DataRegister;
    }break;
    case 0x0097:
    case 0x0098:
    case 0x0099:
    case 0x009A:{
      NumCom=0xA5; NumByte=2;
      mass[4]=AddressRegister-0x97+1;
      mass[5]=DataRegister;
    }break;
*/
    case 0x00A0:
    case 0x00A1:
    case 0x00A2:
    case 0x00A3:
    case 0x00A4:{
      NumCom=AddressRegister+0x15; NumByte=1;
      mass[4]=DataRegister;
    }break;
    case 0x00A5:{
      NumCom=AddressRegister+0x15; NumByte=2;
      mass[4]=Hi(DataRegister);
      mass[5]=Lo(DataRegister);
    }break;
    case 0x00A6:{
      NumCom=AddressRegister+0x15; NumByte=1;
      mass[4]=DataRegister;
    }break;
    case 0x00A7:{
      NumCom=0xBD; NumByte=1;
      mass[4]=DataRegister;
    }break;
    case 0x00B4:{
      NumCom=0xFA; NumByte=0;
      for (i_mod=0; i_mod<129; i_mod++) JournalMass[i_mod]=0xEEEE;
    }break; //стереть архив событий, для этого пишется любое значение в ячейку с количеством записей архива
    case 0x00B5:{
      NumCom=0xCA; NumByte=0;
      for (i_mod=0; i_mod<129; i_mod++) JournalMass[i_mod]=0xEEEE;
    }break; //стереть архив Поста, для этого пишется любое значение в ячейку с количеством записей архива
    case 0x00B6:{
      NumCom=0xDA; NumByte=0;
      for (i_mod=0; i_mod<129; i_mod++) JournalMass[i_mod]=0xEEEE;
    }break; //стереть архив Приемника, для этого пишется любое значение в ячейку с количеством записей архива
    case 0x00B7:{
      NumCom=0xEA; NumByte=0;
      for (i_mod=0; i_mod<129; i_mod++) JournalMass[i_mod]=0xEEEE;
    }break; //стереть архив Передатчика, для этого пишется любое значение в ячейку с количеством записей архива
  }
};

void BazaModBus::NewPass(unsigned char* mass){
  DataMass[0x0016]=((mass[0]-0x30)<<12)+((mass[1]-0x30)<<8)+((mass[2]-0x30)<<4)+((mass[3]-0x30)&0x0F);
};

void BazaModBus::NumberRecordingArchive(unsigned int numrecA){
  NumRecNow=numrecA;  //номер считываемой записи архива
}

void BazaModBus::ClearJournalMass(void){
  for(i_mod=0; i_mod<129; i_mod++) JournalMass[i_mod]=0xEEEE;
}

/*********** PRIVATE *************/

void BazaModBus::writeparampost(unsigned int numcom, unsigned char* mass){
  switch(numcom){
    case 0x01:  {DataMass[0x0050]=mass[4];} break;  //тип защиты
    case 0x02:  {DataMass[0x0051]=mass[4]+1;} break;  //тип линии
    case 0x03:  {DataMass[0x0052]=mass[4];} break;  //допустимое время без манипуляции
    case 0x04:  {DataMass[0x0053]=mass[4];} break;  //задержка на линии
    case 0x05:  {DataMass[0x0054]=mass[4];} break;  //перекрытие импульсов
    case 0x06:  {DataMass[0x0055]=mass[4];} break;  //напряжение порога
    case 0x07:  {DataMass[0x0056]=mass[4];} break; //форсировка
  }
};

void BazaModBus::writeparamprm(unsigned int numcom, unsigned char* mass){
  switch(numcom){
    case 0x11:  {DataMass[0x0060]=mass[4]*USPtime;} break;  //Время включения
    case 0x12:  {DataMass[0x0061]=10*mass[4];} break; //длительность команды
//    case 0x13:  {DataMass[0x006A+mass[4]-1]=mass[5]*10;} break; //задржка на выключение
    case 0x13:  {for(i_mod=0; i_mod<mass[3]; i_mod++) DataMass[0x6A+i_mod]=mass[4+i_mod]*10;} break; //задржка на выключение
    case 0x14:  {for(i_mod=0; i_mod<mass[3]; i_mod++) DataMass[0x0062+i_mod]=mass[4+i_mod];} break; //блокировка команд 1-16
/*
    case 0x14:  {DataMass[0x0062+mass[4]-1]|=mass[5];} break;
*/
    case 0x15:  {for(i_mod=0; i_mod<mass[3]; i_mod++) DataMass[0x0066+i_mod]=mass[4+i_mod];} break; //длительные команды
/*
    case 0x15:  {DataMass[0x0066+mass[4]-1]|=mass[5];} break;
*/
    case 0x16: {DataMass[0x008A]=mass[4];} break;
  }
};

void BazaModBus::writeparamprd(unsigned int numcom, unsigned char* mass){
  switch(numcom){
    case 0x21:  {DataMass[0x0090]=mass[4]*USPtime;} break; //время включения
    case 0x22:  {DataMass[0x0091]=mass[4]*10;} break; //длительность команды
    case 0x23:  {DataMass[0x0092]=mass[4]*10;} break; //время на повторное формирование команды
    case 0x24:  {for(i_mod=0; i_mod<mass[3]; i_mod++) DataMass[0x0093+i_mod]=mass[4+i_mod];} break; //блокировка команд
/*
    case 0x24:  {DataMass[0x0093+mass[4]-1]|=mass[5];} break;
*/
    case 0x25:  {for(i_mod=0; i_mod<mass[3]; i_mod++) DataMass[0x0097+i_mod]=mass[4+i_mod];} break; //длительные команды
/*
    case 0x25: {DataMass[0x0097+mass[4]-1]|=mass[5];} break;
*/
  }
};

void BazaModBus::writecurrentstate(unsigned char* mass){
  for(i_mod=0;i_mod<10;i_mod++){
    DataMass[0x0030+i_mod]=mass[4+i_mod];
  }
};

void BazaModBus::writeglobalcurrentstate(unsigned char* mass){
  for (i_mod=0; i_mod<8; i_mod++){
    DataMass[0x0020+i_mod]=(mass[4+i_mod*2]<<8) + mass[5+i_mod*2];
  }
};

void BazaModBus::writedatatime(unsigned char* mass){

  DataMass[0x0010]=(mass[4]>>4)*10+(mass[4]&0x0F);//год
  DataMass[0x0011]=(mass[5]>>4)*10+(mass[5]&0x0F);//месяц
  DataMass[0x0012]=(mass[6]>>4)*10+(mass[6]&0x0F);//день
  DataMass[0x0013]=(mass[7]>>4)*10+(mass[7]&0x0F);//часы
  DataMass[0x0014]=(mass[8]>>4)*10+(mass[8]&0x0F);//минуты
  DataMass[0x0015]=(mass[9]>>4)*10+(mass[9]&0x0F);//секунды
};

void BazaModBus::writemeasureparam(unsigned char* mass){
  switch(mass[4]){
    case 0x01: {DataMass[0x0040]=(mass[5]<<8)+mass[6];} break;  //I1, мА
    case 0x02: {DataMass[0x0041]=(mass[5]<<8)+mass[6];} break;  //I2, мА
    case 0x03: {DataMass[0x0042]=mass[5]; DataMass[0x0043]=mass[6];} break; //U, В
    case 0x04: {if (mass[5]<127) DataMass[0x0044]=mass[5];  //Uз
                else DataMass[0x0044]=mass[5]+0xFF00;} break;
    case 0x05: {if (mass[5]<127) DataMass[0x0045]=mass[5];  //Uкч
                else DataMass[0x0045]=mass[5]+0xFF00;} break;
    case 0x06: {if (mass[5]<127) DataMass[0x0046]=mass[5];  //Uш
                else DataMass[0x0046]=mass[5]+0xFF00;} break;
    case 0x07: {DataMass[0x0047]=mass[5];} break; //Kov
    case 0x08: {DataMass[0x0048]=0x00; DataMass[0x0049]=(mass[5]<<8)+mass[6];} break; //Pk
    case 0x00: {DataMass[0x0040]=(mass[5]<<8)+mass[6]; //I1, мА
                DataMass[0x0041]=(mass[7]<<8)+mass[8]; //I2, мА
                DataMass[0x0042]=mass[9]; DataMass[0x0043]=mass[10]; //U, В
                if (mass[11]<127) DataMass[0x0044]=mass[11]; //Uз
                else DataMass[0x0044]=mass[11]+0xFF00;
                if (mass[12]<127) DataMass[0x0045]=mass[12]; //Uкч
                else DataMass[0x0045]=mass[12]+0xFF00;
                if (mass[13]<127) DataMass[0x0046]=mass[13]; //Uш
                else DataMass[0x0046]=mass[13]+0xFF00;
                DataMass[0x0047]=mass[14]; //Kov
                DataMass[0x0048]=0x00; DataMass[0x0049]=(mass[15]<<8)+mass[16]; //Pk
               } break;
  }
};

void BazaModBus::writeparamgl(unsigned int numcom, unsigned char* mass){
  switch(numcom){
    case 0x35:  {DataMass[0x00A0]=mass[4];} break; //синхронизация часов
    case 0x36:  {DataMass[0x00A1]=mass[4];} break; //удержание реле команд приемника
    case 0x37:  {DataMass[0x00A2]=mass[4];} break; //удержание реле команд передатчика
    case 0x38:  {DataMass[0x00A3]=mass[4];} break; //адрес аппарата в локальной сети
    case 0x39:  {DataMass[0x00A4]=mass[4];} break; //время перезапуска
    case 0x3A:  {DataMass[0x00A5]=(mass[4]<<8)+mass[5];} break; //частота
    case 0x3B:  {DataMass[0x00A6]=mass[4];} break; //номер аппарата
    case 0x3D:  {DataMass[0x00A7]=mass[4];} break; //контроль выходного сигнала
  }
};

void BazaModBus::writenumberrecording(unsigned char* mass, unsigned int address){
  DataMass[address]=mass[4]+(mass[5]<<8);
};

void BazaModBus::writerecordingarchive(unsigned char* mass){
  for(i_mod=0;i_mod<7;i_mod++) JournalMass[1+i_mod+NumRecNow*15]=mass[4+i_mod];
  JournalMass[8+NumRecNow*15]=mass[11] + (mass[12]<<8);
  for(i_mod=0;i_mod<7;i_mod++) JournalMass[9+i_mod+NumRecNow*15]=mass[13+i_mod];
  NumRecNow++;
};



