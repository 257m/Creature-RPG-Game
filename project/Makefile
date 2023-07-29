CC = clang
CFLAGS = -g -Wno-everything `pkg-config --cflags glfw3 glu gl` 
LDFLAGS = `pkg-config --libs glfw3 glu gl` -lm

# This is the name of the executable we are going to create
EXE = main

# This is the default target that is going to be built when we run 'make'
all: clean $(EXE) run

# This is the directory where our source will be stored
SRC_DIR = src

# This is the list of all source files
SRC = $(wildcard src/**/*.c) $(wildcard src/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)

# This is the directory where our object files will be stored
OBJ_DIR = obj

# This is the list of object files that we need to create
OBJ = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRC))

# This is the rule to build the executable
$(EXE): $(OBJ)
		$(CC) $(LDFLAGS) $^ -o $@

run:
	./$(EXE)

# These are the rules to build the object files
$(OBJ_DIR)/%.o: src/%.c
		$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/**/%.o: src/**/%.c
		$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/**/**/%.o: src/**/**/%.c
		$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f main
