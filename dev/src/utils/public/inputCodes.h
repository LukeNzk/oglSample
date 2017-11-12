#pragma once
#include "types.h"

enum class ERIEventType : Uint8
{
	RIE_UP,
	RIE_DOWN,
	RIE_MOUSEMOVE,
	RIE_UNKNOWN
};

struct SInputInfo
{
	ERIEventType eventType;
	Int32 x, y;
};

#define RI_LBUTTON        0x01
#define RI_RBUTTON        0x02
#define RI_CANCEL         0x03
#define RI_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

#define RI_BACK           0x08
#define RI_TAB            0x09

/*
* 0x0A - 0x0B : reserved
*/

#define RI_CLEAR          0x0C
#define RI_RETURN         0x0D

#define RI_CLOSE		  0x0E

/*
* 0x0F : unassigned
*/

#define RI_SHIFT          0x10
#define RI_CONTROL        0x11
#define RI_ALT			  0x12
#define RI_PAUSE          0x13
#define RI_CAPITAL        0x14

/*
* 0x16 : unassigned
*/

/*
* 0x1A : unassigned
*/

#define RI_ESCAPE         0x1B

#define RI_CONVERT        0x1C
#define RI_NONCONVERT     0x1D
#define RI_ACCEPT         0x1E
#define RI_MODECHANGE     0x1F

#define RI_SPACE          0x20
#define RI_PRIOR          0x21
#define RI_NEXT           0x22
#define RI_END            0x23
#define RI_HOME           0x24
#define RI_LEFT           0x25
#define RI_UP             0x26
#define RI_RIGHT          0x27
#define RI_DOWN           0x28
#define RI_SELECT         0x29
#define RI_PRINT          0x2A
#define RI_EXECUTE        0x2B
#define RI_SNAPSHOT       0x2C
#define RI_INSERT         0x2D
#define RI_DELETE         0x2E
#define RI_HELP           0x2F

/*
* RI_0 - RI_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
* 0x3A - 0x40 : unassigned
* RI_A - RI_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
*/

#define RI_A			  0x41
#define RI_D			  0x44
#define RI_S			  0x53
#define RI_W			  0x57
#define RI_X			  0x58

#define RI_LWIN           0x5B
#define RI_RWIN           0x5C
#define RI_APPS           0x5D

/*
* 0x5E : reserved
*/

#define RI_SLEEP          0x5F

#define RI_NUMPAD0        0x60
#define RI_NUMPAD1        0x61
#define RI_NUMPAD2        0x62
#define RI_NUMPAD3        0x63
#define RI_NUMPAD4        0x64
#define RI_NUMPAD5        0x65
#define RI_NUMPAD6        0x66
#define RI_NUMPAD7        0x67
#define RI_NUMPAD8        0x68
#define RI_NUMPAD9        0x69
#define RI_MULTIPLY       0x6A
#define RI_ADD            0x6B
#define RI_SEPARATOR      0x6C
#define RI_SUBTRACT       0x6D
#define RI_DECIMAL        0x6E
#define RI_DIVIDE         0x6F
#define RI_F1             0x70
#define RI_F2             0x71
#define RI_F3             0x72
#define RI_F4             0x73
#define RI_F5             0x74
#define RI_F6             0x75
#define RI_F7             0x76
#define RI_F8             0x77
#define RI_F9             0x78
#define RI_F10            0x79
#define RI_F11            0x7A
#define RI_F12            0x7B
#define RI_F13            0x7C
#define RI_F14            0x7D
#define RI_F15            0x7E
#define RI_F16            0x7F
#define RI_F17            0x80
#define RI_F18            0x81
#define RI_F19            0x82
#define RI_F20            0x83
#define RI_F21            0x84
#define RI_F22            0x85
#define RI_F23            0x86
#define RI_F24            0x87


#define RI_NUMLOCK        0x90
#define RI_SCROLL         0x91

/*
* 0x97 - 0x9F : unassigned
*/

/*
* RI_L* & RI_R* - left and right Alt, Ctrl and Shift virtual keys.
* Used only as parameters to GetAsyncKeyState() and GetKeyState().
* No other API or message will distinguish left and right keys in this way.
*/
//#define RI_LSHIFT         0xA0
//#define RI_RSHIFT         0xA1
//#define RI_LCONTROL       0xA2
//#define RI_RCONTROL       0xA3
//#define RI_LMENU          0xA4
//#define RI_RMENU          0xA5

/*
* 0xE8 : unassigned
*/

#define RI_LMB			  0x100
#define RI_RMB			  0x101

/*
* 0xFF : reserved
*/