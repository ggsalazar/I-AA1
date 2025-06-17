#include <thread>
#include <string>
#include "Game.h"
#include "Menu.h"
#include "../Core/Input.h"
#include "../Core/Math.h"
#include "../Entities/Entity.h"

Game::Game(const char* title, const uint init_fps)
    : fps(init_fps), resolution(min_res * 2),
    window("Shadows of Autongrad", resolution), renderer(window.GetWin(), &camera) {

    //Initialize SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
    TTF_Init();

    //Delta time
    target_frame_time = 1.f / fps;
    last_time = Clock::now();

    //Set the resolution, camera dimensions, and tile size
    resolution = window.WinSize();
    Sprite::SetRenderer(renderer.GetRenderer());
    camera.viewport.w = resolution.x;
    camera.viewport.h = resolution.y;
    TS = GetResScale() * BASE_TS;
    METER = 2 * TS;

    //Set the game pointer in Entity
    Entity::SetGame(this);

    //Initialize the Input namespace
    Input::Init(&window, &camera);

    //Initialize fonts
    for (int i = 12; i < 96; i += 12)
        default_fonts.insert({ i, Font("m5x7", i) });
    debug_txt.Init(&default_fonts[48]);

    //Initialize the DJ's tracks
    //Play the title track - TO-DO

    //Initialize Scenes
    Scene::SetGame(this);
    scenes.insert({ Scenes::TITLE, Scene(Scenes::TITLE) });
    scenes.insert({ Scenes::CUTSCENE, Scene(Scenes::CUTSCENE) });
    scenes.insert({ Scenes::AREA, Scene(Scenes::AREA) });

    //Menus
    Menu::SetGame(this);

    //Dialogue Manager
    dlg_mngr.Init(this);

    //Initialize cursor
    //Cursor sprite info
    Sprite::Info csi = {};
    csi.sheet = "UI/Cursors"; csi.frame_size = { 16 }; csi.scale = resolution.x / min_res.x;
    cursor.Init(csi);
    //SDL_SetWindowRelativeMouseMode(); This will lock the cursor to the game window
    SDL_HideCursor();
}

void Game::Run() {
    //Calculate delta time
    auto now = Clock::now();
    chrono::duration<float> delta = now - last_time;
    last_time = now;
    delta_time = delta.count();

    //Game frames
    accumulated_time += delta_time;
    if (accumulated_time >= target_frame_time) {
        accumulated_time -= target_frame_time;
        if (++game_frames >= fps) game_frames = 0;
    }

    //Handle events
    window.PollEvents();
    if (window.open) {
        if (!active_scene) SetScene(Scenes::TITLE);

        //Process input
        ProcessInput();
        //Update the game world
        Update();
        //Draw the game world
        Render();
    }
    else running = false;

    //Framerate cap
    auto frame_time = (Clock::now() - now).count();
    if (frame_time < target_frame_time)
        this_thread::sleep_for(chrono::duration<float>(target_frame_time - frame_time));
}

//Process input
void Game::ProcessInput() {

    //Get input for the active scene
    if (active_scene)
        active_scene->GetInput();
    else
        cerr << "ERROR: ACTIVE SCENE NOT PROCESSING INPUT!\n";

    //Update cursor position
    cursor.MoveTo(Vector2f(Input::MousePos()));
}

//Update the game world
void Game::Update() {
    //Reset our input variables
    Input::Update();

    //Update the active scene
    if (active_scene)
        active_scene->Update();
    else
        cerr << "ERROR: ACTIVE SCENE NOT UPDATING!\n";

    //Close the old scene if needed
    if (old_scene) {
        old_scene->Open(false);
        old_scene = nullptr;
    }
}

//Draw the game world
void Game::Render() {

    renderer.BeginFrame(); //This also clears the frame

    //Draw the current scene
    if (active_scene)
        active_scene->Draw();
    else
        cerr << "ERROR: ACTIVE SCENE NOT RENDERING!\n";


    renderer.DrawSprite(cursor);

    renderer.EndFrame();
}

void Game::SetScene(const Scenes scn) {

    if (scenes.find(scn) != scenes.end()) {

        old_scene = active_scene;

        //Open the new scene
        active_scene = &scenes[scn];
        if (old_scene) {
            if (active_scene->label != Scenes::TITLE)
                active_scene->SetPartyMems(old_scene->GetPartyMems());
        }
        active_scene->Open();

        //Old scene *must* be closed next frame!
    }
    else
        cout << "That Scene does not exist!\n";
}

void Game::SetQuest(const Quest quest, const byte status) {
    switch (quest) {
        case Quest::Test: quest_log.test = status;
    }
}

void Game::SetMusicVolume(float n_v) {
    Math::Clamp(n_v, 0, 100);
    music_volume = n_v;
    //dj.SetVolume(music_volume);
}

void Game::SetSFXVolume(float n_v) {
    Math::Clamp(n_v, 0, 100);
    sfx_volume = n_v;
    active_scene->SetEntitySFXVolume(sfx_volume);
}

void Game::SetResolution(uint res_scalar) {
    //Minimum resolution is 640 x 360
    if (res_scalar > 0) {
        Vector2u new_win_size = res_scalar * min_res;
        while (new_win_size.x > window.ScreenSize().x or new_win_size.y > window.ScreenSize().y) {
            --res_scalar;
            new_win_size = res_scalar * min_res;
        }
        resolution = new_win_size;
    }
    else {
        res_scalar = 1;
        resolution = min_res;
    }

    SetRes();
}

void Game::SetResolution(Vector2u n_r) {

    if (n_r.x > 0 and n_r.y > 0) {
        n_r.x = n_r.x <= window.ScreenSize().x ? n_r.x : window.ScreenSize().x;
        n_r.y = n_r.y <= window.ScreenSize().y ? n_r.y : window.ScreenSize().y;

        resolution = n_r;

        SetRes();
    }

}

void Game::SetRes() {
    //Resize the window
    if (resolution == window.ScreenSize())
        SDL_SetWindowFullscreen(window.GetWin(), true);
    else {
        SDL_SetWindowFullscreen(window.GetWin(), false);
        SDL_SetWindowSize(window.GetWin(), resolution.x, resolution.y);
    }

    //Resize the camera & tiles
    camera.viewport.w = resolution.x;
    camera.viewport.h = resolution.y;
    TS = GetResScale() * BASE_TS;
    METER = 2 * TS;

    //Resize the menus
    if (active_scene)
        active_scene->ResizeMenus();
}
