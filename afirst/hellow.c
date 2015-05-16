#include <stdio.h>
#include <dlfcn.h>

int main() {
	void *so = dlopen("libafirst.so", 0);
	printf("dlopen: %p\n", so);
	char *err = dlerror();
	printf("dlerror: %s\n", err);
	return 0;
}
