# Plus-curses
    A (work in progress) wrapper for curses/ncurses, implementing object orientation, and better support for 'wide' characters, such as emojis (suchas 🙂).

## installation:
    Besides having the prerequisite of a curses implementation, the installation is pretty simple, you may download all files to use in your project, and compile them on demand.

### Utilities:
    Plus-curses has some utilities, described as macros in the 'settings.h' header, such are:
    SAFEGUARD -> enables cancelation of invalid drawings early, avoiding common NULL pointer exception.
    WARNINGS -> extension of SAFEGUARD outputs errors to standard error that are created by SAFEGUARD.
    PEDANTIC\_WARNINGS -> enables pedantic warnings for when a drawing would leave a window's bounds.
    BOUNDS\_CHECKING -> Enables checking for window bounds overflow at runtime, avoiding breaking draw sequences on horizontal lines.
