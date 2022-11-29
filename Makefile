TARGET    := solution.exe

LOCAL_INC := -I./

FLAGS     := 

CFLAGS    := $(FLAGS) $(LOCAL_INC)

VPATH     := ./

OBJS      := solution.o filter.o

CC        := gcc -Wall -O0 -g0 -Wconversion

BUILD     := $(OBJS) $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

clean :
	del /F $(TARGET)
	del /F *.o
	echo clean done

all : $(TARGET)
