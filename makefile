EXEC = r
DIR_CURR=./
DIR_OBJ=./OBJ/

C_SRCS  =$(wildcard ${DIR_CURR}*.c)
C_SRCS +=$(wildcard ./conv/*.c) $(wildcard ./ipc/*.c) $(wildcard ./request/*.c)
C_SRCS +=$(wildcard ./record/*.c)
		
C_OBJS = ${patsubst %.c,${DIR_OBJ}%.o, ${notdir ${C_SRCS}}}
C_SRCS_DIR = ${dir $(C_SRCS)}

# delete the repeatition
uniq 		= $(eval seen :=)$(foreach i,$1,$(if $(filter $i,${seen}),,$(eval seen += $i)))${seen}
rwildcard	= $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
ALL_DIRS 	= $(dir $(foreach src_path,$(DIR_CURR), $(call rwildcard,$(src_path),*/) ) )
ALL_DIRS 	:= $(call uniq, $(ALL_DIRS))
INC_DIR 	:= $(ALL_DIRS)
# libraries
RTMP_LIB_DIR = ./librtmp/
EH_LIB_DIR 	 = ./HIK_EHome/eh_lib/
LIBS_RTMP	 = rtmp
LIBS_EHOME	 = HCEHomeCMS HCEHomeStream SystemTransform hpr

LIB_DIR 	:= $(RTMP_LIB_DIR) $(EH_LIB_DIR)
LIBS 		:= $(LIBS_RTMP) $(LIBS_EHOME)

CC = g++
CFLAGS = -O2 -Wall -g
INCFLAGS = $(foreach i, $(INC_DIR),-I$(i))
LDFLAGS += $(foreach i, $(LIB_DIR),-L$(i))
LDFLAGS += $(foreach i, $(LIBS),-l$(i))
DY_LDFLAGS =-Wl,-rpath=$(EH_LIB_DIR)

all:clean $(EXEC)

$(EXEC):$(C_OBJS)
	cd ./librtmp && make
	$(CC) -o $@ $(C_OBJS) $(LDFLAGS)
	@echo "Make Done~"

VPATH = $(C_SRCS_DIR)
${DIR_OBJ}%.o:${DIR_CURR}%.c	
	@if [ ! -d $(DIR_OBJ) ]; then mkdir -p $(DIR_OBJ); fi; 
	@$(CC) -c $< -o $@ $(CFLAGS) $(INCFLAGS)

.PHONY:clean
clean:
	@rm -rf ${DIR_OBJ}
	@rm -rf ${EXEC}
	@echo "Clean Done~"
