CC = g++ 
CFLAGS = -Wall -ggdb -ansi -pedantic
EXEC_NAME = maze
OBJ_FILES = Astar.o union_find.o maze.o maze_grid.o maze_path.o main.o

all: $(EXEC_NAME)

$(EXEC_NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXEC_NAME) $(OBJ_FILES)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

simple: CFLAGS += -DMAZE_SIMPLE_DISPLAY

simple: clean $(EXEC_NAME)

clean:
	rm -rf $(OBJ_FILES) $(EXEC_NAME)
