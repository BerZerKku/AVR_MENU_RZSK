#ifndef __MENU_H
#define __MENU_H

#include <stdint.h>

/// Номера пунктов меню
enum eMenuLevel
{
	LVL_START = 1,		// 1
	LVL_MENU,
	LVL_DATA_TIME,
	LVL_JRN,
	LVL_SETUP,			// 5
	LVL_VIEW_PARAM,
	LVL_VIEW_DEF,
	LVL_VIEW_PRM,
	LVL_VIEW_PRD,
	LVL_VIEW_GLB,		// 10
	LVL_SETUP_RGM,
	LVL_SETUP_PARAM,	
	LVL_SETUP_DEF,
	LVL_SETUP_PRM,
	LVL_SETUP_PRD,		// 15
	LVL_SETUP_GLB,		
	LVL_INFO = 19,		
	LVL_TEST,			// 20
	LVL_JRN_VIEW,
	LVL_CONTROL
};

// *****************************************************************************
// Меню / Управление

/// Максимально возможное кол-во пунктов в пункте Меню / Управление
#define dControlItemMax 2

/// 
enum eControl
{
	CTRL_RESET,
	CTRL_START,
	CTRL_STOP
};

/// Структура эелементов 
struct strControl
{
	/// номер действия для подтверждения действия
	/// 
	eControl		eActionNumber[dControlItemMax];
	/// кол-во элементов в данный момент
	uint8_t			uItemQuantity;
	/// массив указателей на имя элемента
	uint8_t	__flash *fuName[dControlItemMax];
	/// массив номеров элементов
	uint8_t 		uNumber[dControlItemMax];		
};




// Меню / Управление
// *****************************************************************************

#endif