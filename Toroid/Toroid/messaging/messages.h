#ifndef _MESSAGES_H
#define _MESSAGES_H

typedef enum message_t
{
	message_redrawGl,	// raised with no data
	message_keyPressed,	// raised with key code

	message_count
} message;

#endif//_MESSAGES_H
