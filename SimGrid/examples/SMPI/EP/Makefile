all: ep

SIMGRID_INSTALL_PATH = $$HOME
CC = smpicc
PEDANTIC_PARANOID_FREAK =       -Wshadow -Wcast-align \
				-Waggregate-return -Wmissing-prototypes -Wmissing-declarations \
				 -Wmissing-prototypes -Wmissing-declarations \
				-Wredundant-decls -Wnested-externs \
				-Wpointer-arith -Wwrite-strings -finline-functions -Wall -O0
# -Wstrict-prototypes
REASONABLY_CAREFUL_DUDE =	-Wall -O2
NO_PRAYER_FOR_THE_WICKED =	-w  -O3
WARNINGS = 			$(PEDANTIC_PARANOID_FREAK)
CFLAGS = -g $(WARNINGS) 

XBT_C_FILES = $(wildcard xbt/*.c)
INCLUDES =
DEFS = # -I$(INSTALL_PATH)/include -L$(INSTALL_PATH)/lib/
LDADD = -lm
LIBS = 

C_FILES = $(wildcard *.c)
BIN_FILES = $(patsubst %.c,%,$(C_FILES))
O_FILES = $(patsubst %.c,%.o,$(C_FILES))

ep: ep.o nas_common.o
	$(CC) $(INCLUDES) $(DEFS) $(CFLAGS) $^ $(LIBS) $(LDADD) -o $@

%: %.o
	$(CC) $(INCLUDES) $(DEFS) $(CFLAGS) $^ $(LIBS) $(LDADD) -o $@

%.o: %.c $(H_FILES)
	$(CC) $(INCLUDES) $(DEFS) $(CFLAGS) -c -o $@ $<

TAGS: $(C_FILES) $(H_FILES)
	etags $(C_FILES) $(H_FILES)

griffon_hostfile.txt:
	for i in `seq 1 92`; do echo griffon-$$i.nancy.grid5000.fr; done > $@

run:
	smpirun -np 12 -platform griffon.xml -hostfile griffon_hostfile.txt ./ep 8 A

clean:
	rm -f $(BIN_FILES) *.o *~
.SUFFIXES:
.PHONY : clean all old_msg_all

