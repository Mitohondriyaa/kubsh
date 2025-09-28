#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <signal.h>

extern volatile sig_atomic_t sighup_received;

void handle_sighup(int sig);

#endif