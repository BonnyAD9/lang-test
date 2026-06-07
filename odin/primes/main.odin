package main

import "cli"
import "core:fmt"
import "core:os"
import "prime"

main :: proc() {
	if !start() {
		fmt.eprintfln("error: Failed to parse arguments.")
		os.exit(1)
	}
}

start :: proc() -> (ok: bool) {
	ok = true

	args := cli.parse(os.args[1:]) or_return

	value, is_set := args.value.?
	if !is_set {
		if !args.helped {
			cli.help()
		}
		return
	}

	switch args.action {
	case .Is_Prime:
		is_prime(value)
	case .Factor:
		factor(value)
	}

	return
}

is_prime :: proc(n: u64) {
	if prime.is(n) {
		fmt.printfln("%d is prime.", n)
	} else {
		fmt.printfln("%d is not prime.", n)
	}
}

factor :: proc(n: u64) {
	factors := prime.factors(n)
	cnt := 0

	for {
		fac := prime.next_factor(&factors).? or_break
		fmt.println(fac)
		cnt += 1
	}

	fmt.printfln("Total of %d factors.", cnt)
}
