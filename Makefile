bin/main:
	mkdir -p bin
	g++ -std=c++14 -O3 -finput-charset=cp932 -fexec-charset=cp932 CodeVSReborn/main.cpp -o bin/main
