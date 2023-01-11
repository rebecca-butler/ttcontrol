all: helloworld

./bin/libfoo.so: ./src/foo.cpp
	g++ -I C:\Eigen ./src/foo.cpp -static-libstdc++ -shared -fPIC -o ./bin/libfoo.so

helloworld: ./src/main.c ./bin/libfoo.so
	gcc ./src/main.c -L. -l:./bin/libfoo.so -o ./bin/main
