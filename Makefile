CC=mpicc
C_FLAGS=-Wall  -g
L_FLAGS=-lm -lrt
TARGET=prlab4
FILES=main.o
SOURCE=main.c
HOSTFILE=hosts
N=4
K=3
MAP_FILE=./mapa

${TARGET}: ${FILES}
	${CC} -o ${TARGET} ${FILES} ${L_FLAGS}

${FILES}: ${SOURCE}
	${CC} -o ${FILES} -c ${SOURCE} ${C_FLAGS}

.PHONY: clean run

run:
	mpirun --hostfile ${HOSTFILE} ./${TARGET} ${N} ${K} ${MAP_FILE}
	
clean:
	-rm -f ${FILES} ${TARGET}
 
