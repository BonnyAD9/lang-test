package printers

import "core:strings"
import "core:fmt"

is_ascii_graphic :: proc(chr: u8) -> bool {
	return chr > 0x20 && chr < 0x7f
}

ascii :: proc(data: []u8) {
	fmt.print('|')

	for c in data {
		if is_ascii_graphic(c) {
			fmt.print(cast(rune)c)
		} else {
			fmt.print('.')
		}
	}

	fmt.print('|')

	for _ in len(data)..<16 {
		fmt.print(' ')
	}
}