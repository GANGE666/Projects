/*
	define all key scancode for net-arm3000 keyboard
	created by Zou jian guo <ah_zou@163.com>
	2004-4-5

*/

#ifndef _KBD_TYPES_H
#define _KBD_TYPES_H

/* Keyboard values*/
typedef unsigned short	MWKEY;
typedef unsigned short	MWSCANCODE;

/* Keyboard state modifiers*/
typedef unsigned int	MWKEYMOD;


#define MWKEY_UNKNOWN		0

#define MWKEY_NONASCII_MASK	0xFF00


/* Numeric keypad*/
#define MWKEY_KP0		'0'
#define MWKEY_KP1		'1'
#define MWKEY_KP2		'2'
#define MWKEY_KP3		'3'
#define MWKEY_KP4		'4'
#define MWKEY_KP5		'5'
#define MWKEY_KP6		'6'
#define MWKEY_KP7		'7'
#define MWKEY_KP8		'8'
#define MWKEY_KP9		'9'
#define MWKEY_KP_DEL		0x7F
#define MWKEY_KP_DIVIDE		'/'
#define MWKEY_KP_MULTIPLY	'*'
#define MWKEY_KP_MINUS		'-'
#define MWKEY_KP_PLUS		'+'
#define MWKEY_KP_ENTER		13
#define MWKEY_KP_EQUALS		61
#define MWKEY_KP_NUMLOCK	0XFF

#define MWKEY_C_DEL			'.' //0x7F
#define MWKEY_C_DIVIDE		'/'
#define MWKEY_C_MULTIPLY	'*'
#define MWKEY_C_MINUS		'-'
#define MWKEY_C_PLUS		'+'
#define MWKEY_C_ENTER		13
#define MWKEY_C_EQUALS		61
#define MWKEY_C_NUMLOCK		0XFF


/* Following keysyms are mapped to private use portion of Unicode-16*/
/* arrows + home/end pad*/
#define MWKEY_FIRST		MWKEY_KP4
#define MWKEY_LEFT		MWKEY_KP4
#define MWKEY_RIGHT		MWKEY_KP6
#define MWKEY_UP		MWKEY_KP8
#define MWKEY_DOWN		MWKEY_KP2
#define MWKEY_INSERT	MWKEY_KP0
#define MWKEY_DELETE	MWKEY_KP_DEL
#define MWKEY_HOME		MWKEY_KP7
#define MWKEY_END		MWKEY_KP1
#define MWKEY_PAGEUP	MWKEY_KP9
#define MWKEY_PAGEDOWN	MWKEY_KP3
#define MWKEY_NUMLOCK	MWKEY_KP_NUMLOCK


/*int  (*Open)(struct _kbddevice *pkd);*/
/* Interface to Keyboard Device Driver*/
typedef struct _kbddevice {
	int  (*Open)(void);
	void (*Close)(void);
	void (*GetModifierInfo)(MWKEYMOD *modifiers, MWKEYMOD *curmodifiers);
	int  (*Read)(MWKEY *buf,MWKEYMOD *modifiers,MWSCANCODE *scancode);
	int  (*Poll)(void);		/* not required if have select()*/
} KBDDEVICE;


#endif
