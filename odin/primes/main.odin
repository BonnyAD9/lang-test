package main

import "cli"
import "core:fmt"
import "core:os"
import "core:strings"
import "prime"

main :: proc() {
	ok := true
	{
		when ODIN_DEBUG {
			track: mem.Tracking_Allocator
			mem.tracking_allocator_init(&track, context.allocator)
			context.allocator = mem.tracking_allocator(&track)

			defer {
				if len(track.allocation_map) > 0 {
					fmt.eprintfln(
						"=== %v allocations not freed: ===",
						len(track.allocation_map),
					)
					for _, entry in track.allocation_map {
						fmt.eprintfln(
							"- %v B @ %v",
							entry.size,
							entry.location,
						)
					}
				}
			}
		}

		if err := start(); err != nil {
			buf := strings.builder_make()
			defer strings.builder_destroy(&buf)
			fmt.eprintfln("error: %s", err_msg(&buf, err))
			ok = false
		}
	}
	if !ok {
		os.exit(1)
	}
}

start :: proc() -> (err: Error) {
	err = nil

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
