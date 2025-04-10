#include <iostream>
#include <thread>
#include <string>
#include "Game.h"

Game::Game(const char* title, uint init_fps) :
            fps(init_fps) {
    //Delta time
    target_frame_time = 1.f / fps;
    last_time = Clock::now();

    //Initialize the window
    window = make_unique<Window_Windows>(1); //1 for primary home monitor, 0 for secondary home monitor
    window->Create("Seeds of Apocrypha"); //Defaults to fullscreen
    resolution = window->GetSize();
    
    //Initialize the renderer
    renderer = make_unique<Renderer_D2D>();
    renderer->Init(window->GetHandle());

    //Test sprite
    test_sheet = make_unique<Spritesheet_D2D>(renderer->GetRT());
    if (!test_sheet->LoadFromFile("assets/Sprites/UI/Toggle"))
        cout << "You done goofed" << endl;
    Sprite::Info s_init_info; s_init_info.pos = { (uint)(resolution.x * .5), (uint)(resolution.y*.5) }; s_init_info.origin = { .5f, .5f };
    s_init_info.frame_size = { 24, 24 }; s_init_info.num_frames = 2; s_init_info.anim_fps = 2; s_init_info.scale = 4;
    test_spr = make_unique<Sprite_D2D>(move(test_sheet), s_init_info);

    //Initialize the camera
    /*
    camera.setSize(Vector2f(window.getSize()));
    //View is set in Scene.cpp

    //Initialize the fonts
    if (!default_font.openFromFile("assets/Fonts/m5x7.ttf"))
        cerr << "Failed to load font 'm5x7'!" << endl;
    default_font.setSmooth(false);

    //Initialize the DJ's tracks
    //Play the title track - TO-DO

    //Initialize Scenes
    title_scene = make_unique<Scene>(*this, window, Scenes::TITLE);
    cutscene_scene = make_unique<Scene>(*this, window, Scenes::CUTSCENE);
    area_scene = make_unique<Scene>(*this, window, Scenes::AREA);
    scenes.insert(make_pair(Scenes::TITLE, title_scene));
    scenes.insert(make_pair(Scenes::CUTSCENE, cutscene_scene));
    scenes.insert(make_pair(Scenes::AREA, area_scene));
    SetScene(Scenes::TITLE);

    //Cursors for lmb actions
    sf::Image c_i;
    //Default cursor
    c_i.loadFromFile("assets/Sprites/Cursors/Default.png");
    auto c = make_unique<sf::Cursor>(sf::Cursor::createFromPixels(c_i.getPixelsPtr(), { 16, 16 }, { 0, 0 }).value());
    cursors.insert({ Actions::DEFAULT, move(c)});
    //NoAction
    c_i.loadFromFile("assets/Sprites/Cursors/NoAction.png");
    c = make_unique<sf::Cursor>(sf::Cursor::createFromPixels(c_i.getPixelsPtr(), { 16, 16 }, { 0, 0 }).value());
    cursors.insert({ Actions::NOACTION, move(c) });
    //Move
    c_i.loadFromFile("assets/Sprites/Cursors/Move.png");
    c = make_unique<sf::Cursor>(sf::Cursor::createFromPixels(c_i.getPixelsPtr(), { 16, 16 }, { 0, 0 }).value());
    cursors.insert({ Actions::MOVE, move(c) });
    
    
    cursor = cursors[Actions::DEFAULT].get();
    window.setMouseCursor(*cursor);


    //Init debug line
    /*
    debug_box.setSize(Vector2f(2, resolution.y));
    debug_box.setPosition(Vector2f(resolution.x * .5, 0));
    debug_box.setFillColor(sf::Color(255, 255, 0, 127));
    */
}

void Game::Run() {
    //Calculate delta time
    auto now = Clock::now();
    chrono::duration<float> delta = now - last_time;
    last_time = now;
    delta_time = delta.count();

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
    
    //if (!--debug_timer) {
    //    debug_timer = fps * 3;
    //}
    
    //if (++frames_elapsed > fps) frames_elapsed = 0;
    test_spr->Update(*this);
    /*
    //Reset our variables
    Input::UpdateVars();

    //Close the old scene if needed
    if (auto old_scn = old_scene.lock()) {
        old_scn->Open(false);
        old_scene.reset();
    }

    auto scene = active_scene.lock();
    scene->Update();

    //window.setMouseCursor(*cursor);
    */
}

//Draw the game world
void Game::Render() {
    
    renderer->BeginFrame(); //This also clears the frame

    renderer->DrawSprite(*test_spr);

    /*
    if (auto scene = active_scene.lock())
        scene->Draw();
    else
        cerr << "ERROR: ACTIVE SCENE NO LONGER VALID!" << endl;
    */
    renderer->EndFrame();
}

void Game::SetScene(Scenes scn) {
    /*
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
        */
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
        Vector2u new_win_size = res_scalar * Display::MinRes();
        while (new_win_size.x > Display::ScreenSize().x or new_win_size.y > Display::ScreenSize().y) {
            --res_scalar;
            new_win_size = res_scalar * Display::MinRes();
        }
        resolution = new_win_size;
    }
    else {
        res_scalar = 1;
        resolution = Display::MinRes();
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
        n_r.x = n_r.x <= Display::ScreenSize().x ? n_r.x : Display::ScreenSize().x;
        n_r.y = n_r.y <= Display::ScreenSize().y ? n_r.y : Display::ScreenSize().y;

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
