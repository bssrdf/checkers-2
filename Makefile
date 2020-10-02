.SUFFIXES:
TARGETS = checkers
CFLAGS = -Wfatal-errors

all : $(TARGETS)

%: %.c
	$(CC) $^ $(CFLAGS) -o $@
