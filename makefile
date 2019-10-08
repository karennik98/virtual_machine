vm.a: main.cpp ./src/*.cpp
	g++ main.cpp ./src/*.cpp -o ./bin/static/vm.a -std=c++17 -w -g