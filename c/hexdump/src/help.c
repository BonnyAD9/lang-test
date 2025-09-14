#include "help.h"

#include <stdio.h>

static const char *help_text =
    "Welcome in help for hexdump by BonnyAD9.\n"
    "Usage:\n"
    "  hexdump [flags] [<file>]\n"
    "    Do what the falgs say.\n"
    "\n"
    "Flags:\n"
    "  -h  -?  --help\n"
    "    Show this help.\n"
    "\n"
    "  -f  --file <file>\n"
    "    Set the file to hex dump.\n"
    "\n"
    "  -l  --label\n"
    "    Enable file position on each line. (enabled by default).\n"
    "\n"
    "  -L  --no-label\n"
    "    Disable file postion on each line. (enabled by default).\n"
    "\n"
    "  -a  --ascii\n"
    "    Enable ascii column. (enabled by default).\n"
    "\n"
    "  -A  --no-ascii\n"
    "    Disable ascii column. (enabled by default)\n"
    "\n"
    "    “ One who despises the word will do badly, But\n"
    "      one who fears the commandment will be rewarded. ”\n"
    "                                       ✝ Proverbs 13:13\n";

void help() {
    printf("%s", help_text);
}