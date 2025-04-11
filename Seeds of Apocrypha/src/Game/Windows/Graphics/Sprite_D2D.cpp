#include "Sprite_D2D.h"
#include "../Game.h"

void Engine::Sprite_D2D::Update(const Game& game) {
	info.frame_timer += game.delta_time;

	if (info.frame_timer >= info.frame_length) {
		info.frame_timer -= info.frame_length;
		if (info.anim_fps > 0) {
			++info.curr_frame;
			if (info.curr_frame == info.num_frames) info.curr_frame = 0;
		}
		else if (info.anim_fps < 0) {
			--info.curr_frame;
			if (info.curr_frame < 0) info.curr_frame = info.num_frames - 1;
		}
	}

}
