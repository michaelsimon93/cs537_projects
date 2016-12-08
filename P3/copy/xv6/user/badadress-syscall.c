#include "types.h"
#include "user.h"
#include "stat.h"


int main(int argc, char* argv[]) {
	
	char *p = (char*)0x10000;
	mkdir(p);
	exit();
}
