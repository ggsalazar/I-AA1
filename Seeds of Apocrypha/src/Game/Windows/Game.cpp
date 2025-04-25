#include <iostream>
#include <thread>
#include <string>
#include "Game.h"
#include "Scene.h"

Game::Game(const char* title, uint init_fps) :
    fps(init_fps) {
    //Delta time
    target_frame_time = 1.f / fps;
    last_time = Clock::now();

    //Initialize the window
    resolution = min_res*2;
    window = make_u<Window_Windows>(1, "Seeds of Apocrypha", resolution); //1 for primary home monitor, 0 for secondary home monitor, defaults to fullscreen
    resolution = window->WinSize();
    cout << resolution << endl;

    //Initialize the renderer
    renderer = make_u<Renderer_D2D>(window->GetHandle());

    //Initialize the Input namespace
    Input::Init(window->GetHandle(), resolution);

    //Load the default font
    default_font = make_u<Font_D2D>("assets/Fonts/m5x7", renderer->GetDWriteFactory());
    debug_txt = make_u<Text>(default_font.get());

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
    Sprite::Info csi = {}; csi.frame_size = { 16 }; csi.scale = resolution.x / min_res.x;
    cursor = make_u<Sprite_D2D>("assets/Sprites/Cursors/Default", renderer->GetDC(), csi);
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
    /*
    if (auto scene = active_scene.lock())
        scene->GetInput();
    else
        cerr << "ERROR: ACTIVE SCENE NO LONGER VALID!" << endl;
    */
}

//Update the game world
void Game::Update() {

    //Reset our input variables
    Input::Update();

    //Update cursor position
    cursor->MoveTo(Input::MousePos());

    //Open the Title scene
    auto a_s = active_scene.lock();
    if (!a_s)
        SetScene(Scenes::TITLE);

    //Close the old scene if needed
    if (auto old_scn = old_scene.lock()) {
        old_scn->Open(false);
        old_scene.reset();
    }

    if (auto scene = active_scene.lock())
        scene->Update();
}

//Draw the game world
void Game::Render() {
    
    renderer->BeginFrame(); //This also clears the frame

    //renderer->DrawTxt(*debug_txt);

    if (auto scene = active_scene.lock())
        scene->Draw();
    else
        cerr << "ERROR: ACTIVE SCENE NO LONGER VALID!" << endl;

    renderer->DrawSprite(*cursor);
    renderer->EndFrame();
}

void Game::SetScene(Scenes scn) {
    if (scenes.find(scn) != scenes.end()) {

        old_scene = active_scene;

        //Open the new scene
        active_scene = scenes[scn];
        auto scene = active_scene.lock();
        if (auto o_scn = old_scene.lock()) {
            if (scene->label != Scenes::TITLE)
                scene->SetPartyMems(o_scn->GetPartyMems());
        }
        scene->Open();

        //Old scene *must* be closed next frame!
    }
    else
        cout << "That Scene does not exist!" << endl;
}

void Game::SetMusicVolume(float n_v) {
    Math::Clamp(n_v, 0, 100);
    music_volume = n_v;
    //dj.SetVolume(music_volume);
}

void Game::SetSFXVolume(float n_v) {
    /*
    Math::Clamp(n_v, 0, 100);
    sfx_volume = n_v;
    auto scene = active_scene.lock();
    scene->SetEntitySFXVolume(sfx_volume);
    */
}

void Game::SetResolution(uint res_scalar) {
    //Minimum resolution is 640 x 360
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
    window.setVerticalSyncEnabled(true);
    camera.setSize(Vector2f(window.getSize()));
    camera.setCenter(Vector2f(window.getSize()) * .5f);

    if (auto scene = active_scene.lock())
        scene->ResizeMenus();
    */
}

void Game::SetResolution(Vector2u n_r) {
    if (n_r.x > 0 and n_r.y > 0) {
        n_r.x = n_r.x <= window->ScreenSize().x ? n_r.x : window->ScreenSize().x;
        n_r.y = n_r.y <= window->ScreenSize().y ? n_r.y : window->ScreenSize().y;

        resolution = n_r;

        window->Destroy();
        window->Create("Seeds of Apocrypha", resolution);
        /*
        window.setVerticalSyncEnabled(true);
        camera.setSize(Vector2f(window.getSize()));
        camera.setCenter(Vector2f(window.getSize()) * .5f);
     
        if (auto scene = active_scene.lock())
            scene->ResizeMenus();
            */
    }
}
