#pragma once
#include <iostream>
#include <chrono>
#include <unordered_map>
#include "../../../Engine/Engine.h" //(<memory>, aliases for Engine and smart ptrs)
#include "../Graphics/Window_SDL.h" //(<vector>, SDL_main.h, SDL_video.h)
#include "../Graphics/Renderer_SDL.h" //(SDL.h, SDL_render, Sprite_SDL.h (SDL_image.h), Font_SDL.h, Text.h)
#include "Input.h"

using Clock = std::chrono::high_resolution_clock;

using namespace std;

class Scene;
class Font_SDL; //Needs to be here despite Font_SDL.h being included in Renderer_SDL.h?

class Game {
public:

    //Window + rendering target
    u_ptr<Window_SDL> window;
    u_ptr<Renderer_SDL> renderer;

    //Game UTH details
    bool running = true;
    float delta_time = 0.f;
    bool paused = false;
    uint curr_ui_layer = 0;

    //Camera
    //Camera camera;
    float cam_move_spd = 10.f;
    bool cam_locked = false;

    //Music & SFX
    //DJ dj;
    //Soundboard sb;
    
    //Scenes
    unordered_map<Scenes, s_ptr<Scene>> scenes;
    w_ptr<Scene> active_scene;
    s_ptr<Scene> scene;
    w_ptr<Scene> old_scene;
    s_ptr<Scene> title_scene;
    s_ptr<Scene> cutscene_scene;
    s_ptr<Scene> area_scene;

    //Miscellaneous
    u_ptr<Font_SDL> default_font48;
    u_ptr<Font_SDL> default_font72;
    u_ptr<Font_SDL> default_font96;
    u_ptr<Text> debug_txt;
    u_ptr<Sprite_SDL> cursor;
    Areas area = Areas::DEFAULT;

    Game(const char* title, uint init_fps);
    ~Game() {
        TTF_Quit();
        SDL_Quit();
    }
    
    //Engine-type stuff
    void Run();
    void ProcessInput();
    void Update();
    void Render();

    //Gameplay stuff
    void SetScene(Scenes scn);

    //Settings
    void SetMusicVolume(float n_v);
    float GetMusicVolume() const { return music_volume; }
    void SetSFXVolume(float n_v);
    float GetSFXVolume() const { return sfx_volume; }

    void SetResolution(uint res_scalar);
    void SetResolution(Vector2u n_r);
    Vector2u GetResolution() const { return resolution; }
    uint GetResScale() const { return resolution.x / min_res.x; }

    //Frame stuff
    uint GetFPS() const { return fps; }
    int GetGameFrames() const { return game_frames; }


private:
    //Variables
    uint fps = 0;
    uint game_frames = 0;
    float target_frame_time;
    Clock::time_point last_time;
    float accumulated_time = 0.f;
    float music_volume = 100;
    float sfx_volume = 100;
    Vector2u resolution;
    const Vector2u min_res = { 640, 360 };
};