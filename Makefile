CC = gcc
MD = mkdir
RM = rm
CP = cp
COMP_VER = 0.0.1
SRC_PATH = ./src
INC_PATH = ./inc
X_VERSION = '1.0.1'

TARGET = frd
CFLAGS  = -Wall -I$(INC_PATH) 
CFLAGS += -Wunused-variable
CFLAGS += -Wunused-function
CFLAGS += -Wno-char-subscripts
CFLAGS += -D_GNU_SOURCE
LDFLAGS  = -lpthread
LDFLAGS += -lpcap
MD = mkdir -p

src = $(wildcard *.c) $(wildcard $(SRC_PATH)/*.c) 
obj = $(src:.c=.o)
dep = $(obj:.o=.d)  # one dependency file for each source

$(TARGET): $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)
%.d: %.c
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@
.PHONY: clean
clean:
	$(RM) -f $(obj) $(TARGET) 

