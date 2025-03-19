CC = gcc
CFLAGS = -Wall -Wextra -O2

SRCDIR = src
INCDIR = include
OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

TARGET = libfastlog.a

all: $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(TARGET): $(OBJECTS)
	ar rcs $(TARGET) $(OBJECTS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)

install:
	cp $(TARGET) /usr/local/lib/
	cp include/fastlog.h /usr/local/include/

uninstall:
	rm -f /usr/local/lib/$(TARGET)
	rm -f /usr/local/include/fastlog.h
