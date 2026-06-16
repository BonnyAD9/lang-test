package lm

Vec2 :: struct($T: typeid) {
	using _: [2]T,
}

vec :: proc {
	vec2_auto,
	vec2_typed,
	vec2_arr,
}

vec2 :: proc {
	vec2_auto,
	vec2_typed,
	vec2_arr,
}

cas :: proc {
	vec2_cas,
}

add :: proc {
	vec2_add,
}

sub :: proc {
	vec2_sub,
}

mul :: proc {
	vec2_cmul,
}

div :: proc {
	vec2_div,
	vec2_cdiv,
}

vec2_auto :: proc(x: $T, y: T) -> Vec2(T) {
	return {{x, y}}
}

vec2_typed :: proc($T: typeid, x: T, y: T) -> Vec2(T) {
	return {{x, y}}
}

vec2_arr :: proc(d: [2]$T) -> Vec2(T) {
	return {d}
}

vec2_cas :: proc($T: typeid, v: Vec2($O)) -> Vec2(T) {
	return vec(cast(T)v.x, cast(T)v.y)
}

vec2_add :: proc(a: Vec2($T), b: Vec2(T)) -> Vec2(T) {
	return vec(a.x + b.x, a.y + b.y)
}

vec2_sub :: proc(a: Vec2($T), b: Vec2(T)) -> Vec2(T) {
	return vec(a.x - b.x, a.y - b.y)
}

vec2_cmul :: proc(a: Vec2($T), b: Vec2(T)) -> Vec2(T) {
	return vec(a.x * b.x, a.y * b.y)
}

vec2_div :: proc(v: Vec2($T), a: T) -> Vec2(T) {
	return vec(v.x / a, v.y / a)
}

vec2_cdiv :: proc(a: Vec2($T), b: Vec2(T)) -> Vec2(T) {
	return vec(a.x / b.x, a.y / b.y)
}
