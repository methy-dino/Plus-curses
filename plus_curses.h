#pragma once
#include <ncurses.h>
#include <string>
#include <iostream>
#include "window_utils.h"
#include <unistd.h>
#include "settings.h"
namespace cppcurses {
	class window {
		public:
			WINDOW *win;
			window(WINDOW* canvas);
			/* wrapper for NCURSES' wborder, no multi byte chars.
			 * ARGUMENTS:
			 * vector2d start -> coordinates for the start of the line
			 * chtype ch -> character to fill
			 * int n -> length of line
			 */
			int draw_at(const vector2d point, const drawable draw);

			int ncurses_vline(vector2d start, chtype ch, int n);

			int draw_vline(vector2d start, const drawable fill, unsigned n);

			int ncurses_hline(vector2d start, chtype ch, int n);

			int draw_hline(const vector2d start, const drawable fill, unsigned n);

			/* draw_rect
			 * ARGUMENTS:
			 * fill -> window char from ncurses that will be the filling of such rectangle, assumes it's drawing width and height are one
			 * up_corner -> vector2d of upper left coordinates 
			 * dimensions -> vector2d of the rectangle's dimensions
			 * width -> width of fill when printed
			 */
			int draw_rect(const vector2d up_corner, const vector2d dimensions, const drawable fill);
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
			int ncurses_border(chtype ls, chtype rs, chtype ts, chtype bs, chtype tl, chtype tr, chtype bl, chtype br);
			/* box draw with arguments like ncurses' borders */
			int draw_box(const drawable ls, const drawable rs, const drawable ts, const drawable bs, const drawable tl, const drawable tr, const drawable bl, const drawable br, const vector2d top, const vector2d bottom);
			/* window border draw with arguments like ncurses' borders */
			int draw_border(const drawable ls, const drawable rs, const drawable ts, const drawable bs, const drawable tl, const drawable tr, const drawable bl, const drawable br);
			/* wrapper for NCURSES' box, no multi byte chars.
			 * ARGUMENTS:
			 * chtype verch -> for vertical walls
			 * chtype horch -> for horizontal walls
			 */
			int ncurses_box(chtype verch, chtype horch);
			/* closes the ncurses' window managed by this instance */
			inline int closewin(){
				return delwin(win);
			}

			/* flushes visuals to console/terminal */
			inline int flush(){
				return wrefresh(win);
			}

			/*moves the window left-up corner to newup */
			inline int move_root(vector2d newup){
				return mvwin(win, newup.y, newup.x);
			}

			/* accepts COLOR_PAIRs and curses attributes
			 * -> ex: COLOR_PAIR(0) will set colors to default
			 */
			inline int win_attrset(int attrs){
				return wattrset(win, attrs);
			}

			/* accepts COLOR_PAIRs and curses attributes
			 * turns on given attribute.
			 */
			inline int win_attron(int attrs){
				return wattron(win, attrs);
			}

			/* accepts COLOR_PAIRs and curses attributes
			 * turns off the given attribute.
			 */
			inline int win_attroff(int attrs){
				return wattroff(win, attrs);
			}

			/* gets character from stdin when the focus is the window */
			inline int win_getch(void){
				return wgetch(win);
			}
	};
	inline int get_mouse(MEVENT *event){
		return getmouse(MEVENT);
	}
	inline window create_window(vector2d start, vector2d dimensions){
		return window(newwin(dimensions.x, dimensions.y, start.y, start.x));
	}

	void init_cppcurses(mmask_t events);

	int new_color_pair(short index, short foreground, short background);
	int new_color(short index, short red, short green, short blue);
}
extern cppcurses::window stdwin;
