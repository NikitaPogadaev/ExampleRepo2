run: build/main.out
	./build/main.out

build/main.out: build/Fib.o
	g++ main.cpp build/Fib.o -o build/main.out

build/Fib.o:
	g++ -c src/Fib.cpp -o build/Fib.o

create_dir:
	mkdir -p build

clean:
	rm -rf build

rebuild: clean create_dir build/main.out
