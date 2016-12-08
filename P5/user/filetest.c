#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
  int fd = open("tmp", O_CREATE | O_CHECKED);
  printf(fd, "testline\ntestline2\n");
  close(fd);

  printf(1, "%d\n", fd);
  exit();
}
