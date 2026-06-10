package cli

import "core:fmt"
import "core:strings"
Error :: union {
	Failed_To_Parse_Number,
	Unknown_Argument,
}

Failed_To_Parse_Number :: struct {
	val: string,
}

Unknown_Argument :: struct {
	arg: string,
}

err_msg :: proc(buf: ^strings.Builder, error: Error) -> string {
	switch err in error {
	case nil:
		return "No error."
	case Failed_To_Parse_Number:
		return fmt.sbprintf(
			buf,
			"Failed to parse `%s` into a number.",
			err.val,
		)
	case Unknown_Argument:
		return fmt.sbprintf(buf, "Unknown argument `%s`.", err.arg)
	case:
		return "An unknonwn cli error."
	}
}
