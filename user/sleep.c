#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    fprintf(2, "sleep: missing operand");
  }
  int duration = atoi(argv[1]);
  sleep(duration);
  exit(0);
}
