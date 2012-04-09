//Библиотека обработки ModBus

//подпрограмма обработки полученного сообщения
extern void DataModBus1(unsigned char NumberByte);
extern void TransDataInf1(unsigned char ByteCommand, unsigned char NumberTransByte);

extern void TransDataByte1(unsigned char ByteCommand, unsigned char ByteData);
extern unsigned char DataMass[];
extern unsigned char KeyBuf[];
