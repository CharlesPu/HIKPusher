EXEC = r
DIR_CURR=./
DIR_OBJ=./OBJ
RTMP_LIB_DIR = ./librtmp
EH_INC_DIR = ./eh_cn/
EH_LIB_DIR = ./eh_lib/

C_SRC  =${wildcard ${DIR_CURR}/*.c}
		
C_OBJS = ${patsubst %.c, ${DIR_OBJ}/%.o, ${notdir ${C_SRC}}}

CC = g++
CFLAGS = -O2 -Wall -I${RTMP_LIB_DIR} -I${EH_INC_DIR}
LDFLAGS = -L${RTMP_LIB_DIR} -lrtmp -L${EH_LIB_DIR} \
           -lhpr -lHCEHomeAlarm -lHCEHomeCMS -lHCEHomeStream -liconv2 -lSystemTransform
LD_DY_LIB =-Wl,--rpath=${EH_LIB_DIR}:${RTMP_LIB_DIR} 
            

all:$(EXEC)

$(EXEC):$(C_OBJS)
	cd ./librtmp && make	
	$(CC) -o $@ $(C_OBJS) $(LDFLAGS) $(LD_DY_LIB)
	@echo "Make Done~"
	
${DIR_OBJ}/%.o:${DIR_CURR}/%.c	
	@if [ ! -d $(DIR_OBJ) ]; then mkdir -p $(DIR_OBJ); fi; 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf ${DIR_OBJ}/*.o
	@rm -rf ${DIR_OBJ}
	@rm -rf ${EXEC}
	@echo "Clean Done~"