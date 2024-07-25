sources := $(wildcard *.c)
flags := -Wno-dangling-else
name := sukoGenerator

run: compile
	@echo "Running..."
	./$(name)

compile:
	clear
	@echo "Building..."
	gcc $(flags) -framework OpenGL -framework GLUT $(sources) -o $(name)

run_gdb: compile
	@echo "Running gdb..."
	gdb app.o

clean:
	@echo "Cleaning..."
	@rm -f $(name)