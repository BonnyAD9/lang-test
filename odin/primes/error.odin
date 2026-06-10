package main

import "cli"
import "core:strings"

Error :: union #shared_nil {
	cli.Error,
}

err_msg :: proc(buf: ^strings.Builder, error: Error) -> string {
	switch err in error {
	case cli.Error:
		return cli.err_msg(buf, err)
	case nil:
		return "No error."
	case:
		return "Unknown error."
	}
}
