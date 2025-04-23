#pragma once
#include <iostream>
#include <chrono>
#include <unordered_map>
#include "../../Engine/Engine.h" //Includes <memory> and aliases for Engine and smart ptrs
#include "Graphics/Window_Windows.h"
#include "Graphics/Renderer_D2D.h" //Includes Sprite_D2D.h, Font_D2D.h, and Text.h
#include "Input.h"

using Clock = std::chrono::high_resolution_clock;

using namespace std;

class Scene;
class Font_D2D;

class Game {
public:

    //Window + rendering target
    u_ptr<Window_Windows> window;
    u_ptr<Renderer_D2D> renderer;

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
    w_ptr<Scene> old_scene;
    s_ptr<Scene> title_scene;
    s_ptr<Scene> cutscene_scene;
    s_ptr<Scene> area_scene;

    //Miscellaneous
    u_ptr<Font> default_font; //Declaring it this way (constructing it as D2D) so I don't have to cast it (TEST IF THAT'S ACTUALLY TRUE)
    u_ptr<Text> debug_txt;
    u_ptr<Sprite_D2D> cursor;
    Areas area = Areas::DEFAULT;

    Game(const char* title, uint init_fps);
    
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
    const Vector2u min_res = { 640, 320 };
};