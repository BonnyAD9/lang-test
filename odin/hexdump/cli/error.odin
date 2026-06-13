package cli

import "core:fmt"
import "core:strings"
Error :: union {
	Failed_To_Parse_Number,
	Unknown_Argument,
	Expected_More_Arguments,
}

Failed_To_Parse_Number :: struct {
	arg: string,
}

Unknown_Argument :: struct {
	arg: string,
}

Expected_More_Arguments :: struct {
	arg: string,
}

err_msg :: proc(sb: ^strings.Builder, error: Error) -> string {
	switch err in error {
	case nil:
		return "No error."
	case Failed_To_Parse_Number:
		return fmt.sbprintf(sb, "Failed to parse `%s` into a number.", err.arg)
	case Unknown_Argument:
		return fmt.sbprintf(sb, "Unknown argument `%s`.", err.arg)
	case Expected_More_Arguments:
		return fmt.sbprintf(sb, "Expected more arguments after `%s`.", err.arg)
	case:
		return "Unknown error."
	}
}
