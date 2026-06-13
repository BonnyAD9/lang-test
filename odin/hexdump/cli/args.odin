package cli

import "core:strings"

Args :: struct {
	lines:  Maybe(u64),
	helped: bool,
	file:   Maybe(string),
}

parse :: proc(args: []string) -> (res: Args, err: Error) {
	p := parser_new(args)

	for arg, cont := next(&p); cont; arg, cont = next(&p) {
		switch arg {
		case "-h", "-?", "--help":
			help()
			res.helped = true
		case "-l", "--lines":
			res.lines = next_arg(&p, u64) or_return
		case "-i", "--input":
			res.file = next_arg(&p, string) or_return
		case "--head":
			res.lines = 10
		case:
			if strings.starts_with(arg, "-") {
				err = Unknown_Argument{arg}
				return
			}
			res.file = arg
		}
	}

	return
}
