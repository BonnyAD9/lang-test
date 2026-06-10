package cli

import "core:strconv"
import "core:strings"

Action :: enum {
	Is_Prime,
	Factor,
}

Args :: struct {
	action: Action,
	helped: bool,
	value:  Maybe(u64),
}

parse :: proc(args: []string) -> (res: Args, err: Error) {
	err = nil
	ok: bool

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
			if strings.starts_with(arg, "-") {
				err = Unknown_Argument{arg}
				return
			} else if res.value, ok = strconv.parse_u64(arg); !ok {
				err = Failed_To_Parse_Number {
					val = arg,
				}
				return
			}
		}
	}

	return
}
