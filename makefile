plus_curses.a: plus_curses.o
	ar rsv plus_curses.a plus_curses.o

plus_curses.o: 
	g++ -c plus_curses.cpp -o plus_curses.o -O3
