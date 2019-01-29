#include <stdio.h>
#include "db.h"

int main(int argc, char *argv[]) {
  printf ("Database: enter 'x' to exit\n");
  while (execute_command(get_command()));
  return 0;
}
