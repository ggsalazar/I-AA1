#pragma once
#include <iostream>
#include <unordered_map>
#include <chrono>
#include "../../Engine/Engine.h" //Includes <memory> and aliases for std, Engine, and smart ptrs
#include "Graphics/Window_Windows.h"
#include "Graphics/Renderer_D2D.h" //Includes Sprite_D2D.h
#include "Graphics/Fonts/Font_D2D.h"

using Clock = std::chrono::high_resolution_clock;

using namespace Engine;
using namespace std;

class Scene;
class Font;

class Game {
public:

    //Window + rendering target
    u_ptr<Window_Windows> window;
    u_ptr<Renderer_D2D> renderer;

    //Game UTH details
    bool running = true;
    float delta_time;
    int debug_timer = 0;
    bool paused = false;
    uint curr_ui_layer = 0;
    bool debug = false;

    //Camera
    //Camera camera;
    float cam_move_spd = 10.f;
    bool cam_locked = false;

    //Music & SFX
    //DJ dj;
    //Soundboard sb;
    

    //Test sprite
    u_ptr<Sprite_D2D> test_spr;


    //Test Font
    u_ptr<Font> test_font;


    //Miscellaneous
    //u_ptr<Font_D2D> default_font;



    //u_ptr<Text> test_txt;
    
    
    
    
    //unordered_map<Actions, u_ptr<sf::Cursor>> cursors;
    //sf::Cursor* cursor;
    Areas area = Areas::DEFAULT;

    Game(const char* title, uint init_fps);
    
    //Game stuff
    void Run();

    void ProcessInput();
    void Update();
    void Render();

    //Getters
    uint GetFPS() const { return fps; }
    int GetDebugTimer() const { return debug_timer; }
    uint GetFramesElapsed() const { return frames_elapsed; }
    float GetMusicVolume() const { return music_volume; }
    float GetSFXVolume() const { return sfx_volume; }
    Vector2u GetResolution() const { return resolution; }
    uint GetResScale() const { return resolution.x / min_res.x; }

    //Setters
    void SetScene(Scenes scn);
    void SetMusicVolume(float n_v);
    void SetSFXVolume(float n_v);
    void SetResolution(uint res_scalar);
    void SetResolution(Vector2u n_r);

    //Scenes
    unordered_map<Scenes, s_ptr<Scene>> scenes;
    w_ptr<Scene> active_scene;
    w_ptr<Scene> old_scene;
    s_ptr<Scene> title_scene;
    s_ptr<Scene> cutscene_scene;
    s_ptr<Scene> area_scene;

    
private:
    //Variables
    uint fps = 0;
    float target_frame_time;
    Clock::time_point last_time;
    uint frames_elapsed = 0;
    float music_volume = 100;
    float sfx_volume = 100;
    Vector2u resolution;
    const Vector2u min_res = { 640, 320 };
};