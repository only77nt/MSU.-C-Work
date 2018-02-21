all: sock.exe client.exe
sock.exe : sockmain.o func.o
	gcc sockmain.o func.o -o sock.exe
client.exe: client.o func.o
	gcc client.o func.o -o client.exe
func.o: func.c sock.h
	gcc -c func.c
sockmain.o : sockmain.c sock.h
	gcc -c sockmain.c
client.o: client.c sock.h
	gcc -c client.c

