bin/programa : src/main.cpp include/Foco.hpp
	c++ src/main.cpp -o bin/programa -I include

run1 : bin/animacion1
	./bin/animacion1

bin/animacion1 : src/animacion1.cpp include/*.hpp
	@mkdir -p bin
	g++ -Iinclude -o bin/animacion1 src/animacion1.cpp -lftxui-component -lftxui-dom -lftxui-screen

run2 : bin/animacion2
	./bin/animacion2

bin/animacion2 : src/animacion2.cpp include/*.hpp
	@mkdir -p bin
	g++ -Iinclude -o bin/animacion2 src/animacion2.cpp -lftxui-component -lftxui-dom -lftxui-screen