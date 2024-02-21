run: build/main.out
	./build/main.out

build/main.out: build/Fib.o
	g++ main.cpp build/Fib.o build/Fibfunction.o -o build/main.out

build/Fib.o: build/Fibfunction.o
	g++ -c src/Fib.cpp -o build/Fib.o

build/Fibfunction.o:
	g++ -c src/Fibfunction.cpp -o build/Fibfunction.o

create_dir:
	mkdir -p build

clean:
	rm -rf build

build: create_dir build/main.out

rebuild: clean create_dir build/main.out
