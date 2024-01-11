default:
	echo "No file provided"
ddemo1:
	g++ -c src/*.cpp -std=c++14 -m64 -g -Wall -I include
	g++ *.o -o bin/debug/demo1 -lncursesw
	./bin/debug/demo1
rdemo1:
	g++ -c src/*.cpp -std=c++14 -m64 -O3 -Wall -I include
	g++ *.o -o bin/release/demo1 -lncursesw
	./bin/release/demo1
