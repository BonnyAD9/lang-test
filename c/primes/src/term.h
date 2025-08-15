#ifndef TERM_H_INCLUDED
#define TERM_H_INCLUDED

#define T_ESC(t) "\x1b" t
#define T_CSI(c) T_ESC("[" c)

#define T_COLOR(n) T_CSI(#n "m")

#define T_RESET T_COLOR(0)

#define T_BOLD T_COLOR(1)
#define T_FAINT T_COLOR(2)
#define T_ITALIC T_COLOR(3)
#define T_UNDERLINE T_COLOR(4)
#define T_BLINKING T_COLOR(5)
#define T_INVERSE T_COLOR(7)
#define T_HIDDEN T_COLOR(8)
#define T_STRIKETROUGH T_COLOR(9)

#define T_BLACK T_COLOR(30)
#define T_DGRAY T_COLOR(90)
#define T_DRED T_COLOR(31)
#define T_DGREEN T_COLOR(32)
#define T_DYELLOW T_COLOR(33)
#define T_DBLUE T_COLOR(34)
#define T_DMAGENTA T_COLOR(35)
#define T_DCYAN T_COLOR(36)

#define T_WHITE T_COLOR(97)
#define T_GRAY T_COLOR(37)
#define T_RED T_COLOR(91)
#define T_GREEN T_COLOR(92)
#define T_YELLOW T_COLOR(93)
#define T_BLUE T_COLOR(94)
#define T_MAGENTA T_COLOR(95)
#define T_CYAN T_COLOR(96)

#endif // TERM_H_INCLUDED