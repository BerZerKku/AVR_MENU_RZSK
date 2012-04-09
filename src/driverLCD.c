//подпрограмма работы с ЖК-индикатором
#include <ioavr.h>
#include <ina90.h>
#include "driverLCD.h"

#define SetE PORTB|=1<<5
#define ClrE PORTB&=~(1<<5)
#define SetRS PORTB|=0x40
#define ClrRS PORTB&=0xBF

#define dLCDwrData(data); SetRS; SetE; PORTA = data; ClrE;
#define dLCDwrCom(data); ClrRS; SetE; PORTA = data; ClrE;

static uint8_t uBuf[80];
static uint8_t uCnt = 0;
static uint8_t uStat;

static __flash strLCDinit stInit[] = {	0x30, 63,	// 
										0x30, 2,	//
										0x30, 3,	// первоначальная инициализация	
										0x38, 1,	// 8-бит, 4 строки, 5х8 шрифт
										0x0C, 1,	// вкл.дисплея, курсор отключен
										0x01, 32,	// очистка дисплея, DDRAM в 0
										0x02, 1,	// сброс DDRAM и смещения в 0
										0x00, 0		};

//                                             A    Б   В     Г   Д     Е   Ж     З   И     й   К     Л   М     Н   О     П
static __flash uint8_t CodeLetterLCD[]={0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xA4,0xA5,0xA6,0x4B,0xA7,0x4D,0x48,0x4F,0xA8,
//                                             Р     С   Т     У    Ф    Х   Ц     Ч   Ш     Щ   Ъ     Ы    Ь    Э   Ю     Я
                                             0x50,0x43,0x54,0xA9,0xAA,0x58,0xE1,0xAB,0xAC,0xE2,0xAD,0xAE,0x62,0xAF,0xB0,0xB1,
//                                             а     б    в    г   д     е   ж     з    и    й   к     л   м     н   о     п
                                             0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,
//                                             р    с   т     у   ф     х   ц     ч   ш     щ   ъ     ы    ь    э    ю    я
                                             0x70,0x63,0xBF,0x79,0xE4,0x78,0xE5,0xC0,0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7};

#pragma vector = TIMER2_COMP_vect
__interrupt void Timer2_comp(void)
{
	static uint8_t uCom = 0;
	
	switch(uStat)
	{
		case 2:
		{
			OCR2 = 1;
			dLCDwrData(uBuf[uCnt]);
			uCnt = (uCnt < 79) ? (uCnt + 1) : 0;
			switch(uCnt)
			{
				case 0: 
				{
					uCom = 0x80; 
					uStat = 1;
				}break;
				case 20: 
				{
					uCom = 0xC0; 
					uStat = 1;
				}break;
				case 40: 
				{
					uCom = 0x94; 
					uStat = 1;
				}break;
				case 60: 
				{
					uCom = 0xD4; 
					uStat = 1;
				}break;
			}
		}break;
		case 1:
		{
			OCR2 = (uCom == 0x01) ? 32 : 1;
			dLCDwrCom(uCom);
			uStat = 2;
		}break;
		case 3:
		{ //инициализация ЖК
			if (stInit[uCnt].uCom != 0x00)
			{
				dLCDwrCom(stInit[uCnt].uCom);
				OCR2 = stInit[uCnt].uDelay;
				uCnt++;
			}
			else
			{
				uStat = 2;
				uCnt = 0;
			}
		}break;
	}
} 

/** Вывод строки на экран
 */
void LCDprint(unsigned char Line, unsigned char AddressInLine, unsigned char LCDstatus, unsigned char* bufer, unsigned char convers)
{
	unsigned char StartChar;
	unsigned char cnt = 0;
	
	StartChar = (Line - 1) * 20 + AddressInLine - 1;
	
	while(bufer[cnt] != 0x00)
	{
		if (convers == 1)
		{
			if ( (bufer[cnt] >= 'А') && (bufer[cnt] <= 'я') ) 
				uBuf[StartChar++] = CodeLetterLCD[bufer[cnt] - 192];
			else 
				uBuf[StartChar++] = bufer[cnt];
		}
		else 
			uBuf[StartChar++] = bufer[cnt];
		
		cnt++;
	}
}

/** Очистка 3-х нижник строк
 */
void LCDbufClear(void)
{
	for (char i = 20; i < 80; i++) 
		uBuf[i] = ' ';  
	uCnt = 0;
}

/** Инициализация дисплея
 */
void FuncInitLCD(void)
{
	uCnt = 0;
	uStat = 3;
}

/** Очистка заданного кол-ва символов
 */
void FuncClearCharLCD(unsigned char Line, unsigned char AddressInLine, unsigned char NumberChar)
{
	unsigned char StartChar;
	
	StartChar = (Line - 1) * 20 + AddressInLine - 1;
	for (char i = 0; i < NumberChar; i++) 
		uBuf[StartChar++] = ' ';
}

void LCDprintHEX(unsigned char Line, unsigned char AddressInLine, unsigned char CodePrint){
	unsigned char StartChar;
	StartChar=(Line-1)*20+AddressInLine-1;
	uBuf[StartChar]=CodePrint/16;
	uBuf[StartChar+1]=CodePrint-uBuf[StartChar]*16;
	if (uBuf[StartChar]>9) uBuf[StartChar]+=0x37;
	else  uBuf[StartChar]+=0x30;
	if (uBuf[StartChar+1]>9) uBuf[StartChar+1]+=0x37;
	else  uBuf[StartChar+1]+=0x30;
}
//вывод на экран числа в десятичном виде, со сдвигом влево
void LCDprintDEC(unsigned char Line, unsigned char AddressInLine, unsigned char CodePrint){
	unsigned char StartChar;
	StartChar=(Line-1)*20+AddressInLine-1;
	uBuf[StartChar]=CodePrint/10+0x30;
	if (uBuf[StartChar]!=0x30) StartChar++;
	uBuf[StartChar]=(CodePrint-(CodePrint/10)*10)+0x30;
}

//вывод на экран числа в десятичном виде, со сдвигом вправо
void LCDprintDEC1(unsigned char Line, unsigned char AddressInLine, unsigned char CodePrint){
	unsigned char StartChar;
	StartChar=(Line-1)*20+AddressInLine-1;
	uBuf[StartChar]=CodePrint/10+0x30;
	if (uBuf[StartChar]==0x30) uBuf[StartChar]=' ';
	StartChar++;
	uBuf[StartChar]=(CodePrint-(CodePrint/10)*10)+0x30;
}

//вывод на экран числа в десятичном виде 4 знаков, со сдвигом вправо
void LCDprintDEC2(unsigned char Line, unsigned char AddressInLine, unsigned char CodePrint){
	unsigned char StartChar;
	StartChar=(Line-1)*20+AddressInLine-1;
	uBuf[StartChar]=CodePrint/100+0x30;
	CodePrint=CodePrint-(uBuf[StartChar]-0x30)*100;
	StartChar++;
	uBuf[StartChar]=CodePrint/10+0x30;
	StartChar++;
	uBuf[StartChar]=CodePrint%10+0x30;
	if (uBuf[StartChar-2]==0x30) uBuf[StartChar-2]=' ';
	if (uBuf[StartChar-1]==0x30) uBuf[StartChar-1]=' ';
	StartChar++;
	uBuf[StartChar]='0';
}

void LCDprintBits(unsigned char Line, unsigned char AddressInLine, unsigned char CodePrint){
	unsigned char StartChar;
	unsigned char temp=0x80;
	
	StartChar=(Line-1)*20+AddressInLine-1;
	do{
		uBuf[StartChar]=(CodePrint&temp)+0x30;
		if (uBuf[StartChar]!=0x30) uBuf[StartChar]=0x31;
		StartChar++;
		temp=temp>>1;
	}while(temp!=0);
}

void LCDprintTetr(unsigned char Line, unsigned char AddressInLine, unsigned char CodePrint){
	unsigned char StartChar;
	unsigned char temp=0x08;
	
	StartChar=(Line-1)*20+AddressInLine-1;
	do{
		uBuf[StartChar]=(CodePrint&temp)+0x30;
		if (uBuf[StartChar]!=0x30) uBuf[StartChar]=0x31;
		StartChar++;
		temp=temp>>1;
	}while(temp!=0);
}

/** Вывод строки из flash 
 */
void LCDprintf(unsigned char Line, unsigned char AddressInLine, unsigned char LCDstatus, unsigned __flash char* buferf, unsigned char convers)
{
	unsigned char StartChar;
	unsigned char cnt = 0;
	
	StartChar=(Line-1)*20+AddressInLine-1;

	while(buferf[cnt] != 0x00)
    {
		if (convers == 1)
        {
			if ( (buferf[cnt] >= 'А') && (buferf[cnt] <= 'я') ) 
				uBuf[StartChar++] = CodeLetterLCD[buferf[cnt] - 192];
			else 
				uBuf[StartChar++] = buferf[cnt];
        }
		else
			if (convers == 2)
			{
				uBuf[StartChar++] = ' ';
			}
			else
			{
				uBuf[StartChar++] = buferf[cnt];
			}
		cnt++;
    }
}

void LCDprintInsertion(unsigned char Line, unsigned char AddressInLine, unsigned int CodePrint){
	unsigned char StartChar;
	StartChar=(Line-1)*20+AddressInLine-1;
	
	uBuf[StartChar]=CodePrint/1000+0x30;
	CodePrint=CodePrint-(uBuf[StartChar]-0x30)*1000;
	StartChar++;
	uBuf[StartChar]=CodePrint/100+0x30;
	CodePrint=CodePrint-(uBuf[StartChar]-0x30)*100;
	StartChar++;
	uBuf[StartChar]=CodePrint/10+0x30;
	StartChar++;
	uBuf[StartChar]=CodePrint%10+0x30;
}

/** Вывод символа на экран
 */
void LCDprintChar (unsigned char Line, unsigned char AddressInLine, unsigned int CodePrint){
	unsigned char StartChar;
	
	StartChar = (Line - 1) * 20 + AddressInLine - 1;
	
	uBuf[StartChar] = CodePrint;
}

/** Очистка дисплея
 */
void LCDbufClMenu(void)
{  //очистка 3 нижних строк
	for (unsigned char i = 0; i < 80; i++) 
		uBuf[i] = ' ';
}

void LCDptinrArchCount(unsigned char AddressInLine, unsigned int Count, unsigned char Shift){
	unsigned char tt, cc, ii;
	AddressInLine+=59;  //четвертая строка
	cc=AddressInLine;
	uBuf[AddressInLine++]='(';
	if ((Count==0)||(Count>256)){
		uBuf[AddressInLine++]=0x30;
	}else{
		tt=0;
		if (Shift==255){
			uBuf[AddressInLine++]='2';
			uBuf[AddressInLine++]='5';
			uBuf[AddressInLine++]='6';
		}else{
			Shift++;  //т.к. сдвиг у нас считался с 0, увеличим значение на 1
			if (Shift>99){
				tt=Shift/100;
				uBuf[AddressInLine++]=tt+0x30;
			}
			if (Shift>9){
				tt=Shift/10 - tt*10 ;
				uBuf[AddressInLine++]=tt+0x30;
			}
			uBuf[AddressInLine++]=Shift%10+0x30;
		}
	}
	
	uBuf[AddressInLine++]='/';
	if (Count==256){
		uBuf[AddressInLine++]='2';
		uBuf[AddressInLine++]='5';
		uBuf[AddressInLine++]='6';
	}else
		if (Count<256){
			tt=0;
			ii=(unsigned char) Count;
			if (ii>99){
				tt=ii/100;
				uBuf[AddressInLine++]=tt+0x30;
			}
			if (ii>9){
				tt=(ii/10) - (tt*10);
				uBuf[AddressInLine++]=tt+0x30;
			}
			uBuf[AddressInLine++]=ii%10+0x30;
		}else uBuf[AddressInLine++]='?';  //ошибочное значение
	
	
	uBuf[AddressInLine]=')';
	
	for(cc=(AddressInLine-cc); cc<8; cc++) uBuf[++AddressInLine]=' '; //сотрем незадействованные ячейки
}

void LCDprintData(unsigned char Adr, unsigned char* Mass){
	char i, CodePrint;
	
	for(i=0; i<3; i++){
		if (i==0) CodePrint=Mass[13];
		else
			if (i==1) CodePrint=Mass[14];
			else CodePrint=Mass[15];
			
			uBuf[Adr]=CodePrint>>4;
			if (uBuf[Adr]>9) uBuf[Adr]+=0x37;
			else  uBuf[Adr]+=0x30;
			uBuf[++Adr]=CodePrint&0x0F;
			if (uBuf[Adr]>9) uBuf[Adr]+=0x37;
			else  uBuf[Adr]+=0x30;
			
			if (i<2) uBuf[++Adr]='.';
			Adr++;
	}
}

void LCDprintTime(unsigned char Adr, unsigned char* Mass){
	char i, CodePrint;
	unsigned int ms;
	for(i=0; i<3; i++){
		if (i==0) CodePrint=Mass[11];
		else
			if (i==1) CodePrint=Mass[10];
			else CodePrint=Mass[9];
			
			uBuf[Adr]=CodePrint>>4;
			if (uBuf[Adr]>9) uBuf[Adr]+=0x37;
			else  uBuf[Adr]+=0x30;
			uBuf[++Adr]=CodePrint&0x0F;
			if (uBuf[Adr]>9) uBuf[Adr]+=0x37;
			else  uBuf[Adr]+=0x30;
			
			if (i<2) uBuf[++Adr]=':';
			else uBuf[++Adr]='.';
			Adr++;
	}
	
	ms=Mass[7]+ (Mass[8]<<8);
	if (ms>999){  //ошибочное значение.
		for(i=0; i<3; i++) uBuf[Adr++]='?';
	}else{
		uBuf[Adr+2]=ms%10 +0x30;
		ms=ms/10;
		uBuf[Adr+1]=ms%10 + 0x30;
		uBuf[Adr]=ms/10 + 0x30;
	}
}

