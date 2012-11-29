#include "MyDef.h"


__flash unsigned char fNoIzmI[] = "I1= --- ";

__flash unsigned char LCDavarNoUSP[]= "Нет связи с БСП     ";
__flash unsigned char fNoReadVers[]	= "  Инициализация     ";

__flash unsigned char Menu21[] = "1.Журнал.";
__flash unsigned char Menu22[] = "2.Дата/время.";
__flash unsigned char Menu23[] = "3.Управление.";
__flash unsigned char Menu24[] = "4.Установить.";
__flash unsigned char Menu25[] = "5.Просмотр парам.";
__flash unsigned  __flash char *Menu2point[] = 
{
	Menu21, 
	Menu22, 
	Menu23, 
	Menu24,
	Menu25
};

__flash unsigned char Menu31[] = "1. Дата.            ";
__flash unsigned char Menu32[] = "2. Время.           ";

__flash unsigned char Menu51[] = "1.Режим.";
__flash unsigned char Menu52[] = "2.Параметры.";
__flash unsigned char Menu53[] = "3.Пароль.";
__flash unsigned char Menu54[] = "4.Тест.";
__flash unsigned __flash char *Menu5point[] = {Menu51, Menu52, Menu53, Menu54};

__flash unsigned char Menu61[] = ".Защита.";
__flash unsigned char Menu62[] = ".Приемник.";
__flash unsigned char Menu63[] = ".Передатчик.";
__flash unsigned char Menu64[] = ".Общие.";
__flash unsigned __flash char *Menu6point[] = {Menu61, Menu62, Menu63, Menu64}; //массив строк для меню просмотр параметров/ установить параметры

//Параметры Поста
#define MAX_DEF_PARAM (sizeof(Menu7paramPOST) / sizeof(Menu7paramPOST[0]))
__flash unsigned char Menu70[] = "Тип защиты          ";
__flash unsigned char Menu71[] = "Тип линии           ";
__flash unsigned char Menu72[] = "Доп. время без ман. ";
__flash unsigned char Menu73[] = "Комп.задержки в лин.";
__flash unsigned char Menu74[] = "Перекрытие импульсов";
__flash unsigned char Menu75[] = "Загр.чувствит. РЗ   ";
__flash unsigned char Menu76[] = "Порог ПРЕДУПР по РЗ ";
__flash unsigned char Menu77[] = "Тип приемника       ";
__flash unsigned __flash char *Menu7paramPOST[] = 
{
	Menu70, 
	Menu71, 
	Menu72, 
	Menu73, 
	Menu74, 
	Menu75, 
	Menu76, 
	Menu77
};
//диапазон значений для Поста
__flash unsigned char Menu13Shift0[] =	"список";
__flash unsigned char Menu13Shift1[] =	"список";
__flash unsigned char Menu13Shift2[] =	"0..99";
__flash unsigned char Menu13Shift3[] =	"0..18";
__flash unsigned char Menu13Shift4[] =	"0..54";
__flash unsigned char Menu13Shift5[] =	"0..20";
__flash unsigned char Menu13Shift6[] =	"0..20";
__flash unsigned char Menu13Shift7[] =	"список";
__flash unsigned __flash char *Menu13ParamRange[] = 
{
	Menu13Shift0, 
	Menu13Shift1, 
	Menu13Shift2, 
	Menu13Shift3, 
	Menu13Shift4, 
	Menu13Shift5, 
	Menu13Shift6,
	Menu13Shift7
};

//Параметры Приемника
__flash unsigned char Menu81[] = "Время включения     ";
__flash unsigned char Menu82[] = "Задержка на выкл. ?к";
__flash unsigned char Menu83[] = "Блок. команды  4..1 ";
__flash unsigned __flash char *Menu8paramPRM[] = {Menu81, Menu82, Menu83};
#define MAX_PRM_PARAM (sizeof(Menu8paramPRM) / sizeof(Menu8paramPRM[0]))
__flash unsigned char Menu8Shift0[]	= "0..10";
__flash unsigned char Menu8Shift2[]	= "0..1000";
__flash unsigned char Menu8Shift3[]	= "нет";
__flash unsigned __flash char *Menu8ParamRange[] = {Menu8Shift0, Menu8Shift2, Menu8Shift3};


//Параметры Передатчика
__flash unsigned char Menu91[]= "Время включения     ";
__flash unsigned char Menu92[]= "Длительность команды";
__flash unsigned char Menu93[]= "Длит. команды  4..1 ";
__flash unsigned char Menu94[]= "Блок. команды  4..1 ";
__flash unsigned __flash char* Menu9paramPRD[]={Menu91, Menu92, Menu93, Menu94};
#define MAX_PRD_PARAM (sizeof(Menu9paramPRD) / sizeof(Menu9paramPRD[0]))
__flash unsigned char Menu9Shift0[]="0..10";
__flash unsigned char Menu9Shift1[]="20..100";
__flash unsigned char Menu9Shift2[]="";
__flash unsigned __flash char* Menu9ParamRange[]={Menu9Shift0, Menu9Shift1, Menu9Shift2, Menu9Shift2};

//Общие параметры
__flash unsigned char Menu101[]="Удержание реле ПРМ";
__flash unsigned char Menu102[]="Удержание реле ПРД";
__flash unsigned char Menu103[]="Время перезапуска";
__flash unsigned char Menu104[]="Контроль вых.сигнала";
__flash unsigned char Menu105[]="Порог ПРЕДУПР по КЧ ";
__flash unsigned char Menu106[]="Умен.усиления 1 кан.";
#define defNumParamAll 6
__flash unsigned __flash char* Menu10paramAll[]={Menu101, Menu102, Menu103, Menu104, Menu105, Menu106};

__flash unsigned char Menu11d[]="ЗАЩ";
__flash unsigned char Menu11r[]="ПРМ";
__flash unsigned char Menu11r1[]="ПМ1";
__flash unsigned char Menu11r2[]="ПМ2";
__flash unsigned char Menu11t[]="ПРД";

__flash unsigned char Menu11var1[]="Вывед ";
__flash unsigned char Menu11var2[]="Готов ";
__flash unsigned char Menu11var3[]="Введен";
__flash unsigned char Menu11var4[]="Речь  ";
__flash unsigned char Menu11var5[]="Тест1 ";
__flash unsigned char Menu11var6[]="Тест2 ";
__flash unsigned char Menu11var7[]="Тест3 ";

__flash unsigned __flash char* Menu11var[]={Menu11var1, Menu11var2, Menu11var3,Menu11var4,Menu11var5,Menu11var6,Menu11var7};
__flash unsigned char Menu11Err[]="????";

__flash unsigned char TestDelayMline2[]="   Запуск теста     ";
__flash unsigned char TestDelayMline3[]="  Подождите ? сек   ";

__flash unsigned char ParamTypeDef0[]="ДФЗ-ПрПд  ";
__flash unsigned char ParamTypeDef1[]="ДФЗ-ПрмМК1";
__flash unsigned char ParamTypeDef2[]="ДФЗ-ПрмМК2";
__flash unsigned char ParamTypeDef3[]="ДФЗ-L60   ";
__flash unsigned char ParamTypeDef4[]="НЗ-ПрПд   ";
__flash unsigned char ParamTypeDef5[]="НЗ-Прм-МК ";
__flash unsigned char ParamTypeDef6[]="ППЗ-ПрПд  ";
__flash unsigned char ParamTypeDef7[]="ППЗ-Прм-МК";
__flash unsigned char ParamTypeDef8[]="ошибка    ";
#define defNumTypeDef 8
__flash unsigned __flash char *ParamTypeDef[]={	ParamTypeDef0, ParamTypeDef1, ParamTypeDef2, ParamTypeDef3,
												ParamTypeDef4, ParamTypeDef5, ParamTypeDef6, ParamTypeDef7, ParamTypeDef8};

__flash unsigned char ParamLineDef0[]="2 концевая";
__flash unsigned char ParamLineDef1[]="3 концевая";
__flash unsigned char ParamLineDef2[]="ошибка    ";
__flash unsigned __flash char *ParamLineDef[] = {ParamLineDef0, ParamLineDef1, ParamLineDef2};


#define MAX_NUM_ITEM_TYPE_PRM ( (sizeof(ParamTypePrmDef) / sizeof(ParamTypePrmDef[0])) - 1 )
__flash unsigned char ParamTypePrmDef0[] = "акт+пасс ";
__flash unsigned char ParamTypePrmDef1[] = "активная ";
__flash unsigned char ParamTypePrmDef2[] = "пассивная";
__flash unsigned char ParamTypePrmDef3[] = "ошибка   ";
__flash unsigned __flash char *ParamTypePrmDef[] = 
{ 
	ParamTypePrmDef0,
	ParamTypePrmDef1,
	ParamTypePrmDef2,
	ParamTypePrmDef3
};
							

//меню Установить
__flash unsigned char Menu1Def[]="ЗАЩ:";
__flash unsigned char Menu1Rec[]="ПРМ:";
__flash unsigned char Menu1Rec1[]="ПМ1:";
__flash unsigned char Menu1Rec2[]="ПМ2:";
__flash unsigned char Menu1Tran[]="ПРД:";
__flash unsigned char Menu1Err[]="????    ";
__flash unsigned char Menu1disrepair[]="Неиспр  ";
__flash unsigned char Menu1warning[]="Предупр ";

__flash unsigned char Menu1Disc[]= "Выведен ";
__flash unsigned char Menu1Ready[]="Готов   ";
__flash unsigned char Menu1Work[]= "Введен  ";
__flash unsigned char Menu1Talk[]= "Речь    ";
__flash unsigned char Menu1Test1[]="Тест1   ";
__flash unsigned char Menu1Test2[]="Тест2   ";
__flash unsigned char Menu1Test3[]="Тест3   ";
__flash unsigned __flash char* Menu1regime[]={Menu1Disc,Menu1Ready,Menu1Work,Menu1Talk,Menu1Test1,Menu1Test2};
__flash unsigned __flash char* Menu1regimeInp[]={Menu1Disc,Menu1Work};
__flash unsigned __flash char* Menu1regimeInp1[]={Menu1Disc,Menu1Test1,Menu1Test2,Menu1Test3};
__flash unsigned __flash char* Menu1regimeInp2[]={Menu1Disc,Menu1Work,Menu1Test1,Menu1Test2,Menu1Test3};

//состояния Поста
__flash unsigned char Menu1post00[]="Исходн. ";
__flash unsigned char Menu1post01[]="Контроль";
__flash unsigned char Menu1post02[]="Пуск    ";
__flash unsigned char Menu1post03[]="Работа  ";
__flash unsigned char Menu1post04[]="Неиспр. ";
__flash unsigned char Menu1post05[]="П.неиспр";
__flash unsigned char Menu1post06[]="Ожидание";
__flash unsigned char Menu1post07[]="Нал.пуск";
__flash unsigned char Menu1post08[]="?0x08?  ";
__flash unsigned char Menu1post09[]="Нет РЗ  ";
__flash unsigned char Menu1post0A[]="Речь    ";
__flash unsigned __flash char* Menu1condPOST[]={Menu1post00,Menu1post01,Menu1post02,Menu1post03,Menu1post04,Menu1post05,Menu1post06,Menu1post07,Menu1post08,Menu1post09,Menu1post0A};
//состояния ПРМ
__flash unsigned char Menu1prm00[]="Исходн. ";
__flash unsigned char Menu1prm01[]="ПРМ КЧ  ";
__flash unsigned char Menu1prm02[]="ПРМ ПА  ";
__flash unsigned char Menu1prm03[]="Нет КЧ  ";
__flash unsigned char Menu1prm04[]="Неиспр. ";
__flash unsigned char Menu1prm05[]="П.неиспр";
__flash unsigned char Menu1prm06[]="Ожидание";
__flash unsigned char Menu1prm07[]="БЛК КМ  ";
__flash unsigned char Menu1prm08[]="?0x08?  ";
__flash unsigned char Menu1prm09[]="?0x09?  ";
__flash unsigned char Menu1prm0A[]="Речь    ";
__flash unsigned __flash char* Menu1condPRM[]={Menu1prm00,Menu1prm01,Menu1prm02,Menu1prm03,Menu1prm04,Menu1prm05,Menu1prm06,Menu1prm07,Menu1prm08,Menu1prm09,Menu1prm0A};
//состояния ПРД
__flash unsigned char Menu1prd00[]="Исходн. ";
__flash unsigned char Menu1prd01[]="ПРД КЧ  ";
__flash unsigned char Menu1prd02[]="ПРД ПА  ";
__flash unsigned char Menu1prd03[]="Нет КЧ  ";
__flash unsigned char Menu1prd04[]="Неиспр. ";
__flash unsigned char Menu1prd05[]="П.неиспр";
__flash unsigned char Menu1prd06[]="Ожидание";
__flash unsigned char Menu1prd07[]="?0x07?  ";
__flash unsigned char Menu1prd08[]="?0x08?  ";
__flash unsigned char Menu1prd09[]="Систем. ";
__flash unsigned char Menu1prd0A[]="Речь    ";
__flash unsigned __flash char* Menu1condPRD[]={Menu1prd00,Menu1prd01,Menu1prd02,Menu1prd03,Menu1prd04,Menu1prd05,Menu1prd06,Menu1prd07,Menu1prd08,Menu1prd09,Menu1prd0A};

__flash unsigned char GlobalAvar[]="g-";
__flash unsigned char LocalAvar[]="l-";

__flash unsigned char MenuInputData[]="Ввод:";
__flash unsigned char MenuInputOldPassword[]="Пароль:";
__flash unsigned char MenuInputNewPassword[]="Новый пароль:";


__flash unsigned char MenuValue[]="Значение: ";
__flash unsigned char MenuState[]="Сотояние: ";


__flash unsigned char MenuAllSynchrTimer2[]="????";
__flash unsigned char MenuAllSynchrTimer0[]="выкл.";
__flash unsigned char MenuAllSynchrTimer1[]="вкл.";
__flash unsigned __flash char* MenuAllSynchrTimerNum[]={MenuAllSynchrTimer0, MenuAllSynchrTimer1, MenuAllSynchrTimer2};

__flash unsigned char ParamRange[]="Диапазон: ";

//диапазон значение для Общих параметров
__flash unsigned char Menu16Shift0[]="вкл/выкл";
__flash unsigned char Menu16Shift1[]="вкл/выкл";
__flash unsigned char Menu16Shift2[]="0..5";
__flash unsigned char Menu16Shift3[]="вкл/выкл";
__flash unsigned char Menu16Shift4[]="0..22";
__flash unsigned char Menu16Shift5[]="0..22";
__flash unsigned char Menu16Shift6[]="100..1000";
__flash unsigned char Menu16Shift7[]="0..10";
__flash unsigned __flash char* Menu16ParamRange[]={Menu16Shift0,Menu16Shift1,Menu16Shift2,Menu16Shift3, Menu16Shift4,Menu16Shift5,Menu16Shift7};

//Прошивки
__flash unsigned char Menu19Shift0[]="Прошивка P-USP  ";
__flash unsigned char Menu19Shift1[]="Прошивка AT-USP ";
__flash unsigned char Menu19Shift2[]="Прошивка DSP-USP";
__flash unsigned char Menu19Shift3[]="Дополн. параметры";
__flash unsigned char Menu19Shift4[]="Вид параметра";
__flash unsigned __flash char* Menu19Param[]={Menu19Shift0, Menu19Shift1, Menu19Shift2, Menu19Shift3, Menu19Shift4};
  __flash unsigned char fDopParamViewTrue[]="Есть";
  __flash unsigned char fDopParamViewFalse[]="Нет";
  __flash unsigned __flash char*fDopParamView[]={fDopParamViewFalse, fDopParamViewTrue};
  __flash unsigned char fDopParamValueTrue[]="стандарт";
  __flash unsigned char fDopParamValueFalse[]="HEX";
  __flash unsigned __flash char*fDopParamValue[]={fDopParamValueFalse, fDopParamValueTrue};

//Неизвестная ошибка
__flash unsigned char UnknownErrorT[]=       "     Error      ";
//общие предупреждения
__flash unsigned char Menu1GlobalWarning1[] ="Установите часы ";

//ошибки глобальные
__flash unsigned char Menu1GlobalError1[]   ="Неиспр.чт. FLASH";
__flash unsigned char Menu1GlobalError2[]   ="Неиспр.зап.FLASH";
__flash unsigned char Menu1GlobalError4[]   ="Неиспр.чт. PLIS ";
__flash unsigned char Menu1GlobalError8[]   ="Неиспр.зап.PLIS ";
__flash unsigned char Menu1GlobalError10[]  ="Неиспр.зап.2RAM ";
__flash unsigned char Menu1GlobalError100[] ="Неиспр.DSP 2RAM ";
__flash unsigned char Menu1GlobalError200[] ="Неиспр.чт. 2RAM ";
__flash unsigned char Menu1GlobalError400[] ="Установите часы ";
__flash unsigned char Menu1GlobalError800[] ="Низкое напр.вых.";
__flash unsigned char Menu1GlobalError1000[]="Высокое напр.вых";
__flash unsigned __flash char *Menu1GlobalErrorT[] = 
{
	Menu1GlobalError1, 
	Menu1GlobalError2, 
	Menu1GlobalError4, 
	Menu1GlobalError8, 
	Menu1GlobalError10,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
    Menu1GlobalError100, 
	Menu1GlobalError200,
	Menu1GlobalError400,
	Menu1GlobalError800,
	Menu1GlobalError1000,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT
};

//ошибки Защиты
__flash unsigned char Menu1PostError1[]   ="Нет блока БСЗ   ";
__flash unsigned char Menu1PostError2[]   ="Неиспр.верс.БСЗ ";
__flash unsigned char Menu1PostError4[]   ="Неиспр.перекл.  ";
__flash unsigned char Menu1PostError8[]   ="Неиспр.зап. БСЗ ";
__flash unsigned char Menu1PostError100[] ="Неиспр.вход.ПУСК";
__flash unsigned char Menu1PostError200[] ="Неиспр.вход.СТОП";
__flash unsigned char Menu1PostError800[] ="Неиспр.цепь.ВЫХ ";
__flash unsigned char Menu1PostError2000[]="Неиспр.зап. ВЫХ ";
__flash unsigned char Menu1PostError4000[]="Нет сигнала РЗ  ";
__flash unsigned __flash char *Menu1PostErrorT[] = 
{
	Menu1PostError1,
	Menu1PostError2,
	Menu1PostError4,
	Menu1PostError8,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
    Menu1PostError100,
	Menu1PostError200,
	UnknownErrorT,
	Menu1PostError800,
	UnknownErrorT,
	Menu1PostError2000,
	Menu1PostError4000,
	UnknownErrorT
};

//предупреждения Защиты
__flash unsigned char Menu1PostWarning1[] ="Низкий уров. РЗ ";

//ошибки Приемника
__flash unsigned char Menu1PrmError1[]    ="Нет блока БСК   ";
__flash unsigned char Menu1PrmError2[]    ="Неиспр.верс.БСК ";
__flash unsigned char Menu1PrmError4[]    ="Неиспр.нет КЧ1  ";
__flash unsigned char Menu1PrmError8[]    ="Прев.дл-сти.ком.";
__flash unsigned char Menu1PrmError100[]  ="Неиспр.зап. БСК ";
__flash unsigned char Menu1PrmError200[]  ="Неиспр.выкл.КСК ";
__flash unsigned char Menu1PrmError400[]  ="Неиспр.вкл. КСК ";
__flash unsigned char Menu1PrmError800[]  ="Неиспр.контр.КСК";
__flash unsigned char Menu1PrmError1000[] ="Предупр.нет КЧ1 ";
__flash unsigned char Menu1PrmError8000[] ="Прием блок. Ком.";
__flash unsigned __flash char *Menu1PrmErrorT[] = 
{
	Menu1PrmError1,
	Menu1PrmError2,
	Menu1PrmError4,
	Menu1PrmError8,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
    Menu1PrmError100,
	Menu1PrmError200,
	Menu1PrmError400,
	Menu1PrmError800,
	Menu1PrmError1000,
	UnknownErrorT,
	UnknownErrorT,
	Menu1PrmError8000
};

//предупреждения Приемника
__flash unsigned char Menu1PrmWarning1[] = "Сниж. уровня ПРМ";

//ошибки Передатчика
__flash unsigned char Menu1PrdError1[]    ="Нет блока БСК   ";
__flash unsigned char Menu1PrdError2[]    ="Неиспр.верс.БСК ";
__flash unsigned char Menu1PrdError100[]  ="Неиспр.чт.команд";
__flash unsigned char Menu1PrdError200[]  ="Неиспр.выкл.Тест";
__flash unsigned char Menu1PrdError400[]  ="Неиспр.вкл. Тест";
__flash unsigned char Menu1PrdError800[]  ="Неиспр.вход.КСК ";
__flash unsigned __flash char *Menu1PrdErrorT[] = 
{
	Menu1PrdError1,
	Menu1PrdError2,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
    Menu1PrdError100,
	Menu1PrdError200,
	Menu1PrdError400,
	Menu1PrdError800,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT
};

//Тестовый режим 1
__flash unsigned char Menu20line2test1[]= "Гр1:      Гр2:      ";
__flash unsigned char Menu20line2test11[]="Гр1:";
__flash unsigned char Menu20line2test12[]="Гр2:";
__flash unsigned char Menu201line3test1[]="Группа 1";
__flash unsigned char Menu202line3test1[]="Группа 2";
__flash unsigned __flash char* Menu20line3[]={Menu201line3test1, Menu202line3test1};
__flash unsigned char Menu20gr11[]="выкл";
__flash unsigned char Menu20gr12[]="КЧ1";
__flash unsigned char Menu20gr13[]="КЧ2";
__flash unsigned char Menu20gr14[]="Ком1"; __flash unsigned char Menu20gr15[]="Ком2"; __flash unsigned char Menu20gr16[]="Ком3"; __flash unsigned char Menu20gr17[]="Ком4";
__flash unsigned char Menu20gr18[]="Ком5"; __flash unsigned char Menu20gr19[]="Ком6"; __flash unsigned char Menu20gr110[]="Ком7"; __flash unsigned char Menu20gr111[]="Ком8";
__flash unsigned __flash char* Menu20gr1[]={Menu20gr11, Menu20gr12, Menu20gr13, Menu20gr14, Menu20gr15, Menu20gr16, Menu20gr17,
                                             Menu20gr18, Menu20gr19, Menu20gr110, Menu20gr111};
__flash unsigned char Menu20gr21[]="выкл";
__flash unsigned char Menu20gr22[]="РЗ1";
__flash unsigned char Menu20gr23[]="РЗ2";
__flash unsigned __flash char* Menu20gr2[]={Menu20gr21, Menu20gr22, Menu20gr23};

__flash unsigned char Menu201line4[]="Тест 1";
__flash unsigned char Menu202line4[]="Тест 2";
__flash unsigned __flash char* Menu20line4[]={Menu201line4, Menu202line4};

__flash unsigned char Menu20no[]="Тест не запущен";

//Архивы
__flash unsigned char Archive[]="Журнал ";
__flash unsigned char ArchiveEvent[]="событий";
__flash unsigned char ArchiveDef[]= "защиты";
__flash unsigned char ArchivePrm[]= "приемника";
__flash unsigned char ArchivePrd[]= "передатчика";
__flash unsigned __flash char* Menu4point[]={ArchiveEvent,ArchiveDef,ArchivePrm,ArchivePrd};
__flash unsigned char Menu21event[]="События.";
__flash unsigned char Menu2xRdArch[]="Чтение архива.";
__flash unsigned char ArchPrm[]="ПМ";
__flash unsigned char ArchPrd[]="ПД";
__flash unsigned char ArchEvt[]="СБ";
__flash unsigned char ArchDef[]="ЗЩ";
__flash unsigned __flash char* Menu21e[]={ArchEvt, ArchPrd, ArchPrm, ArchDef};

__flash unsigned char Menu21g[]="ОБЩ";

__flash unsigned char ArchEv00[]="";
__flash unsigned char ArchEv01[]="Н.вкл.тест ";
__flash unsigned char ArchEv02[]="Н.выкл.тест";
__flash unsigned char ArchEv03[]="Н.входа КСК";
__flash unsigned char ArchEv04[]="Н.вкл. КСК ";
__flash unsigned char ArchEv05[]="Н.выкл. КСК";
__flash unsigned char ArchEv06[]="Н.зап. БСК ";
__flash unsigned char ArchEv07[]="Н.контр.БСК";
__flash unsigned char ArchEv08[]="Нет бл. БСЗ";
__flash unsigned char ArchEv09[]="Ош.верс.БСЗ";
__flash unsigned char ArchEv10[]="Н.перек.БСЗ";
__flash unsigned char ArchEv11[]="Н.кон.вых.ц";
__flash unsigned char ArchEv12[]="Н.зап.вых.ц";
__flash unsigned char ArchEv13[]="Нет с. ПУСК";
__flash unsigned char ArchEv14[]="Нет с. СТОП";
__flash unsigned char ArchEv15[]="Нет сигн.РЗ";
__flash unsigned char ArchEv16[]="Перезапуск ";
__flash unsigned char ArchEv17[]="Изм.режима ";
__flash unsigned char ArchEv18[]="Восст. 1КЧ ";
__flash unsigned char ArchEv19[]="Восст. 2КЧ ";
__flash unsigned char ArchEv20[]="Нет 1КЧ 5с ";
__flash unsigned char ArchEv21[]="Нет 2КЧ 5с ";
__flash unsigned char ArchEv22[]="Н.работ.DSP";
__flash unsigned char ArchEv23[]="Вос.раб.DSP";
__flash unsigned char ArchEv24[]="Н.чт. 2RAM ";
__flash unsigned char ArchEv25[]="Н.зап. 2RAM";
__flash unsigned char ArchEv26[]="Н.времени  ";
__flash unsigned char ArchEv27[]="Н.зап. PLIS";
__flash unsigned char ArchEv28[]="Н.чт. PLIS ";
__flash unsigned char ArchEv29[]="Н.чт. FLASH";
__flash unsigned char ArchEv30[]="Н.зап.FLASH";
__flash unsigned char ArchEv31[]="Низ.нап.вых";
__flash unsigned char ArchEv32[]="Выс.нап.вых";
__flash unsigned __flash char* Menu21ev[]={ArchEv00,
                                           ArchEv01,ArchEv02,ArchEv03,ArchEv04,ArchEv05,ArchEv06,ArchEv07,ArchEv08,
                                           ArchEv09,ArchEv10,ArchEv11,ArchEv12,ArchEv13,ArchEv14,ArchEv15,ArchEv16,
                                           ArchEv17,ArchEv18,ArchEv19,ArchEv20,ArchEv21,ArchEv22,ArchEv23,ArchEv24,
                                           ArchEv25,ArchEv26,ArchEv27,ArchEv28,ArchEv29,ArchEv30,ArchEv31,ArchEv32};


__flash unsigned char ArchEvV[]="Перез.";
__flash unsigned char ArchEvV1[]="Перезап ";
__flash unsigned char ArchEvStart[]="Начало ";
__flash unsigned char ArchEvEnd[]=  "Конец ";
__flash unsigned char ArchEvCom[]="команды";

// Элементы пункта Меню / Управление
__flash unsigned char fuControlItem0[] = ".Сброс своего.     ";
__flash unsigned char fuControlItem1[] = ".Пуск налад. вкл.  ";
__flash unsigned char fuControlItem2[] = ".Пуск налад. выкл. ";

// Действия элементов пункта Меню / Управление
// должно соответсвовать eControl
__flash unsigned char fuControlAction0[] = "Сброс";
__flash unsigned char fuControlAction1[] = "Пуск";
__flash unsigned char fuControlAction2[] = "Стоп";
__flash unsigned char __flash *fuControlAction[] = 
{
	fuControlAction0,
	fuControlAction1,
	fuControlAction2
};