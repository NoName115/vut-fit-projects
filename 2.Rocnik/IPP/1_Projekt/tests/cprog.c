#include <stdio.h>

// test program

int main() {
	
	int a = 0x10;
	float b = 20.34;
	char c;
	
	if (a < b && b > 10) {
		printf("A!\n");
	} else {
		printf("B!\n");
	}

	while(a < b) {
		a++;
	}

	return 0;
}

