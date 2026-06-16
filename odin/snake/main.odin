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

Page :: enum {
	Intro,
	Game,
	Pause,
	Lose,
}

Game :: struct {
	size:      lm.Vec2(int),
	snake:     queue.Queue(lm.Vec2(int)),
	apple:     lm.Vec2(int),
	direction: Direction,
	dq:        [2]Maybe(Direction),
	speed:     f64,
	next:      f64,
}

State :: struct {
	game:  Game,
	page:  Page,
	wsize: lm.Vec2(int),
}

main :: proc() {
	wsize := lm.vec(800, 800)
	raylib.InitWindow(auto_cast wsize.x, auto_cast wsize.y, "Snake")
	raylib.SetExitKey(.Q)

	state: State
	state_init(&state, lm.vec(20, 20))
	state.wsize = wsize
	defer state_destroy(&state)

	state.game.next = raylib.GetTime() + 1. / state.game.speed

	for !raylib.WindowShouldClose() {
		switch state.page {
		case .Intro:
			state_intro(&state)
		case .Game:
			state_game(&state)
		case .Pause:
			state_pause(&state)
		case .Lose:
			state_lose(&state)
		}
	}

	raylib.CloseWindow()
}

state_intro :: proc(state: ^State) {
	text: cstring = "Play"
	h: i32 = 32
	w := raylib.MeasureText(text, h)
	x := (cast(i32)state.wsize.x - w) / 2
	y := (cast(i32)state.wsize.y - h) / 2

	if raylib.IsMouseButtonPressed(.LEFT) {
		pos := lm.cas(i32, lm.vec(raylib.GetMousePosition()))
		if pos.x >= x && pos.y >= y && pos.x <= x + w && pos.y <= y + h {
			state.page = .Game
			game_reset(&state.game)
		}
	}

	raylib.BeginDrawing()

	raylib.ClearBackground(xrgb(0x123456))

	raylib.DrawText(text, x, y, h, xrgb(0xffffff))

	raylib.EndDrawing()
}

state_lose :: proc(state: ^State) {
	text: cstring = "Play"
	h: i32 = 32
	w := raylib.MeasureText(text, h)
	x := (cast(i32)state.wsize.x - w) / 2
	y := (cast(i32)state.wsize.y - h) / 2

	if raylib.IsMouseButtonPressed(.LEFT) {
		pos := lm.cas(i32, lm.vec(raylib.GetMousePosition()))
		if pos.x >= x && pos.y >= y && pos.x <= x + w && pos.y <= y + h {
			state.page = .Game
			game_reset(&state.game)
		}
	}

	raylib.BeginDrawing()

	raylib.ClearBackground(xrgb(0x123456))

	ts := lm.div(lm.cas(f32, state.wsize), lm.cas(f32, state.game.size))

	for i in 0 ..< state.game.snake.len {
		pos := lm.cas(f32, queue.get(&state.game.snake, i))
		color := xrgb(0x66cc66)
		if state.game.snake.len != 1 {
			cpos := 1. - cast(f32)i / cast(f32)(state.game.snake.len - 1)
			if (state.game.snake.len - i) % 2 == 0 {
				cpos = 1 - (1 - cpos) / 2
			}
			color = raylib.ColorLerp(xrgb(0xaacc66), xrgb(0x55aa55), cpos)
		}
		raylib.DrawRectangleV(lm.mul(pos, ts), ts, color)
	}

	{
		pos := lm.mul(lm.cas(f32, state.game.apple), ts)
		raylib.DrawRectangleV(pos, ts, xrgb(0xee5555))
	}

	raylib.DrawText(text, x, y, h, xrgb(0xffffff))

	{
		txt: cstring = "You Loose"
		size := lm.vec(i32, 0, 64)
		size.x = raylib.MeasureText(txt, size.y)
		pos := lm.div(
			lm.sub(lm.cas(i32, state.wsize), size),
			lm.vec(i32, 2, 4),
		)
		raylib.DrawText(txt, pos.x, pos.y, size.y, xrgb(0xee5555))
	}

	raylib.EndDrawing()
}

state_pause :: proc(state: ^State) {
	text: cstring = "Continue"
	h: i32 = 32
	w := raylib.MeasureText(text, h)
	x := (cast(i32)state.wsize.x - w) / 2
	y := (cast(i32)state.wsize.y - h) / 2

	if raylib.IsMouseButtonPressed(.LEFT) {
		pos := lm.cas(i32, lm.vec(raylib.GetMousePosition()))
		if pos.x >= x && pos.y >= y && pos.x <= x + w && pos.y <= y + h {
			state.page = .Game
			state.game.next = raylib.GetTime() + 1 / state.game.speed
		}
	}

	raylib.BeginDrawing()

	raylib.ClearBackground(xrgb(0x123456))

	ts := lm.div(lm.cas(f32, state.wsize), lm.cas(f32, state.game.size))

	for i in 0 ..< state.game.snake.len {
		pos := lm.cas(f32, queue.get(&state.game.snake, i))
		color := xrgb(0x66cc66)
		if state.game.snake.len != 1 {
			cpos := 1. - cast(f32)i / cast(f32)(state.game.snake.len - 1)
			if (state.game.snake.len - i) % 2 == 0 {
				cpos = 1 - (1 - cpos) / 2
			}
			color = raylib.ColorLerp(xrgb(0xaacc66), xrgb(0x55aa55), cpos)
		}
		raylib.DrawRectangleV(lm.mul(pos, ts), ts, color)
	}

	{
		pos := lm.mul(lm.cas(f32, state.game.apple), ts)
		raylib.DrawRectangleV(pos, ts, xrgb(0xee5555))
	}

	raylib.DrawText(text, x, y, h, xrgb(0xffffff))

	{
		txt: cstring = "Paused"
		size := lm.vec(i32, 0, 64)
		size.x = raylib.MeasureText(txt, size.y)
		pos := lm.div(
			lm.sub(lm.cas(i32, state.wsize), size),
			lm.vec(i32, 2, 4),
		)
		raylib.DrawText(txt, pos.x, pos.y, size.y, xrgb(0xaacc66))
	}

	raylib.EndDrawing()
}

state_game :: proc(state: ^State) {
	for key := raylib.GetKeyPressed();
	    key != .KEY_NULL;
	    key = raylib.GetKeyPressed() {
		#partial switch key {
		case .LEFT:
			game_push_direction(&state.game, .Left)
		case .UP:
			game_push_direction(&state.game, .Up)
		case .RIGHT:
			game_push_direction(&state.game, .Right)
		case .DOWN:
			game_push_direction(&state.game, .Down)
		case .ESCAPE:
			state.page = .Pause
		}
	}

	now := raylib.GetTime()
	if now >= state.game.next {
		if !game_advance(&state.game) {
			state.page = .Lose
		} else {
			s :=
				state.game.speed *
				math.pow(1.01, cast(f64)state.game.snake.len)
			state.game.next = now + 1. / s
		}
	}

	raylib.BeginDrawing()

	raylib.ClearBackground(xrgb(0x123456))

	ts := lm.div(lm.cas(f32, state.wsize), lm.cas(f32, state.game.size))

	for i in 0 ..< state.game.snake.len {
		pos := lm.cas(f32, queue.get(&state.game.snake, i))
		color := xrgb(0x66cc66)
		if state.game.snake.len != 1 {
			cpos := 1. - cast(f32)i / cast(f32)(state.game.snake.len - 1)
			if (state.game.snake.len - i) % 2 == 0 {
				cpos = 1 - (1 - cpos) / 2
			}
			color = raylib.ColorLerp(xrgb(0xaacc66), xrgb(0x55aa55), cpos)
		}
		raylib.DrawRectangleV(lm.mul(pos, ts), ts, color)
	}

	{
		pos := lm.mul(lm.cas(f32, state.game.apple), ts)
		raylib.DrawRectangleV(pos, ts, xrgb(0xee5555))
	}

	raylib.EndDrawing()
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
	game_init(&s.game, size)
}

state_destroy :: proc(s: ^State) {
	game_destroy(&s.game)
}

game_init :: proc(s: ^Game, size: lm.Vec2(int)) {
	s.size = size
	s.speed = 4.
	queue.init(&s.snake)
	queue.enqueue(&s.snake, lm.div(size, 2))
	game_select_apple(s)
}

game_destroy :: proc(s: ^Game) {
	queue.destroy(&s.snake)
}

game_reset :: proc(s: ^Game) {
	s.direction = .Left
	s.next = raylib.GetTime() + 1. / s.speed
	s.dq = {}
	queue.clear(&s.snake)
	queue.enqueue(&s.snake, lm.div(s.size, 2))
	game_select_apple(s)
}

game_push_direction :: proc(s: ^Game, d: Direction) {
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

game_pop_direction :: proc(s: ^Game) -> Direction {
	res, dx := s.dq.x.?
	s.dq.x = s.dq.y
	s.dq.y = nil
	return dx ? res : s.direction
}

game_advance :: proc(s: ^Game) -> bool {
	d := game_pop_direction(s)
	for direction_opposite(d) == s.direction {
		d = game_pop_direction(s)
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
		game_select_apple(s)
	} else {
		queue.dequeue(&s.snake)
	}
	return true
}

game_select_apple :: proc(s: ^Game) {
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
