/*
 * Microwindows keyboard driver for Compaq iPAQ
 *
 * Copyright (c) 2000, 2003 Century Software Embedded Technologies
 * Written by Jordan Crouse
 */
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "kbd_types.h"
#include "keyboard.h"


#define KEYBOARD "/dev/Mega8-kbd"

static int keyboard_fd;

typedef struct{
	MWKEY mwkey;
	int scancode;
}KeyMap;

static MWKEY scancodes[64];

static KeyMap keymap[] = {
      {MWKEY_KP0,  11}, 
      {MWKEY_KP1,  0x2}, 
      {MWKEY_KP2,  3}, 
      {MWKEY_KP3,  4}, 
      {MWKEY_KP4,  5}, 
      {MWKEY_KP5,  6}, 
      {MWKEY_KP6,  7}, 
      {MWKEY_KP7,  8}, 
      {MWKEY_KP8,  9}, 
      {MWKEY_KP9,  0xa}, 
      {MWKEY_NUMLOCK, 42}, 
      {MWKEY_KP_DIVIDE,	53}, 
      {MWKEY_KP_MULTIPLY,55}, 
      {MWKEY_KP_MINUS,	74}, 
      {MWKEY_KP_PLUS,	78}, 
      {MWKEY_KP_ENTER,	28}, 
      {MWKEY_KP_DEL,	83}, 
};
#if 0
static KeyMap keymap[] = {
      {MWKEY_KP0,  0x1d},
      {MWKEY_KP1,  0x21},
      {MWKEY_KP2,  0x25},
      {MWKEY_KP3,  0x23},
      {MWKEY_KP4,  0x29},
      {MWKEY_KP5,  0x2d},
      {MWKEY_KP6,  0x2b},
      {MWKEY_KP7,  0x31},
      {MWKEY_KP8,  0x35},
      {MWKEY_KP9,  0x33},
      {MWKEY_NUMLOCK,   0x11},
      {MWKEY_KP_DIVIDE, 0x15},
      {MWKEY_KP_MULTIPLY,       0x13},
      {MWKEY_KP_MINUS,  0x0b},
      {MWKEY_KP_PLUS,           0x34},
      {MWKEY_KP_ENTER,  0x24},
      {MWKEY_KP_DEL,    0x1b},
};
#endif
int
KBD_Open(void)
{
	int i;
//	printf("open %d ing add by lyj_uptech\n\n",KEYBOARD);
	/* Open the keyboard and get it ready for use */
	keyboard_fd = open(KEYBOARD, O_RDONLY | O_NONBLOCK);
//	printf("open %d ing add by lyj_uptech ok! keyboard_fd : %d \n\n",KEYBOARD,keyboard_fd);

	if (keyboard_fd < 0) {
		printf("%s - Can't open keyboard!\n", __FUNCTION__);
		return -1;
	}else  
		printf("keyboard is opened\n");

	for (i=0; i<sizeof(scancodes)/sizeof(scancodes[0]); i++)
		scancodes[i]=MWKEY_UNKNOWN;

	for (i=0; i< sizeof(keymap)/sizeof(keymap[0]); i++)
	{	scancodes[keymap[i].scancode]=keymap[i].mwkey;
//		printf("scancodes[%d]\n", keymap[i].scancode);
	}
	return keyboard_fd;
}


void
KBD_Close(void)
{
	close(keyboard_fd);
	keyboard_fd = -1;
}

void
KBD_GetModifierInfo(MWKEYMOD * modifiers, MWKEYMOD * curmodifiers)
{
	if (modifiers)
		*modifiers = 0;	/* no modifiers available */
	if (curmodifiers)
		*curmodifiers = 0;
}


int
KBD_Read(char* kbuf, MWKEYMOD * modifiers, MWSCANCODE * scancode)
{
	int keydown = 0;
	int cc = 0;
	char buf,key;
	cc = read(keyboard_fd, &buf, 1);
//	printf("cc = %d\n",cc);
	if (cc < 0) {
		if ((errno != EINTR) && (errno != EAGAIN)
		    && (errno != EINVAL)) {
			perror("KBD KEY");
			return (-1);
		} else {
			return (0);
		}
	}
//	printf("KBD_Read begin\n");
	if (cc == 0)
		return (0);

	/* If the code is big than 127, then we know that */
	/* we have a key down.  Figure out what we've got */

	*modifiers = 0;

	if (buf & 0x80) {
		keydown = 1;	/* Key pressed but not released */
	} else {
		keydown = 2;	/* key released */
	}

	buf &= (~0x80);
	if( buf >= sizeof(scancodes) ) *kbuf = MWKEY_UNKNOWN;
	*scancode = scancodes[(int) buf];
      //  printf("scancodes[%d]\n",(int) buf);
	*kbuf = *scancode ;
//	printf("%c",*kbuf);  	
//	printf("by threewater: orgvalue=%d  key=%d  keystatus=%d, scancode=%d\n",buf, *kbuf,keydown, *scancode);
	return keydown;
	
}
