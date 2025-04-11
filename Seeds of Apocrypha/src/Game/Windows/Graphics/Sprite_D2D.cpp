#include "Sprite_D2D.h"
#include "../Game.h"

void Engine::Sprite_D2D::Update(const Game& game) {
	if (info.fci != 0 and game.GetFramesElapsed() % info.fci == 0) {
		if (info.anim_fps > 0) ++info.curr_frame;
		else if (info.anim_fps < 0) --info.curr_frame;
	}

}
