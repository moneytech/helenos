#ifndef _libc_ASYNC_H_
#define _libc_ASYNC_H_

#include <ipc/ipc.h>
#include <psthread.h>
#include <sys/time.h>

typedef ipc_callid_t aid_t;

int async_manager(void);
void async_create_manager(void);
void async_destroy_manager(void);
int _async_init(void);
ipc_callid_t async_get_call(ipc_call_t *data);

aid_t async_send_2(int phoneid, ipcarg_t method, ipcarg_t arg1, ipcarg_t arg2,
		   ipc_call_t *dataptr);
void async_wait_for(aid_t amsgid, ipcarg_t *result);
int async_wait_timeout(aid_t amsgid, ipcarg_t *retval, suseconds_t timeout);
pstid_t async_new_connection(ipcarg_t in_phone_hash,ipc_callid_t callid, 
			     ipc_call_t *call,
			     void (*cthread)(ipc_callid_t,ipc_call_t *));
void async_usleep(suseconds_t timeout);

/* Should be defined by application */
void client_connection(ipc_callid_t callid, ipc_call_t *call) __attribute__((weak));
void interrupt_received(ipc_call_t *call)  __attribute__((weak));

#endif
