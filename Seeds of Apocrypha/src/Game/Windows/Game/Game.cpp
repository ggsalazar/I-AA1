#include <iostream>
#include <thread>
#include <string>
#include "Game.h"
#include "Scene.h"

Game::Game(const char* title, uint init_fps) :
    fps(init_fps) {
    //Initialize SDL
    TTF_Init();

    //Delta time
    target_frame_time = 1.f / fps;
    last_time = Clock::now();

    //Initialize the window
    resolution = min_res*3;
    window = make_u<Window_SDL>("Seeds of Apocrypha", resolution);
    resolution = window->WinSize();

    //Initialize the renderer
    renderer = make_u<Renderer_SDL>(window->GetWin());

    //Initialize the Input namespace
    Input::Init(window->GetWin(), resolution);
    

    //Load the default font
    default_font48 = make_u<Font_SDL>("assets/Fonts/m5x7");
    default_font72 = make_u<Font_SDL>("assets/Fonts/m5x7", 72);
    default_font96 = make_u<Font_SDL>("assets/Fonts/m5x7", 96);
    debug_txt = make_u<Text>(default_font48.get());

    //Initialize the camera
    //camera.setSize(Vector2f(window.getSize()));
    //View is set in Scene.cpp

    //Initialize the DJ's tracks
    //Play the title track - TO-DO

    //Initialize Scenes
    title_scene = make_u<Scene>(*this, Scenes::TITLE);
    cutscene_scene = make_u<Scene>(*this, Scenes::CUTSCENE);
    area_scene = make_u<Scene>(*this, Scenes::AREA);
    scenes.insert(make_pair(Scenes::TITLE, title_scene));
    scenes.insert(make_pair(Scenes::CUTSCENE, cutscene_scene));
    scenes.insert(make_pair(Scenes::AREA, area_scene));

    //Initialize cursor
    //Cursor sprite info
    Sprite::Info csi = {}; csi.frame_size = { 16 }; csi.scale = resolution.x / min_res.x; csi.sheet = "assets/Sprites/Cursors/Default";
    cursor = make_u<Sprite_SDL>(renderer->GetRenderer(), csi);
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
    window->PollEvents();
    if (window->open) {
        scene = active_scene.lock();
        if (!scene) SetScene(Scenes::TITLE);

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
    //Reset our input variables
    Input::Update();

    //Get input for the active scene
    if (scene)
        scene->GetInput();
    else
        cerr << "ERROR: ACTIVE SCENE NOT PROCESSING INPUT!\n";

    //Update cursor position
    cursor->MoveTo(Vector2i{ (int)Input::MousePos().x, (int)Input::MousePos().y });
}

//Update the game world
void Game::Update() {

    //Update the active scene
    if (scene)
        scene->Update();
    else
        cerr << "ERROR: ACTIVE SCENE NOT UPDATING!\n";

    //Close the old scene if needed
    if (auto old_scn = old_scene.lock()) {
        old_scn->Open(false);
        old_scene.reset();
    }
}

//Draw the game world
void Game::Render() {

    renderer->BeginFrame(); //This also clears the frame

    //Draw the current scene
    if (scene)
        scene->Draw();
    else
        cerr << "ERROR: ACTIVE SCENE NOT RENDERING!\n";


    renderer->DrawSprite(*cursor);

    renderer->EndFrame();
}

void Game::SetScene(Scenes scn) {
    
    if (scenes.find(scn) != scenes.end()) {

        old_scene = active_scene;

        //Open the new scene
        active_scene = scenes[scn];
        scene = active_scene.lock();
        if (auto o_scn = old_scene.lock()) {
            if (scene->label != Scenes::TITLE)
                scene->SetPartyMems(o_scn->GetPartyMems());
        }
        scene->Open();

        //Old scene *must* be closed next frame!
    }
    else
        cout << "That Scene does not exist!\n";
        
}

void Game::SetMusicVolume(float n_v) {
    Math::Clamp(n_v, 0, 100);
    music_volume = n_v;
    //dj.SetVolume(music_volume);
}

void Game::SetSFXVolume(float n_v) {
    Math::Clamp(n_v, 0, 100);
    sfx_volume = n_v;
    scene->SetEntitySFXVolume(sfx_volume);
}

void Game::SetResolution(uint res_scalar) {
    //Minimum resolution is 640 x 360
    /*
    if (res_scalar > 0) {
        Vector2u new_win_size = res_scalar * min_res;
        while (new_win_size.x > window->ScreenSize().x or new_win_size.y > window->ScreenSize().y) {
            --res_scalar;
            new_win_size = res_scalar * min_res;
        }
        resolution = new_win_size;
    }
    else {
        res_scalar = 1;
        resolution = min_res;
    }

    window->Destroy();
    window->Create("Seeds of Apocrypha", resolution);
    /*
    camera.setSize(Vector2f(window.getSize()));
    camera.setCenter(Vector2f(window.getSize()) * .5f);

    if (auto scene = active_scene.lock())
        scene->ResizeMenus();
    */
}

void Game::SetResolution(Vector2u n_r) {
    /*
    if (n_r.x > 0 and n_r.y > 0) {
        n_r.x = n_r.x <= window->ScreenSize().x ? n_r.x : window->ScreenSize().x;
        n_r.y = n_r.y <= window->ScreenSize().y ? n_r.y : window->ScreenSize().y;

        resolution = n_r;

        window->Destroy();
        window->Create("Seeds of Apocrypha", resolution);
        /*
        camera.setSize(Vector2f(window.getSize()));
        camera.setCenter(Vector2f(window.getSize()) * .5f);
     
        if (auto scene = active_scene.lock())
            scene->ResizeMenus();
            
    }
    */
}
