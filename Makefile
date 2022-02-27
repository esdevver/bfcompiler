
OBJDIR := obj

SRC := $(shell find src/* -type f -name '*.c')
OBJ := $(SRC:%.c=$(OBJDIR)/%.o)

CFLAGS := -ansi -O3 -Wall -Wextra -pedantic

exe := bfc

$(exe): $(OBJ)
	#Linking program
	$(CC) $(CFLAGS) -o $(exe) $(OBJ)

$(OBJ): $(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: hefty
hefty:
	$(CC) $(CFLAGS) -o $(exe) $(SRC)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(exe)

