package printers

import "core:fmt"

hex :: proc(data: []u8) {
	for value, idx in data {
		if idx < 8 {
			fmt.printf("%2x ", value)
		} else {
			fmt.printf(" %2x", value)
		}
	}
	for _ in len(data)..<16 {
		fmt.print("   ")
	}
}