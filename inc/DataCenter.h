//Библиотека обработки ModBus

//подпрограмма обработки полученного сообщения
extern void DataModBus(unsigned char NumberByte);
extern void TransDataInf(unsigned char ByteCommand, unsigned char NumberTransByte);

extern void TransDataByte(unsigned char ByteCommand, unsigned char ByteData);
extern unsigned char DataMass[];
extern unsigned char KeyBuf[];
