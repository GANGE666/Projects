/*
	define all key scancode for net-arm3000 keyboard
	created by Zou jian guo <ah_zou@163.com>
	2004-4-5

*/

#ifndef _KBD_H
#define _KBD_H
#include "kbd_types.h"


int  KBD_Open(void);
void KBD_Close(void);
void KBD_GetModifierInfo(MWKEYMOD *modifiers, MWKEYMOD *curmodifiers);
int  KBD_Read(char *kbuf, MWKEYMOD *modifiers, MWSCANCODE *scancode);

#endif
