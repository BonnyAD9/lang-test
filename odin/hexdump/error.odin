package main

import "cli"
import "core:os"
import "core:strings"

Error :: union #shared_nil {
	Invalid_Number_Of_Arguments,
	os.Error,
	cli.Error,
}

Invalid_Number_Of_Arguments :: enum {
	Error = 1,
}

err_msg :: proc(sb: ^strings.Builder, error: Error) -> string {
	switch err in error {
	case nil:
		return "No error."
	case Invalid_Number_Of_Arguments:
		return "Invalid number of arguments."
	case os.Error:
		return os.error_string(err)
	case cli.Error:
		return cli.err_msg(sb, err)
	case:
		return "Unknown error."
	}
}
