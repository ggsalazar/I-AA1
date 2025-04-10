#pragma once
#include "Math.h"

class Game;
class Entity;

struct AnimInfo {
	std::string tex_name;
	Vector2u sprite_size = { 0, 0 };
	uint tex_row = 0;
	uint num_frames = 1;
	uint curr_frame = 0;
	int fps = 0;
	uint fci = 0; //frame_change_interval: How many frames until the next frame of the animation
	float anim_length = 0.f; //Length of the animation in seconds
};

class Animation {
	friend class Entity;
public:
	sf::Sprite sprite;

	//Position, origin, and scale
	struct Transform {
		Vector2u pos = { 0, 0 };
		Vector2f origin = { .0f, .0f };
		Vector2u scale = { 1, 1 };
	};


	Animation(Game& g, sf::RenderWindow& w, Entity* e, const AnimInfo& a_i, const Transform& t = {});


	void Draw();

	void SetSprite(const std::string new_im);

	void SetSpriteW(const uint new_s_w);
	uint GetSpriteW() const { return anim_info.sprite_w; }

	void SetSpriteH(const uint new_s_h);
	uint GetSpriteH() const { return anim_info.sprite_h; }

	void SetTexRow(int new_t_r, const int new_n_f = -1);
	uint GetTexRow() const { return anim_info.tex_row; }

	void SetCurrFrame(int new_c_f);
	uint GetCurrFrame() const { return anim_info.curr_frame; }

	void SetNumFrames(const int new_n_f) { if (new_n_f > 0) anim_info.num_frames = new_n_f; }
	uint GetNumFrames() const { return anim_info.num_frames; }

	void SetFPS(const int new_fps);
	uint GetFPS() const { return anim_info.fps; }


	void SetOrigin(Vector2f new_ori = { .5f, .5f });
	//Returns origin from 0-1
	Vector2f GetOrigin() { return Vector2f(sprite.getOrigin().x / anim_info.sprite_w, sprite.getOrigin().y / anim_info.sprite_h); }

private:
	AnimInfo anim_info;

	sf::Texture texture;
	sf::IntRect tex_rect;

	Game& game;
	sf::RenderWindow& window;
	Entity* entity;
};