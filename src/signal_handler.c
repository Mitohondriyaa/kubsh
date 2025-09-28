#include <unistd.h>
#include "signal_handler.h"

volatile sig_atomic_t sighup_received;

void handle_sighup(int sig) {
    char message[] = "\nConfiguration reloaded";

    write(STDOUT_FILENO, message, sizeof(message) - 1);

    sighup_received = 1;
}