#include "common.h"
#include "main.h"
#include "sniff.h"
static frd_main_t self = {0, };
frd_main_t *
frd_main_self(void) {
	return &self;
}
void frd_main_init(void) {
	snprintf(self.ifname, IFNAMSIZ, "br0");
	frd_sniff_init(self.ifname);	// INIT PCAP
	self.x_running = TRUE;
}
void frd_main_free(void) {
	frd_sniff_exit();	// EXIT PCAP
}
void frd_main_exit(void) {
	self.x_running = FALSE;
}
void
frd_sigint_handler(int signo) {
	frd_main_exit();
}
void
frd_sighup_handler(int signo) {
	frd_main_exit();
}
void
frd_main_process(void) {
	pthread_t _thread[X_THREAD_SIZE_MAX] = {0, };   // thread
	int i, index = 0;
	pthread_create(&_thread[index++], NULL, (void *)frd_sniff_process, NULL);
	for(i = index - 1; i >=0; i--) {
		pthread_join(_thread[i], NULL);
	}
}
int
main(int argc, char *argv[]) {
#ifdef HAVE_DAEMON
	pid_t process_id = 0;
	pid_t sid = 0;
	process_id = fork();
	if (process_id > 0)
	{
		exit(0);
	}
	umask(0);
	sid = setsid();
	if(sid < 0)
	{
		// Return failure
		exit(1);
	}
	chdir("/");
	// Close stdin. stdout and stdout
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
#endif
	if (signal(SIGINT, frd_sigint_handler) == SIG_ERR) {
		fprintf(stderr,"[%s] is failed...\n", __func__);
		exit(EXIT_FAILURE);
	}
	if (signal(SIGHUP, frd_sighup_handler) == SIG_ERR) {
		fprintf(stderr,"[%s] is failed...\n", __func__);
		exit(EXIT_FAILURE);
	}
	// START
	frd_main_init();
	// LOOP
	frd_main_process();
	// EXIT
	frd_main_free();
	return 0;
}
