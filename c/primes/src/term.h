#ifndef TERM_H_INCLUDED
#define TERM_H_INCLUDED

/// Escape code.
#define T_ESC(t) "\x1b" t
/// Csi code, it is `ESC [ code`.
#define T_CSI(c) T_ESC("[" c)

/// Color code, it is `CSI code m`.
#define T_COLOR(n) T_CSI(#n "m")

/// Reset all text modes.
#define T_RESET T_COLOR(0)

/// Set text mode to bold.
#define T_BOLD T_COLOR(1)
/// Set text mode to faint.
#define T_FAINT T_COLOR(2)
/// Set text mode to italic.
#define T_ITALIC T_COLOR(3)
/// Set text mode to underline.
#define T_UNDERLINE T_COLOR(4)
/// Set text mode to blinking.
#define T_BLINKING T_COLOR(5)
/// Set text mode to inverse.
#define T_INVERSE T_COLOR(7)
/// Set text mode to hidden.
#define T_HIDDEN T_COLOR(8)
/// Set text mode to striketrough.
#define T_STRIKETROUGH T_COLOR(9)

/// Set foreground color to black.
#define T_BLACK T_COLOR(30)
/// Set foreground color to dark gray.
#define T_DGRAY T_COLOR(90)
/// Set foreground color to dark red.
#define T_DRED T_COLOR(31)
/// Set foreground color to dark green.
#define T_DGREEN T_COLOR(32)
/// Set foreground color to dark yellow.
#define T_DYELLOW T_COLOR(33)
/// Set foreground color to dark blue.
#define T_DBLUE T_COLOR(34)
/// Set foreground color to dark magenta.
#define T_DMAGENTA T_COLOR(35)
/// Set foreground color to dark cyan.
#define T_DCYAN T_COLOR(36)

/// Set foreground color to white.
#define T_WHITE T_COLOR(97)
/// Set foreground color to gray.
#define T_GRAY T_COLOR(37)
/// Set foreground color to red.
#define T_RED T_COLOR(91)
/// Set foreground color to green.
#define T_GREEN T_COLOR(92)
/// Set foreground color to yellow.
#define T_YELLOW T_COLOR(93)
/// Set foreground color to blue.
#define T_BLUE T_COLOR(94)
/// Set foreground color to magenta.
#define T_MAGENTA T_COLOR(95)
/// Set foreground color to cyan.
#define T_CYAN T_COLOR(96)

#define T_MOVE_TO_START T_CSI("G")
#define T_ERASE_TO_END T_CSI("0J")

#endif // TERM_H_INCLUDED