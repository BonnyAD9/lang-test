package cli

import "core:fmt"

help :: proc() {
	fmt.print(
		`Welcome in help for primes implemented in odin.

Usage:
  primes [flags] number
    Does what the flags say.

Flags:
  -h  -?  --help
    Show this help.

  -f  --factor
    Factor the prime number.

  -i  --is
    Check if the number is prime. This is the default.
`,
	)
}
