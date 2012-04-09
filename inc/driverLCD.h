#ifndef DRV_LCD_H
#define DRV_LCD_H

#include <stdint.h>

/// Структура команды ЖК-индикатора
struct strLCDinit
{
	/// Код команды
	uint8_t uCom;		
	/// Задержка, такты Таймера 2
	uint8_t uDelay;	
};

//
extern void LCDprint (unsigned char Line, unsigned char AddressInLine, unsigned char LCDstatus, unsigned char *bufer, unsigned char convers);
extern void LCDprintf (unsigned char Line, unsigned char AddressInLine, unsigned char LCDstatus, unsigned char __flash *buferf, unsigned char convers);
extern void FuncInitLCD(void);
extern void LCDbufClear(void);
extern void LCDprintHEX(unsigned char Line, unsigned char AddressInLine, unsigned char CodePrint);
extern void FuncClearCharLCD(unsigned char Line, unsigned char AddressInLine, unsigned char NumberChar);
extern void LCDprintDEC(unsigned char Line, unsigned char AddressInLine, unsigned char CodePrint);
extern void LCDprintDEC1(unsigned char Line, unsigned char AddressInLine, unsigned char CodePrint);
extern void LCDprintDEC2(unsigned char Line, unsigned char AddressInLine, unsigned char CodePrint);
extern void LCDprintBits(unsigned char Line, unsigned char AddressLine, unsigned char CodePrint);
extern void LCDprintInsertion(unsigned char Line, unsigned char AddressInLine, unsigned int CodePrint);
extern void LCDprintTetr(unsigned char Line, unsigned char AddressInLine, unsigned char CodePrint);
extern void LCDprintChar (unsigned char Line, unsigned char AddressInLine, unsigned int CodePrint); //вывод на экран 1 символа
extern void LCDbufClMenu(void);
extern void LCDptinrArchCount(unsigned char AddressInLine, unsigned int Count, unsigned char Shift);
extern void LCDprintData(unsigned char Adr, unsigned char* Mass);
extern void LCDprintTime(unsigned char Adr, unsigned char* Mass);

#endif
