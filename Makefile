CC = gcc
CFLAGS = -Wall -Wextra -O2

SRCDIR = src
INCDIR = include
OBJDIR = obj
EXAMPLEDIR = examples

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

TARGET = libfastlog.a
EXAMPLE = example_app

all: $(TARGET) $(EXAMPLE)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(TARGET): $(OBJECTS)
	ar rcs $(TARGET) $(OBJECTS)

$(EXAMPLE): $(EXAMPLEDIR)/main.c $(TARGET)
	$(CC) $(CFLAGS) -I$(INCDIR) $(EXAMPLEDIR)/main.c -L. -lfastlog -o $(EXAMPLE)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET) $(EXAMPLE)

install:
	cp $(TARGET) /usr/local/lib/
	cp include/fastlog.h /usr/local/include/

uninstall:
	rm -f /usr/local/lib/$(TARGET)
	rm -f /usr/local/include/fastlog.h
