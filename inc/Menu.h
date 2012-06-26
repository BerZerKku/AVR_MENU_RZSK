#ifndef __MENU_H
#define __MENU_H

#include <stdint.h>

/// ������ ������� ����
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
// ���� / ����������

/// ����������� ��������� ���-�� ������� � ������ ���� / ����������
#define dControlItemMax 2

/// 
enum eControl
{
	CTRL_RESET,
	CTRL_START,
	CTRL_STOP
};

/// ��������� ���������� 
struct strControl
{
	/// ����� �������� ��� ������������� ��������
	/// 
	eControl		eActionNumber[dControlItemMax];
	/// ���-�� ��������� � ������ ������
	uint8_t			uItemQuantity;
	/// ������ ���������� �� ��� ��������
	uint8_t	__flash *fuName[dControlItemMax];
	/// ������ ������� ���������
	uint8_t 		uNumber[dControlItemMax];		
};




// ���� / ����������
// *****************************************************************************

#endif