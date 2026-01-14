#include <sys/stat.h>
#include <errno.h>

int _write(int file, char *ptr, int len) {
    // bijvoorbeeld: stuur naar UART
    for (int i = 0; i < len; i++) {
        // HAL_UART_Transmit(&huart1, (uint8_t*)&ptr[i], 1, 10);
    }
    return len;
}

int _read(int file, char *ptr, int len) {
    // eventueel: lees van UART, hier dummy
    return 0;
}

int _close(int file) { return -1; }
int _fstat(int file, struct stat *st) { st->st_mode = S_IFCHR; return 0; }
int _isatty(int file) { return 1; }
int _lseek(int file, int ptr, int dir) { return 0; }
int _getpid(void) { return 1; }
int _kill(int pid, int sig) { return -1; }
