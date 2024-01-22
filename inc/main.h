#ifndef __frd_MAIN_H__
#define	__frd_MAIN_H__
#define X_THREAD_SIZE_MAX	8
typedef struct {
	int x_running;
	char ifname[IFNAMSIZ];
} frd_main_t;

frd_main_t *frd_main_self(void);
void frd_main_init(void);
void frd_main_free(void);
void frd_main_exit(void);
#endif	//	__frd_MAIN_H__
