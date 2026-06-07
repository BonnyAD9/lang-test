package cli

import "core:strconv"

Action :: enum {
	Is_Prime,
	Factor,
}

Args :: struct {
	action: Action,
	helped: bool,
	value:  Maybe(u64),
}

parse :: proc(args: []string) -> (res: Args, ok: bool) {
	ok = true
	res.action = .Is_Prime

	for i := 0; i < len(args); i += 1 {
		arg := args[i]
		switch arg {
		case "-h", "-?", "--help":
			res.helped = true
			help()
		case "-f", "--factor":
			res.action = .Factor
		case "-i", "--is":
			res.action = .Is_Prime
		case:
			res.value = strconv.parse_u64(arg) or_return
		}
	}

	return
}
