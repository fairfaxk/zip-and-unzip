DEBUG = -g
CFLAGS = ${DEBUG} -Wall

EXEC1 = zip
OBJS1 = zip.o

EXEC2 = unzip
OBJS2 = unzip.o

all: ${EXEC1} ${EXEC2}

${EXEC1}: ${OBJS1}
	g++ ${CFLAGS} -o ${EXEC1} ${OBJS1}

${OBJS1}: %.o: %.cpp
	g++ ${CFLAGS} -c -o $@ $<

${EXEC2}: ${OBJS2}
	g++ ${CFLAGS} -o ${EXEC2} ${OBJS2}

${OBJS2}: %.o: %.cpp
	g++ ${CFLAGS} -c -o $@ $<

clean:
	rm -f ${EXEC1} ${OBJS1} ${EXEC2} ${OBJS2}
