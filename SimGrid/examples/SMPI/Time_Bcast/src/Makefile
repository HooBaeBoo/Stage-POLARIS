all: broadcast

#### External Shell Variables that have to be set for the execution to work properly
# - NPROCS (e.g., 10 for a simulation with 10 MPI ranks)
# - N: total number of iterations
# - MIN: the simulation should start at iteration MIN
# - MAX: the simulation should stopt at iteration MAX
# - DELAY: (e.g., '0, 1.2, 0.3, ..., 0.5') by how much each MPI rank should wait before starting the execution for real
# - RESULTS_TRACE: name of the Paje trace (e.g., output.trace)
# - RESULTS_OUTPUT: name of the output file for stdout (e.g., output.txt)

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

%: %.o
	$(CC) $(INCLUDES) $(DEFS) $(CFLAGS) $^ $(LIBS) $(LDADD) -o $@

%.o: %.c $(H_FILES)
	$(CC) $(INCLUDES) $(DEFS) $(CFLAGS) -c -o $@ $<

TAGS: $(C_FILES) $(H_FILES)
	etags $(C_FILES) $(H_FILES)

griffon_hostfile.txt:
	for i in `seq 1 92`; do echo griffon-$$i.nancy.grid5000.fr; done > $@

run:
	mkdir -p `dirname ${RESULTS_TRACE}`
	mkdir -p `dirname ${RESULTS_OUTPUT}`
	smpirun -np ${NPROCS} -platform griffon.xml -hostfile griffon_hostfile.txt --cfg=smpi/simulate-computation:no -trace -trace-file ${RESULTS_TRACE} --cfg=tracing/smpi/internals:yes --cfg=tracing/smpi:yes --cfg=smpi/host-speed:286087 ./broadcast ${MIN} ${MAX} ${N} ${DELAY} > ${RESULTS_OUTPUT}

clean:
	rm -f $(BIN_FILES) *.o *~
.SUFFIXES:
.PHONY : clean all old_msg_all

