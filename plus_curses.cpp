#include <ncurses.h>
#include <string>
#include <iostream>
#include "window_utils.h"
#include <unistd.h>
#include "settings.h"
#include "plus_curses.h"
using namespace cppcurses;
	window::window(WINDOW* canvas) {
		win = canvas;
	}

			/* wrapper for NCURSES' wborder, no multi byte chars.
			 * ARGUMENTS:
			 * vector2d start -> coordinates for the start of the line
			 * chtype ch -> character to fill
			 * int n -> length of line
			 */
			int window::draw_at(const vector2d point, const drawable draw){
				#ifdef SAFEGUARD
					if (win == NULL){
						#ifdef WARNINGS
							std::cerr << "draw_at() at plus_curses.cpp: Invalid window" << std::endl;
							std::cerr << "NULL pointer exception: WINDOW* win" << std::endl;
						#endif
						return INVALID_PTR;
					}
					if (draw.text == NULL){
						#ifdef WARNINGS
							std::cerr << "draw_at() at plus_curses.cpp: Invalid drawable" << std::endl;
							std::cerr << "NULL pointer exception: char* fill" << std::endl;
						#endif
						return INVALID_PTR;
					}
				#endif
				#ifdef BOUNDS_CHECKING
					if (wmove(win, point.y, point.x) != OK) {
						#ifdef PEDANTIC_WARNINGS
							std::cerr << "draw_at() at plus_curses.cpp: Invalid drawing" << std::endl;
							std::cerr << "Point (" << point.x << ", " << point.y << ") will grow past window's dimensions: (" << getmaxx(win) << ", " <<  getmaxy(win) << ")" << std::endl; 
						#endif
						return INVALID_COORD;
					}
				#else
					wmove(win, point.y, point.x);	
				#endif
				wprintw(win, "%s", draw.text);
				return OK;
			}

			int window::ncurses_vline(vector2d start, chtype ch, int n){
				return mvwvline(win, start.y, start.x, ch, n);
			}

			int window::draw_vline(vector2d start, const drawable fill, unsigned n){
				#ifdef SAFEGUARD
					if (win == NULL){
						#ifdef WARNINGS
							std::cerr << "draw_vline() at plus_curses.cpp: Invalid window" << std::endl;
							std::cerr << "NULL pointer exception: WINDOW* win" << std::endl;
						#endif
						return INVALID_PTR;
					}
					if (fill.text == NULL){
						#ifdef WARNINGS
							std::cerr << "draw_vline() at plus_curses.cpp: Invalid filling" << std::endl;
							std::cerr << "NULL pointer exception: char* fill" << std::endl;
						#endif
						return INVALID_PTR;
					}
					NCURSES_SIZE_T winx, winy;
					getmaxyx(win, winy, winx);
					if (winx < start.x && winy < start.y){
						#ifdef WARNINGS
							std::cerr << "draw_vline() at plus_curses.cpp: Invalid drawing" << std::endl;
							std::cerr << "Start (" << start.x << ", " << start.y << ") is out of bounds compared to window's dimensions (" << winx << ", " <<  winy << ")" << std::endl; 
						#endif
						return INVALID_COORD;
					}
					#ifdef PEDANTIC_WARNINGS
						if (winx < start.x+fill.width){
							std::cerr << "draw_vline() at plus_curses.cpp: Invalid drawing" << std::endl;
							std::cerr << "Line with start (" << start.x << ", " << start.y << ") will grow past window's dimensions: (" << winx << ", " <<  winy << ")" << std::endl; 
						}
					#endif
				#endif
				#ifdef BOUNDS_CHECKING
					if (wmove(win, start.y, start.x) != OK){
						return INVALID_COORD;
					}
					NCURSES_SIZE_T width;
					width = getmaxy(win);
					if (n > width - start.y){
						n = width - start.y;
					}
				#else 
					wmove(win, start.y, start.x);
				#endif
				unsigned i;
				for (i = 0; i < n; i++) {
				#ifdef BOUNDS_CHECKING
					if (wmove(win, start.y+i, start.x) != OK){
						return INVALID_COORD;
					}
					NCURSES_SIZE_T win_width;
					win_width = getmaxy(win);
					if (n > win_width - start.y){
						n = win_width - start.y;
					}
				#else
					wmove(win, start.y+i, start.x);
				#endif
					wprintw(win, "%s", fill.text);
				}
				return 0;
			}

			int window::ncurses_hline(vector2d start, chtype ch, int n){
				return mvwhline(win, start.y, start.x, ch, n);
			}


			int window::draw_hline(const vector2d start, const drawable fill, unsigned n){
				#ifdef SAFEGUARD
					if (win == NULL){
						#ifdef WARNINGS
							std::cerr << "draw_hline() at plus_curses.cpp: Invalid window" << std::endl;
							std::cerr << "NULL pointer exception: WINDOW* win" << std::endl;
						#endif
						return INVALID_PTR;
					}
					if (fill.text == NULL){
						#ifdef WARNINGS
							std::cerr << "draw_hline() at plus_curses.cpp: Invalid filling" << std::endl;
							std::cerr << "NULL pointer exception: char* fill" << std::endl;
						#endif
						return INVALID_PTR;
					}
					NCURSES_SIZE_T winx, winy;
					getmaxyx(win, winy, winx);
					if (winx < start.x && winy < start.y){
						#ifdef WARNINGS
							std::cerr << "draw_hline() at plus_curses.cpp: Invalid drawing" << std::endl;
							std::cerr << "Start (" << start.x << ", " << start.y << ") is out of bounds compared to window's dimensions (" << winx << ", " <<  winy << ")" << std::endl; 
						#endif
						return INVALID_COORD;
					}
					#ifdef PEDANTIC_WARNINGS
						if (winx < start.x+n*fill.width){
							std::cerr << "draw_hline() at plus_curses.cpp: Invalid drawing" << std::endl;
							std::cerr << "Line with end (" << start.x+n*fill.width << ", " << start.y << ") would grow past window's dimensions: (" << winx << ", " <<  winy << ")" << std::endl; 
						}
					#endif
				#endif
				#ifdef BOUNDS_CHECKING
					NCURSES_SIZE_T win_width;
					win_width = getmaxx(win);
					int ret = OK;
					if (n*fill.width > win_width - start.x){
						n = (win_width - start.x) / fill.width;
						ret = INVALID_COORD;
					}
					unsigned i;
					for (i = 0; i < n; i++){
					if (wmove(win, start.y, start.x+i*fill.width) != OK){
						return INVALID_COORD;
					}
						wprintw(win, "%s", fill);
					}
					return ret;
				#else
					wmove(win, start.y, start.x);
					while (n){
						wprintw(win, "%s", fill);
						n--;
					}
				#endif
				return 0;
			}

			/* draw_rect
			 * ARGUMENTS:
			 * fill -> window char from ncurses that will be the filling of such rectangle, assumes it's drawing width and height are one
			 * up_corner -> vector2d of upper left coordinates 
			 * dimensions -> vector2d of the rectangle's dimensions
			 * width -> width of fill when printed
			 */
			int window::draw_rect(const vector2d up_corner, const vector2d dimensions, const drawable fill){
				#ifdef SAFEGUARD
					if (win == NULL){
						#ifdef WARNINGS
							std::cerr << "draw_rect() at plus_curses.cpp: Invalid window" << std::endl;
							std::cerr << "NULL pointer exception: WINDOW* win" << std::endl;
						#endif
						return INVALID_PTR;
					}
					if (fill.text == NULL){
						#ifdef WARNINGS
							std::cerr << "draw_rect() at plus_curses.cpp: Invalid filling" << std::endl;
							std::cerr << "NULL pointer exception: char* fill" << std::endl;
						#endif
						return INVALID_PTR;
					}
					NCURSES_SIZE_T winx, winy;
					getmaxyx(win, winy, winx);
						if (winx < up_corner.x && winy < up_corner.y){
								std::cerr << "draw_rect() at plus_curses.cpp: Invalid drawing" << std::endl;
								std::cerr << "Upper corner (" << up_corner.x << ", " << up_corner.y << ") is out of bounds compared to window's dimensions (" << winx << ", " <<  winy << ")" << std::endl; 	
							return INVALID_COORD;
						}
					#ifdef PEDANTIC_WARNINGS
						// TODO: PEDANTIC WARNING
						if (winx < up_corner.x + dimensions.x*fill.width || winy <  up_corner.y + dimensions.y){
							std::cerr << "draw_rect() at plus_curses.cpp: Invalid drawing" << std::endl;
							std::cerr << "Lower corner (" << up_corner.x + dimensions.x*fill.width << ", " << up_corner.y + dimensions.y << ") is out of bounds compared to window's dimensions (" << winx << ", " <<  winy << ")" << std::endl; 	
						}
					#endif
				#endif
				NCURSES_SIZE_T y, x;
				#ifdef BOUNDS_CHECKING
					int ret = 0;
					for (y = 0; y < dimensions.y; y++){
						for (x = 0; x < dimensions.x; x++){
							if (wmove(win, up_corner.y+y, up_corner.x+x*fill.width) != OK){
								ret = INVALID_COORD;
								break;
							}
							wprintw(win, "%s", fill.text);
						}
					}
					return ret;	
				#else
					for (y = 0; y < dimensions.y; y++){
						wmove(win, up_corner.y+y, up_corner.x);
						for (x = 0; x < dimensions.x; x++){
							wprintw(win, "%s", fill);
						}
					}
					return 0;
				#endif
			}

			/* wrapper for NCURSES' wborder, no multi byte chars.
			 * ARGUMENTS:
			 * chtype ls -> character for the left line of the border
			 * chtype rs -> character for the right line of the border
			 * chtype ts -> character for the top line of the border
			 * chtype bs -> character for the top line of the border
			 * chtype tl -> character for the top left corner of the border
			 * chtype tr -> character for the top right corner of the border
			 * chtype bl -> character for the bottom left corner of the border
			 * chtype bl -> character for the bottom right corner of the border
			 */
			int window::ncurses_border(chtype ls, chtype rs, chtype ts, chtype bs, chtype tl, chtype tr, chtype bl, chtype br){
				return wborder(win, ls, rs, ts, bs, tl, tr, bl, br);
			}

			int window::draw_box(const drawable ls, const drawable rs, const drawable ts, const drawable bs, const drawable tl, const drawable tr, const drawable bl, const drawable br, const vector2d top, const vector2d bottom){
				// TOP:
				unsigned cache = (unsigned)(bottom.x - top.x - tr.width);
				draw_at(top, tl);
				draw_hline({top.x+tl.width, top.y}, ts, (cache) / ts.width);
				draw_at({(unsigned) bottom.x, top.y}, tr);
				// BOTTOM:
				cache = (unsigned)(bottom.x - top.x - br.width);
				draw_at({top.x, bottom.y}, bl);
				draw_hline({top.x + bl.width, bottom.y}, bs, (cache) / bs.width);
				draw_at({(unsigned) bottom.x,bottom.y}, br);
				// LEFT:
				cache = (unsigned)(bottom.y - top.y-1);
				draw_vline({top.x,top.y + 1}, ls, cache);
				// RIGHT:
				draw_vline({bottom.x, top.y+1}, rs, cache);
				return 0;
			}

			int window::draw_border(const drawable ls, const drawable rs, const drawable ts, const drawable bs, const drawable tl, const drawable tr, const drawable bl, const drawable br){
				unsigned cache = (unsigned)(getmaxx(win)-tl.width - tr.width);
				// TOP:
				draw_at({0, 0}, tl);
				draw_hline({tl.width, 0}, ts, (cache) / ts.width);
				draw_at({(unsigned) getmaxx(win)-tr.width, 0}, tr);
				// BOTTOM:
				cache = (unsigned)(getmaxx(win)- bl.width - br.width);
				draw_at({0, (unsigned) getmaxy(win)-1}, bl);
				draw_hline({bl.width, (unsigned) getmaxy(win)-1}, bs, (cache) / bs.width);
				draw_at({(unsigned) getmaxx(win)-br.width,(unsigned) getmaxy(win)-1}, br);
				// LEFT:
				cache = (unsigned)(getmaxy(win) - 3);
				draw_vline({0,1}, ls, cache);
				// RIGHT:
				draw_vline({(unsigned) getmaxx(win)-rs.width, 1}, rs, cache);
				return 0;
			}

			/* wrapper for NCURSES' box, no multi byte chars.
			 * ARGUMENTS:
			 * chtype verch -> for vertical walls
			 * chtype horch -> for horizontal walls
			 */
			int window::ncurses_box(chtype verch, chtype horch){
				return box(win, verch, horch);
			}

window stdwin(NULL);

	void cppcurses::init_cppcurses(mmask_t mouse){
		initscr();
		start_color();
		cbreak();
		noecho();
		nonl();
		intrflush(stdscr, 0);
		keypad(stdscr, 1);
		mousemask(mouse, NULL);
		use_default_colors();
		stdwin.win = stdscr;
	}

	int cppcurses::new_color_pair(short index, short foreground, short background){
		if (has_colors() == false){
			return 1;
		} 
		return init_pair(index, foreground, background);
	}

	int cppcurses::new_color(short index, short red, short green, short blue){
		if (has_colors() == false){
			return 1;
		} else if (can_change_color() == false){
			return 2;
		}
		return init_color(index, red, green, blue);
	}
