#include "help.h"

#include <stdio.h>

#include "term.h"

static const char *help_text =
    "Welcome in help for " T_ITALIC T_GREEN "C primes" T_RESET " by BonnyAD9."
    "\n\n" T_GREEN "Usage:\n" T_CYAN "  primes " T_YELLOW "-h\n" T_RESET
    "    Print this help.\n"
    "\n" T_CYAN "  primes " T_WHITE "<N>\n" T_RESET
    "    Check if the N is prime.\n"
    "\n" T_CYAN "  primes " T_DGRAY "[" T_DYELLOW "flags" T_DGRAY "] " T_WHITE
    "<N>" T_RESET "\n    Do prime stuff with N based on the flags.\n"
    "\n" T_GREEN "Flags:\n" T_YELLOW "  -h  -?  --help\n" T_RESET
    "    Print this help and exit.\n"
    "\n" T_YELLOW "  -n  --nth\n" T_RESET
    "    Calculate the Nth prime instead of checking if N is prime. In\n"
    "    ranged mode, print all prames whoose index is in the range. In\n"
    "    estimate mode, estimate the value of nth prime number."
    "\n" T_YELLOW "  -c  --count\n" T_RESET
    "    Count the number of primes in the given range. If ranged mode is\n"
    "    not enabled, print 1 if number is prime and otherwise 0. In\n"
    "    estimate mode, estimate the number of primes in the range."
    "\n" T_YELLOW "  -r  --range\n" T_RESET
    "    Enable ranged mode. When this mode is enabled, by default print all\n"
    "    primes in the given range. The default lower bound is 0.\n"
    "\n" T_YELLOW "  -s  --start " T_WHITE "<S>\n" T_RESET
    "    Enable ranged mode and set the lower bound to S.\n"
    "\n" T_YELLOW "  -e  --estimate\n" T_RESET "    Enable estimate mode.\n"
    "\n" T_YELLOW "  -o  --output " T_WHITE "<PATH>\n" T_RESET
    "    Print the output to the given file. By default print to stdout.\n"
    "\n"
    "    “ " T_ITALIC "One who despises the word will do badly, But\n"
    "      one who fears the commandment will be rewarded." T_RESET " ”\n"
    "                                       " T_BOLD T_WHITE "✝ Proverbs 13:13\n" T_RESET;
    ;

void help() {
    printf("%s", help_text);
}