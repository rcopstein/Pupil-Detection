OPENCV_FLAGS = `pkg-config --cflags opencv`
OPENCV_LIBS = `pkg-config --libs opencv`

INPUT = ./Main.cpp
OUTPUT = program.out

main: $(INPUT)
	g++ -o $(OUTPUT) $(OPENCV_FLAGS) $(INPUT) $(OPENCV_LIBS)

run:
	./$(OUTPUT)

clean: 
	rm $(OUTPUT)