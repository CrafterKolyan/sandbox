#include <unistd.h>
#include <sys/syscall.h>

const char message[] = "Hello CrafterKolyan!\n";

int main() {
  syscall(SYS_write, STDOUT_FILENO, message, sizeof(message) - 1);
  return 0;
}
