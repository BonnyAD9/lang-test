package main

import "cli"
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
	args := cli.parse(os.args[1:]) or_return

	path, open := args.file.?
	if !open && args.helped {
		return
	}

	file := open ? os.open(path) or_return : os.stdin
	defer if open {
		os.close(file)
	}

	pos: u64 = 0
	line: u64 = 0
	lines := args.lines.? or_else max(u64)
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
		line += 1
		(line < lines) or_break
	}

	if line != lines {
		fmt.printfln("%8X  ", pos)
	}

	return
}
