/*
 * Copyright (C) 2006 Josef Cejka
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <kbd.h>
#include <fb.h>
#include <ipc/ipc.h>
#include <ipc/fb.h>
#include <ipc/services.h>
#include <errno.h>
#include <key_buffer.h>
#include <console.h>
#include <unistd.h>
#include <async.h>
#include <libadt/fifo.h>
#include <screenbuffer.h>

#define CONSOLE_COUNT 12 
#define MAX_KEYREQUESTS_BUFFERED 32

#define NAME "CONSOLE"

int active_console = 1;

struct {
	int phone;		/**< Framebuffer phone */
	ipcarg_t rows;		/**< Framebuffer rows */
	ipcarg_t cols;		/**< Framebuffer columns */
} fb_info;

typedef struct {
	keybuffer_t keybuffer;
	FIFO_CREATE_STATIC(keyrequests, ipc_callid_t , MAX_KEYREQUESTS_BUFFERED);
	int keyrequest_counter;
	int client_phone;
	int used;
	screenbuffer_t screenbuffer;
} connection_t;



connection_t connections[CONSOLE_COUNT];

static int find_free_connection() 
{
	int i = 0;
	
	while (i < CONSOLE_COUNT) {
		if (connections[i].used == 0)
			return i;
		++i;
	}
	return CONSOLE_COUNT;
}


static int find_connection(int client_phone) 
{
	int i = 0;
	
	while (i < CONSOLE_COUNT) {
		if (connections[i].client_phone == client_phone)
			return i;
		++i;
	}
	return  CONSOLE_COUNT;
}

/** Check key and process special keys. 
 *
 * */
static void write_char(int console, char key)
{
	screenbuffer_t *scr = &(connections[console].screenbuffer);
	
	switch (key) {
		case '\n':
			scr->position_y += 1;
			scr->position_x =  0;
			break;
		case '\r':
			break;
		case '\t':
			scr->position_x += 8;
			scr->position_x -= scr->position_x % 8; 
			break;
		case '\b':
			if (scr->position_x == 0) 
				break;

			scr->position_x--;

			if (console == active_console) {
				ipc_call_async_3(fb_info.phone, FB_PUTCHAR, ' ', scr->position_y, scr->position_x, NULL, NULL);
			}
	
			screenbuffer_putchar(scr, ' ');
			
			break;
		default:	
			if (console == active_console) {
				ipc_call_async_3(fb_info.phone, FB_PUTCHAR, key, scr->position_y, scr->position_x, NULL, NULL);
			}
	
			screenbuffer_putchar(scr, key);
			scr->position_x++;
	}
	
	scr->position_y += (scr->position_x >= scr->size_x);
	
	if (scr->position_y >= scr->size_y) {
		scr->position_y = scr->size_y - 1;
		screenbuffer_clear_line(scr, scr->top_line++);
		ipc_call_async(fb_info.phone, FB_SCROLL, 1, NULL, NULL);
	}
	
	scr->position_x = scr->position_x % scr->size_x;
	
	if (console == active_console)	
		ipc_call_async_2(fb_info.phone, FB_CURSOR_GOTO, scr->position_y, scr->position_x, NULL, NULL);
	
}


/* Handler for keyboard */
static void keyboard_events(ipc_callid_t iid, ipc_call_t *icall)
{
	ipc_callid_t callid;
	ipc_call_t call;
	int retval;
	int i, j;
	char c,d;
	connection_t *conn;
	keyfield_t *interbuffer = NULL;

//	interbuffer = mmap(,, PROTO_READ|PROTO_WRITE, MAP_ANONYMOUS, , );	
	
	/* Ignore parameters, the connection is alread opened */
	while (1) {
		callid = async_get_call(&call);
		switch (IPC_GET_METHOD(call)) {
		case IPC_M_PHONE_HUNGUP:
			ipc_answer_fast(callid,0,0,0);
			/* TODO: Handle hangup */
			return;
		case KBD_PUSHCHAR:
			/* got key from keyboard driver */
			
			retval = 0;
			c = IPC_GET_ARG1(call);
			/* switch to another virtual console */
			
			conn = &connections[active_console];
//			if ((c >= KBD_KEY_F1) && (c < KBD_KEY_F1 + CONSOLE_COUNT)) {
			if ((c >= '1') && (c < '1' + CONSOLE_COUNT)) {
				/*FIXME: draw another console content from buffer */
				if (c - '1' == active_console)
						break;
				active_console = c - '1';
				conn = &connections[active_console];

				ipc_call_async(fb_info.phone, FB_CURSOR_VISIBILITY, 0, NULL, NULL); 
				ipc_call_async_2(fb_info.phone, FB_CLEAR, 0, 0, NULL, NULL);
				
				for (i = 0; i < conn->screenbuffer.size_x; i++)
					for (j = 0; j < conn->screenbuffer.size_y; j++) {
						d = get_field_at(&(conn->screenbuffer),i, j)->character;
						if (d && d != ' ')
							ipc_call_async_3(fb_info.phone, FB_PUTCHAR, d, j, i, NULL, NULL);
					}

				ipc_call_async_2(fb_info.phone, FB_CURSOR_GOTO, conn->screenbuffer.position_y, conn->screenbuffer.position_x, NULL, NULL); 
				ipc_call_async(fb_info.phone, FB_CURSOR_VISIBILITY, 1, NULL, NULL); 

				break;
			}
			
			/* if client is awaiting key, send it */
			if (conn->keyrequest_counter > 0) {		
				conn->keyrequest_counter--;
				ipc_answer_fast(fifo_pop(conn->keyrequests), 0, c, 0);
				break;
			}
			
			/*FIXME: else store key to its buffer */
			keybuffer_push(&conn->keybuffer, c);
			
			break;
		default:
			retval = ENOENT;
		}		
		ipc_answer_fast(callid, retval, 0, 0);
	}
}

/** Default thread for new connections */
void client_connection(ipc_callid_t iid, ipc_call_t *icall)
{
	ipc_callid_t callid;
	ipc_call_t call;
	int consnum;
	ipcarg_t arg1;

	if ((consnum = find_free_connection()) == CONSOLE_COUNT) {
		ipc_answer_fast(iid,ELIMIT,0,0);
		return;
	}

	connections[consnum].used = 1;
	connections[consnum].client_phone = IPC_GET_ARG3(call);
	screenbuffer_clear(&(connections[consnum].screenbuffer));
	
	/* Accept the connection */
	ipc_answer_fast(iid,0,0,0);
	
	while (1) {
		callid = async_get_call(&call);
		switch (IPC_GET_METHOD(call)) {
		case IPC_M_PHONE_HUNGUP:
			/* TODO */
			ipc_answer_fast(callid, 0,0,0);
			return;
		case CONSOLE_PUTCHAR:
			write_char(consnum, IPC_GET_ARG1(call));
			break;
		case CONSOLE_CLEAR:
			/* Send message to fb */
			if (consnum == active_console) {
				ipc_call_async_2(fb_info.phone, FB_CLEAR, 0, 0, NULL, NULL); 
			}
			
			screenbuffer_clear(&(connections[consnum].screenbuffer));
			
			break;
		case CONSOLE_GOTO:
			
			screenbuffer_goto(&(connections[consnum].screenbuffer), IPC_GET_ARG1(call), IPC_GET_ARG2(call));
			
			break;

		case CONSOLE_GETCHAR:
			if (keybuffer_empty(&(connections[consnum].keybuffer))) {
				/* buffer is empty -> store request */
				if (connections[consnum].keyrequest_counter < MAX_KEYREQUESTS_BUFFERED) {		
					fifo_push(connections[consnum].keyrequests, callid);
					connections[consnum].keyrequest_counter++;
				} else {
					/* no key available and too many requests => fail */
					ipc_answer_fast(callid, ELIMIT, 0, 0);
				}
				continue;
			};
			keybuffer_pop(&(connections[consnum].keybuffer), (char *)&arg1);
			
			break;
		}
		ipc_answer_fast(callid, 0, arg1, 0);
	}
}

int main(int argc, char *argv[])
{
	ipcarg_t phonehash;
	int kbd_phone, fb_phone;
	ipcarg_t retval, arg1 = 0xdead, arg2 = 0xbeef;
	int i;

	async_set_client_connection(client_connection);
	
	/* Connect to keyboard driver */

	while ((kbd_phone = ipc_connect_me_to(PHONE_NS, SERVICE_KEYBOARD, 0)) < 0) {
		usleep(10000);
	};
	
	if (ipc_connect_to_me(kbd_phone, SERVICE_CONSOLE, 0, &phonehash) != 0) {
		return -1;
	};

	/* Connect to framebuffer driver */
	
	while ((fb_info.phone = ipc_connect_me_to(PHONE_NS, SERVICE_VIDEO, 0)) < 0) {
		usleep(10000);
	}
	
	ipc_call_sync_2(fb_info.phone, FB_GET_CSIZE, 0, 0, &(fb_info.rows), &(fb_info.cols)); 
	ipc_call_sync(fb_info.phone, FB_CURSOR_VISIBILITY, 1, NULL); 
	
	/* Init virtual consoles */
	for (i = 0; i < CONSOLE_COUNT; i++) {
		connections[i].used = 0;
		keybuffer_init(&(connections[i].keybuffer));
		
		connections[i].keyrequests.head = connections[i].keyrequests.tail = 0;
		connections[i].keyrequests.items = MAX_KEYREQUESTS_BUFFERED;
		connections[i].keyrequest_counter = 0;
		
		if (screenbuffer_init(&(connections[i].screenbuffer), fb_info.cols, fb_info.rows ) == NULL) {
			/*FIXME: handle error */
			return -1;
		}
	}
	
	async_new_connection(phonehash, 0, NULL, keyboard_events);
	
	ipc_call_async_2(fb_info.phone, FB_CURSOR_GOTO, 0, 0, NULL, NULL); 

	if (ipc_connect_to_me(PHONE_NS, SERVICE_CONSOLE, 0, &phonehash) != 0) {
		return -1;
	};
	
	async_manager();

	return 0;	
}
