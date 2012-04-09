//библиотека работы с UART

//сброс регистра ошибок
void ClearPortError(void);

//запустить передачу из буфера
void StartTrans(char SizeMessage);

//старт UART
void StartUART(void);

extern unsigned char Rec_buf_data_uart [64] ; //буфер принимаемых данных
extern unsigned char Tr_buf_data_uart [25] ; //буфер передаваемых данных
extern char Sost;
//extern unsigned char DataRec; //=1, когда получено сообщение


//сброс регистра ошибок
void ClearPortError1(void);

//опрос состо€ни€ UART
char GetSostPort1(void);

//запустить передачу из буфера
void StartTrans1(char SizeMessage);

//старт UART
void StartUART1(void);

extern unsigned char Rec_buf_data_uart1 [64] ; //буфер принимаемых данных
extern unsigned char Tr_buf_data_uart1 [255] ; //буфер передаваемых данных
extern char Sost1;
//extern unsigned char DataRec; //=1, когда получено сообщение

extern unsigned char PCready;
extern unsigned char PCbyte;
extern unsigned char PCtime;

extern bool bUartRcReady1; //true - прин€та посылка, надо обработать
extern unsigned char cNumRecByte;  //кол-во прин€тых байт

