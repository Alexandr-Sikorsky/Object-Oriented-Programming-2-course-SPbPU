all: main

main: main.cpp
	g++ contact.cpp validator.cpp ui.cpp filework.cpp add-edit-del.cpp search-sort.cpp main.cpp -o main

run: main
	main.exe

clean:
	rm -f main.exe