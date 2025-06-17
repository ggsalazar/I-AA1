#pragma once
#include <chrono>
#include "Scene.h" //unordered_map, Geometry, Pathfinding, TileMap
#include "../Core/Camera.h" //Geometry
#include "../Graphics/Window.h"
#include "../Graphics/Renderer.h" //SDL_render, Sprite (SDL_image)
#include "../Dialogue/Yggdrasil.h"

using Clock = std::chrono::high_resolution_clock;
using namespace std;

class Game {
private:
    //Variables
    uint fps = 0;
    uint game_frames = 0;
    float target_frame_time;
    Clock::time_point last_time;
    float accumulated_time = 0.f;
    float music_volume = 100;
    float sfx_volume = 100;

    const Vector2u min_res = { 640, 360 };

    struct QuestLog {
        //-2 = missed, -1 = unavailable, 0 = available but not started, 1 = started, 2 = completed, 3 = failed
        byte test{ 0 };
    };

public:
    //Camera
    Camera camera;
    float cam_move_spd = 5.f; //1 - 10 by .5
    bool edge_panning = true;

    //Game UTH details
    Vector2u resolution;
    Window window;
    Renderer renderer;
    bool running = true;
    float delta_time = 0.f;
    bool paused = false;
    uint curr_ui_layer = 0;
    Yggdrasil dlg_mngr;
    QuestLog quest_log = {};

    //Music & SFX - waiting for SDL_mixer 3.0
    //DJ dj;
    //Soundboard sb;

    //Scenes
    unordered_map<Scenes, Scene> scenes;
    Scene* active_scene = nullptr;
    Scene* old_scene = nullptr;

    //Miscellaneous
    unordered_map<int, Font> default_fonts;
    Text debug_txt;
    Sprite cursor;
    Area area = Area::NONE;

    Game(const char* title, const uint init_fps);
    ~Game() {
        TTF_Quit();
        //Mix_CloseAudio();
        //Mix_Quit();
        SDL_Quit(); //Pretty sure this has to be called last
    }

    //Engine-type stuff
    void Run();
    void ProcessInput();
    void Update();
    void Render();

    //Gameplay
    void SetScene(const Scenes scn);
    void SetQuest(const Quest quest, const byte status);

    //Settings
    void SetMusicVolume(float n_v);
    inline float GetMusicVolume() const { return music_volume; }
    void SetSFXVolume(float n_v);
    inline float GetSFXVolume() const { return sfx_volume; }

    void SetResolution(uint res_scalar);
    void SetResolution(Vector2u n_r);
    void SetRes();
    inline int GetResScale() const { return resolution.x / min_res.x; }

    //Frame stuff
    inline uint GetFPS() const { return fps; }
    inline int GetGameFrames() const { return game_frames; }
    inline Vector2u GetMinRes() const { return min_res; }
};