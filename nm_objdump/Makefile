CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lelf

TARGET1 = hnm
TARGET2 = hobjdump

SRCS1 = hnm.c
SRCS2 = hobjdump.c

OBJS1 = $(SRCS1:.c=.o)
OBJS2 = $(SRCS2:.c=.o)

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJS1)
	$(CC) $(CFLAGS) -o $(TARGET1) $(OBJS1) $(LDFLAGS)

$(TARGET2): $(OBJS2)
	$(CC) $(CFLAGS) -o $(TARGET2) $(OBJS2) $(LDFLAGS)

clean:
	rm -f $(OBJS1) $(TARGET1) $(OBJS2) $(TARGET2)
