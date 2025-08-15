#include "help.h"
#include <stdio.h>
#include "term.h"

static const char *HELP_TEXT =
    "Welcome in help for " T_ITALIC T_GREEN "C primes" T_RESET " by BonnyAD9."
    "\n\n"
    T_GREEN "Usage:\n"
    T_CYAN "  primes " T_YELLOW "-h\n" T_RESET
    "    Print this help.\n"
    "\n"
    T_CYAN "  primes " T_WHITE "<N>\n" T_RESET
    "    Check if the N is prime. Prints 1, if it is prime and 0, if it\n"
    "    is not prime.\n"
    "\n"
    T_CYAN "  primes " T_DGRAY "[" T_DYELLOW "flags" T_DGRAY "] " T_WHITE "<N>"
    T_RESET "\n    Do prime stuff with N based on the flags.\n"
    "\n"
    T_GREEN "Flags:\n"
    T_YELLOW "  -h  -?  --help\n" T_RESET
    "    Print this help and exit.\n";


void help() {
    printf("%s", HELP_TEXT);
}