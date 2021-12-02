PROJECT = main
SRC = src
INC = inc
TEST = test
TEST_INC = test/inc
OBJ = obj
ODIR = build

SRCS = $(wildcard $(SRC)/*.c)
TESTS = $(wildcard $(TEST)/*.c)
OBJS = $(addprefix $(OBJ)/, $(notdir $(SRCS:.c=.o)))
TEST_OBJS = $(addprefix $(OBJ)/, $(notdir $(TESTS:.c=.o)))

CC = gcc
DEBUGGER = gdb
RM      = rm -rf
MKDIR   = @mkdir -p $(@D) #creates folders if not present
CFLAGS = -I$(INC) -g

$(info $(TEST_OBJS))

all: $(ODIR)/$(PROJECT).bin

test: $(ODIR)/$(TEST)/test_runner


$(OBJ)/%.o: $(SRC)/%.c
	$(MKDIR)   
	$(CC) -o $@ $^ -c $(CFLAGS)
	
$(ODIR)/$(PROJECT).bin: $(OBJS)
	$(MKDIR)           
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJ)/%.o: $(TEST)/%.c
	$(MKDIR)
	$(CC) -o $@ $^ -c $(CFLAGS) -I$(TEST_INC)

$(ODIR)/$(TEST)/test_runner: $(TEST_OBJS) $(filter-out $(OBJ)/main.o, $(OBJS))
	$(MKDIR)
	$(CC) -o $@ $^ $(CFLAGS)

debug:
	$(DEBUGGER) --tui $(ODIR)/$(PROJECT).bin

#remove object and bin files
clean:
	-$(RM) $(OBJ)
	-$(RM) $(ODIR)

.PHONY: all clean
