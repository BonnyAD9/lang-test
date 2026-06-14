package main

import "core:container/queue"
import "core:math"
import "core:math/rand"
import "lm"
import "vendor:raylib"

Direction :: enum {
	Left,
	Up,
	Right,
	Down,
}

State :: struct {
	size:      lm.Vec2(int),
	snake:     queue.Queue(lm.Vec2(int)),
	apple:     lm.Vec2(int),
	direction: Direction,
	dq:        [2]Maybe(Direction),
}

main :: proc() {
	wsize := lm.vec(800, 800)
	raylib.InitWindow(auto_cast wsize.x, auto_cast wsize.y, "Snake")

	state: State
	state_init(&state, lm.vec(20, 20))
	defer state_destroy(&state)

	speed := 4.
	next := raylib.GetTime() + 1. / speed

	for !raylib.WindowShouldClose() {
		for key := raylib.GetKeyPressed();
		    key != .KEY_NULL;
		    key = raylib.GetKeyPressed() {
			#partial switch key {
			case .LEFT:
				state_push_direction(&state, .Left)
			case .UP:
				state_push_direction(&state, .Up)
			case .RIGHT:
				state_push_direction(&state, .Right)
			case .DOWN:
				state_push_direction(&state, .Down)
			}
		}

		now := raylib.GetTime()
		if now >= next {
			if !state_advance(&state) {
				next = math.INF_F64
			} else {
				s := speed * math.pow(1.01, cast(f64)state.snake.len)
				next = now + 1. / s
			}
		}

		raylib.BeginDrawing()

		raylib.ClearBackground(xrgb(0x123456))

		ts := lm.div(lm.cas(f32, wsize), lm.cas(f32, state.size))

		for i in 0 ..< state.snake.len {
			pos := lm.cas(f32, queue.get(&state.snake, i))
			color := xrgb(0x66cc66)
			if state.snake.len != 1 {
				cpos := 1. - cast(f32)i / cast(f32)(state.snake.len - 1)
				color = raylib.ColorLerp(xrgb(0xaacc66), xrgb(0x55aa55), cpos)
			}
			raylib.DrawRectangleV(lm.mul(pos, ts), ts, color)
		}

		{
			pos := lm.mul(lm.cas(f32, state.apple), ts)
			raylib.DrawRectangleV(pos, ts, xrgb(0xee5555))
		}

		raylib.EndDrawing()
	}

	raylib.CloseWindow()
}

xrgb :: proc(rgb: u32) -> raylib.Color {
	return raylib.Color {
		cast(u8)(rgb >> 16),
		cast(u8)(rgb >> 8),
		cast(u8)rgb,
		0xff,
	}
}

direction_vec :: proc(d: Direction) -> lm.Vec2(int) {
	switch d {
	case .Left:
		return lm.vec(-1, 0)
	case .Up:
		return lm.vec(0, -1)
	case .Right:
		return lm.vec(1, 0)
	case .Down:
		return lm.vec(0, 1)
	}
	return {}
}

direction_opposite :: proc(d: Direction) -> Direction {
	return cast(Direction)((cast(int)d + 2) & 3)
}

state_init :: proc(s: ^State, size: lm.Vec2(int)) {
	s.size = size
	queue.init(&s.snake)
	state_select_apple(s)
	queue.enqueue(&s.snake, lm.div(size, 2))
}

state_destroy :: proc(s: ^State) {
	queue.destroy(&s.snake)
}

state_push_direction :: proc(s: ^State, d: Direction) {
	_, dx := s.dq.x.?
	_, dy := s.dq.y.?
	switch {
	case dy:
		s.dq.x = s.dq.y
		s.dq.y = d
	case dx:
		s.dq.y = d
	case:
		s.dq.x = d
	}
}

state_pop_direction :: proc(s: ^State) -> Direction {
	res, dx := s.dq.x.?
	s.dq.x = s.dq.y
	s.dq.y = nil
	return dx ? res : s.direction
}

state_advance :: proc(s: ^State) -> bool {
	d := state_pop_direction(s)
	for direction_opposite(d) == s.direction {
		d = state_pop_direction(s)
	}
	s.direction = d
	np := lm.add(queue.back(&s.snake), direction_vec(s.direction))
	if np.x < 0 ||
	   np.y < 0 ||
	   np.x >= s.size.x ||
	   np.y >= s.size.y ||
	   (np != queue.front(&s.snake) && queue_contains(&s.snake, np)) {
		return false
	}
	queue.enqueue(&s.snake, np)
	if np == s.apple {
		state_select_apple(s)
	} else {
		queue.dequeue(&s.snake)
	}
	return true
}

state_select_apple :: proc(s: ^State) {
	if s.snake.len == cast(uint)s.size.x * cast(uint)s.size.y {
		return
	}
	for {
		s.apple = lm.vec(
			rand.int_range(0, s.size.x),
			rand.int_range(0, s.size.y),
		)
		if !queue_contains(&s.snake, s.apple) {
			break
		}
	}
}

queue_contains :: proc(q: ^queue.Queue($T), elem: T) -> bool {
	for i in 0 ..< q.len {
		if queue.get(q, i) == elem {
			return true
		}
	}
	return false
}
