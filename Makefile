fruit: fruit.cpp 
	g++ -Wall -g -c -o fruit.o fruit.cpp
	g++ -Wall -g   -c -o board.o board.cpp
	g++ -Wall -g   -c -o tetrisblock.o tetrisblock.cpp
	g++ -Wall -g -c -o InitShader.o include/InitShader.cpp
	g++ -Wall -g -o FruitTetris3D fruit.o InitShader.o board.o tetrisblock.o -lglut -lGLU -lGL -lGLEW -DGL_GLEXT_PROTOTYPES
clean:
	rm ./FruitTetris3D *.o
