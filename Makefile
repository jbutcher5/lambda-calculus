CC      = clang++
INCLUDE = src
OBJ     = build
SRC     = src
SRCS    = $(SRC)/main.c
SRCS    += $(wildcard $(SRC)/*.c)
SRCS    += $(wildcard $(SRC)/*/*.c)
SRCS    += $(wildcard $(SRC)/*/*/*.c)
OBJS    = $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
EXE     = lci
CFLAGS  = -I$(INCLUDE) -std=c2x -pedantic
LDLIBS  = -lm

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p "$(@D)"
	@echo "Compiling: $< -> $@"
	@clang -c $(CFLAGS) -g $< -o $@

$(EXE): $(OBJS)
	@echo "Building final executable: $@"
	@clang $^ -g $(LDLIBS) -o $@

$(OBJ):
	mkdir -p $@

format: $(SRC)
	clang-format $^ -i

clean:
	rm -rf $(OBJ) $(EXE)
