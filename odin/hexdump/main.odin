package main

import "core:fmt"
import "core:io"
import "core:os"
import "core:strings"
import "printers"

main :: proc() {
	if err := start(); err != nil {
		sb := strings.builder_make()
		fmt.eprintfln("error: %s", err_msg(&sb, err))
		strings.builder_destroy(&sb)
		os.exit(1)
	}
}

start :: proc() -> (err: Error) {
	if len(os.args) != 2 {
		return Invalid_Number_Of_Arguments.Error
	}

	path := os.args[1]
	file := os.open(path) or_return

	pos: u64 = 0
	buf: [16]byte
	for {
		cnt, ioe := os.read(file, buf[:])
		if ioe != nil && ioe != io.Error.EOF {
			return ioe
		}
		(cnt != 0) or_break

		printers.number(pos)
		fmt.print("  ")
		printers.hex(buf[:cnt])
		fmt.print("  ")
		printers.ascii(buf[:cnt])
		fmt.println()

		pos += auto_cast cnt
	}

	fmt.printfln("%8X  ", pos)

	return
}
