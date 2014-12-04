CC = g++ 
# try with 
CFLAGS = -g -ansi -std=c++11
EXEC_NAME_TEST = maze
EXEC_NAME = astar

default: $(EXEC_NAME)

$(EXEC_NAME): main.o maze.o Astar.o union_find.o maze_path.o maze_grid.o
	$(CC) $(CFLAGS) -o $(EXEC_NAME) main.o maze.o Astar.o union_find.o maze_path.o maze_grid.o
	
$(EXEC_NAME_TEST): test_maze.o maze.o Astar.o union_find.o maze_path.o maze_grid.o
	$(CC) $(CFLAGS) -o $(EXEC_NAME_TEST) test_maze.o maze.o Astar.o union_find.o maze_path.o maze_grid.o

main.o: main.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

test_maze.o: test_maze.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

maze.o: maze.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

Astar.o: Astar.cpp
	$(CC) $(CFLAGS) -o $@ -c $<
	
union_find.o: union_find.cpp
	$(CC) $(CFLAGS) -o $@ -c $<
	
maze_grid.o: maze_grid.cpp
	$(CC) $(CFLAGS) -o $@ -c $<
	
maze_path.o: maze_path.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

simple: CFLAGS += -DMAZE_SIMPLE_DISPLAY

simple: clean $(EXEC_NAME)

clean:
	rm -rf test_maze.o main.o maze.o Astar.o union_find.o maze_path.o maze_grid.o $(EXEC_NAME) $(EXEC_NAME_TEST)
