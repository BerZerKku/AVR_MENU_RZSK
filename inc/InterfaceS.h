//���������� ������ � UART

//����� �������� ������
void ClearPortError(void);

//��������� �������� �� ������
void StartTrans(char SizeMessage);

//����� UART
void StartUART(void);

extern unsigned char Rec_buf_data_uart [64] ; //����� ����������� ������
extern unsigned char Tr_buf_data_uart [25] ; //����� ������������ ������
extern char Sost;
//extern unsigned char DataRec; //=1, ����� �������� ���������


//����� �������� ������
void ClearPortError1(void);

//����� ��������� UART
char GetSostPort1(void);

//��������� �������� �� ������
void StartTrans1(char SizeMessage);

//����� UART
void StartUART1(void);

extern unsigned char Rec_buf_data_uart1 [64] ; //����� ����������� ������
extern unsigned char Tr_buf_data_uart1 [255] ; //����� ������������ ������
extern char Sost1;
//extern unsigned char DataRec; //=1, ����� �������� ���������

extern unsigned char PCready;
extern unsigned char PCbyte;
extern unsigned char PCtime;

extern bool bUartRcReady1; //true - ������� �������, ���� ����������
extern unsigned char cNumRecByte;  //���-�� �������� ����

