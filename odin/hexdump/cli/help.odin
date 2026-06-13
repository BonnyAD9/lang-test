package cli

import "core:fmt"

help :: proc() {
	fmt.print(
		`Welocme to help for hexdump in odin by BonnyAD9

Usage:
  hexdump [flags] [file]
    Dump the given file or stdin.

Flags:
  -h  -?  --help
    Show this help.

  -l  --lines <number>
    Print the given number of first hex lines.

  --head
    Print only the start of the file (10 hex lines).

  -i  --input <path>
    Specify input file.
`,
	)
}
