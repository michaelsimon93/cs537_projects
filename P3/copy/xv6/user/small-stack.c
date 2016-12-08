#include "types.h"
#include "user.h"
#include "stat.h"


int main(int argc, char* argv[]) {
	
	char a = 'a';
	char b = 'b';
	char c = 'c';
	printf(1, "allocating three characters on stack\n");
	printf(1, "should be:\ta\tb\tc\noutput:\t\t%c\t%c\t%c\n",a,b,c);
	printf(1, "adresses:\t%x\t%x\t%x\n", &a,&b,&c);
	exit();
}
