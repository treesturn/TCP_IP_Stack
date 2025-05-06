CC=gcc
CFLAGS=-g 
TARGET:test.exe 
LIBS=-lpthread -L ./CommandParser -lcli -lrt
OBJS=gluethread/glthread.o \
		  graph.o  \
		  topologies.o \
		  net.o	\
	          utils.o \
		  nwcli.o  

test.exe:testapp.o ${OBJS} CommandParser/libcli.a
	${CC} ${CFLAGS} testapp.o ${OBJS} -o test.exe ${LIBS}

testapp.o:testapp.c
	${CC} ${CFLAGS} -c testapp.c -o testapp.o

gluethread/glthread.o:gluethread/glthread.c
	${CC} ${CFLAGS} -c -I gluethread gluethread/glthread.c -o gluethread/glthread.o

graph.o:graph.c
	${CC} ${CFLAGS} -c -I . graph.c -o graph.o

topologies.o:topologies.c
	${CC} ${CFLAGS} -c -I . topologies.c -o topologies.o

net.o:net.c
	${CC} ${CFLAGS} -c -I . net.c -o net.o

utils.o:utils.c
	${CC} ${CFLAGS} -c -I . utils.c -o utils.o

nwcli.o:nwcli.c
	${CC} ${CFLAGS} -c -I . nwcli.c -o nwcli.o

CommandParser/libcli.a:
	(cd CommandParser; make)

clean:
	rm -f *.o
	rm -f gluethread/glthread.o
	rm -f *exe
		
all:
	make
	(cd CommandParser; make)

cleanall:
	make clean
	(cd CommandParser; make clean)
