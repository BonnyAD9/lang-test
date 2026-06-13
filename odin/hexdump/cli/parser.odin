package cli

import "core:strconv"
@(private)
Parser :: struct {
	args: []string,
	pos:  int,
}

@(private)
parser_new :: proc(args: []string) -> (res: Parser) {
	res.args = args
	res.pos = -1
	return
}

@(private)
next :: proc(p: ^Parser) -> (res: string, ok: bool) {
	p.pos += 1
	if p.pos < len(p.args) {
		res = p.args[p.pos]
		ok = true
	}
	return
}

@(private)
next_arg :: proc {
	next_arg_u64,
	next_arg_string,
}

@(private)
next_arg_u64 :: proc(
	p: ^Parser,
	$T: typeid,
) -> (
	res: T,
	err: Error,
) where T ==
	u64 {
	ok: bool
	s := next_string(p) or_return
	res, ok = strconv.parse_u64(s)
	if !ok {
		err = Failed_To_Parse_Number {
			arg = s,
		}
	}
	return
}

@(private)
next_arg_string :: proc(
	p: ^Parser,
	$T: typeid,
) -> (
	res: T,
	err: Error,
) where T ==
	string {
	return next_string(p)
}

@(private)
next_string :: proc(p: ^Parser) -> (res: string, err: Error) {
	p.pos += 1
	if p.pos >= len(p.args) {
		err = Expected_More_Arguments {
			arg = p.pos == 0 ? "" : p.args[p.pos - 1],
		}
		return
	}
	res = p.args[p.pos]
	return
}
