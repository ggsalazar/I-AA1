#pragma once
#include <iostream>
#include <memory>
#include <unordered_map>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "DJ.h"
#include "Utils/Macros.h"
#include "Utils/Enums.h"

using namespace std;

//Forward declarations
class Scene;

class Game {
public:
    sf::RenderWindow window;
    sf::View camera;
    float cam_move_spd = 10.f;
    bool cam_locked = false;
    DJ dj;
    bool paused = false;
    bool game_over = false;
    sf::Font default_font;
    bool debug = false;
    unordered_map<Actions, std::unique_ptr<sf::Cursor>> cursors;
    sf::Cursor* cursor;

    Game(const char* title, uint init_fps);
    
    //Game stuff
    void HandleEvents();
    void ProcessInput();
    void Update();
    void Render();

    //Getters
    uint GetFPS() const { return fps; }
    int GetDebugTimer() const { return debug_timer; }
    uint GetFramesElapsed() const { return frames_elapsed; }
    float GetMusicVolume() const { return music_volume; }
    float GetSFXVolume() const { return sfx_volume; }
    sf::Vector2u GetResolution() const { return resolution; }
    uint GetResScale() const { return resolution.x / MINRES.x; }

    //Setters
    void SetScene(Scenes scn);
    void SetMusicVolume(float n_v);
    void SetSFXVolume(float n_v);
    void SetResolution(float res_scalar);
    void SetResolution(sf::Vector2u n_r);

    //Scenes
    unordered_map<Scenes, shared_ptr<Scene>> scenes;
    weak_ptr<Scene> active_scene;
    weak_ptr<Scene> old_scene;
    shared_ptr<Scene> title_scene;
    shared_ptr<Scene> cutscene_scene;
    shared_ptr<Scene> area_scene;

    Areas area = Areas::DEFAULT;

private:
    //Variables
    uint fps = 0;
    int debug_timer = 0;
    uint frames_elapsed = 0;
    float music_volume = 100;
    float sfx_volume = 100;
    sf::Vector2u resolution;
};