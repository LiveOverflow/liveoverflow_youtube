CC=gcc -std=c99

all: functions32 functions64 variables32 variables64 control_flow example

functions32:
	$(CC) functions.c -m32 -o functions32

functions64:
	$(CC) functions.c -o functions64

variables32:
	$(CC) variables.c -m32 -o variables32

variables64:
	$(CC) variables.c -o variables64

control_flow:
	$(CC) control_flow.c -o control_flow

example:
	$(CC) example.c -o example

clean:
	rm functions32 functions64 variables32 variables64 control_flow example

