#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#define GPIO_FUNC_SEL 0x20200000
#define GPIO_LEN  0xB4
#define GPSET0 7
#define GPCLR0 10
#define GPLEV0 13

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

static uint32_t *gpioReg = (uint32_t*)GPIO_FUNC_SEL;

char * err_out;

int main()
{
	int fd;
	
	printf("Starting main program.\n");

	fd = open("/dev/mem", O_RDWR | O_SYNC);

	if (fd < 0)
	{
		fprintf(stderr, "This program needs root privileges.\n");
		return -1;
	}
	

	gpioReg = mmap(0,
			BLOCK_SIZE,
			PROT_READ|PROT_WRITE,
			MAP_SHARED,
			fd,
			GPIO_FUNC_SEL);

	close(fd);

	if (gpioReg == MAP_FAILED)
	{
		fprintf(stderr, "mmap failed");
		return -1;
	}	

	gpioReg[0] = 0x40;

	printf("Output Settings: %d", gpioReg[0]);
	*(gpioReg + GPSET0) = 0xFFFFFFFF;

	sleep(1);

	*(gpioReg + GPCLR0) = 0xFFFFFFFF;

	sleep(1);

	int regVal = *(gpioReg + GPLEV0);
	printf("Register value: %d", regVal);
  	return 0;
}


