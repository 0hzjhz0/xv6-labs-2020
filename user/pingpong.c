#include "kernel/types.h"
#include "user/user.h"

int main() {
  int p1[2]; // p -> c
  int p2[2]; // c -> p

  pipe(p1);
  pipe(p2);

  if (fork() > 0) {
    // parent
    close(p1[0]);

    write(p1[1], "ping", 4);

    char buf[4];
    read(p2[0], buf, 4);
    printf("%d: received %s\n", getpid(), buf);

    close(p2[0]);
    close(p1[1]);
  } else {
    close(p1[1]);

    char buf[4];
    read(p1[0], buf, 4);
    printf("%d: received %s\n", getpid(), buf);

    write(p2[1], "pong", 4);

    close(p1[0]);
    close(p2[1]);
  }

  exit(0);
}
