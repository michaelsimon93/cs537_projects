#include "types.h"
#include "user.h"
#include "stat.h"


int main(int argc, char* argv[]) {

	int *p = (int*)0xa0000 - 0x2000;
	printf(1, "this should not print:%d\n",*p);
	exit();
}
