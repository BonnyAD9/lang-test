package printers

import "core:fmt"

number :: proc(pos: u64) {
	fmt.printf("%8X", pos)
}
