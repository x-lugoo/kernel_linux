ALL: ppoint asm

ppoint:ppoint.o
	gcc ppoint.c -o ppoint  
asm:asm.o
	gcc asm.c -o asm

.PHONY:clean

clean:
	rm *.o ppoint asm
