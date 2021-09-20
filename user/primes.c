#include "kernel/types.h"
#include "user/user.h"

void print_prime(int p[2]) {
  close(p[1]); // don't need to write to left neighbor
  int prime;
  int len = read(p[0], &prime, sizeof(prime));
  if (len == 0) {
    close(p[0]);
    exit(0);
  }
  printf("prime %d\n", prime);

  int pp[2];
  pipe(pp);
  if (fork() == 0) {
    close(p[0]);
    print_prime(pp);
  } else {
    close(pp[0]);
    while (1) {
      int num;
      len = read(p[0], &num, sizeof(num));
      if (len == 0) {
        break;
      }
      if (num % prime != 0) {
        write(pp[1], &num, sizeof(num));
      }
    }
    close(p[0]);
    close(pp[1]);
    wait(0);
  }
  exit(0);
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    fprintf(2, "too mush arguments");
    exit(0);
  }

  int p[2];
  pipe(p);

  if (fork() == 0) {
    print_prime(p);
  } else {
    close(p[0]);
    for (int i = 2; i <= 35; i++) {
      write(p[1], &i, sizeof(i));
    }
    close(p[1]);
    wait(0); // wait until the entire pipeline terminates
  }
  exit(0);
}
