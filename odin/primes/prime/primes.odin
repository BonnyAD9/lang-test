package prime

import "core:math"

Factors :: struct {
	fac: u64,
	n:   u64,
}

is :: proc(n: u64) -> bool {
	if n == 2 || n == 3 {
		return true
	}

	if n < 5 || n % 2 == 0 || n % 3 == 0 {
		return false
	}

	lim := cast(u64)math.sqrt(cast(f64)n) + 1

	for i: u64 = 5; i <= lim; i += 6 {
		if n % i == 0 || n % (i + 2) == 0 {
			return false
		}
	}

	return true
}

factors :: proc(n: u64) -> Factors {
	return Factors{n = n, fac = 2}
}

next_factor :: proc(f: ^Factors) -> Maybe(u64) {
	for ; f.fac <= f.n; f.fac += 1 {
		if f.n % f.fac == 0 {
			f.n /= f.fac
			return f.fac
		}
	}

	return nil
}
