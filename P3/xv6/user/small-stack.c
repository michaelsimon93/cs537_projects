#include "types.h"
#include "user.h"
#include "stat.h"


int main(int argc, char* argv[]) {
	
	char a = 'a';
	char b = 'b';
	char c = 'c';

	write(1, &a, 1);
	write(1, "\n", 1);
	write(1, &b, 1);
	write(1, "\n", 1);
	write(1, &c, 1);
	write(1, "\n", 1);

	exit();
}
