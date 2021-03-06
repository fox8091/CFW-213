CC=arm-none-eabi-gcc
CP=arm-none-eabi-g++
OC=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump
LD=arm-none-eabi-ld

all:
	$(CC) -g source/*.c source/fatfs/*.c source/libFirmlaunch/*.c -c -march=armv5te -O2 -ffast-math -std=c99 -Wall -Wextra -Wpedantic
	$(CC) -g source/*.s source/fatfs/*.s -c -march=armv5te
	@mkdir -p temp
	@mv start.o temp/start.o
	$(CC) -T 3ds.ld temp/start.o *.o 
	$(OC) --set-section-flags .bss=alloc,load,contents -O binary a.out CFW-213.bin
	@rm -rf *.out
	@rm -rf *.o
	@rm -rf temp/

clean:
	@rm -rf *.out *.o temp/start.o
	@rm -rf libFirmlaunch/*.o
	@rm -rf CFW-213.bin
	@mkdir -p temp
	@rmdir temp
