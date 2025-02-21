
#include "Game.h"
#include "Scene.h"
#include "../Utils/Util.h"
#include "../Utils/Input.h"

Game::Game(const char* title, uint win_w, uint win_h, uint init_fps, bool fullscrn) :
            fps(init_fps), debug_timer(fps), resolution(sf::Vector2u(win_w, win_h)) {
    //Initialize the window
    SetResolution(resolution);

    //Initialize the hud view
    hud = window.getView();

    //Initialize the camera
    camera.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));

    //Initialize the fonts
    if (!default_font.openFromFile("assets/Fonts/m5x7.ttf"))
        cerr << "Failed to load font 'm5x7'!" << endl;

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
}

//Handle SFML events
void Game::HandleEvents() {
    while (const optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();

        else if (event->is<sf::Event::FocusLost>()) {
            auto scn = active_scene.lock();
            if (scn->label != Scenes::TITLE) {
                paused = true;
                if (!scn->MenuOpen(Menus::PAUSE))
                    scn->OpenMenu(Menus::PAUSE);
            }
        }
    }
}

//Process input
void Game::ProcessInput() {

    if (auto scene = active_scene.lock())
        scene->GetInput();
    else
        cerr << "ERROR: ACTIVE SCENE NO LONGER VALID!!!" << endl;

    //Putting this here makes shit work so fuck it, we ball
    Input::UpdateVars();
}

//Update the game world
void Game::Update() {
    //if (!--debug_timer) {
    //    debug_timer = fps * 3;
    //}
    
    if (++frames_elapsed > fps) frames_elapsed = 0;

    //Close the old scene if needed
    if (auto old_scn = old_scene.lock())
        old_scn->Close();

    auto scene = active_scene.lock();
    scene->Update();
}

//Draw the game world
void Game::Render() {
    
    window.clear();

    if (auto scene = active_scene.lock())
        scene->Draw();
    else
        cerr << "ERROR: ACTIVE SCENE NO LONGER VALID!" << endl;

    window.display();
}

void Game::SetScene(Scenes scn) {
    if (scenes.find(scn) != scenes.end()) {

        old_scene = active_scene;

        //Open the new scene
        active_scene = scenes[scn];
        auto scene = active_scene.lock();
        if (auto o_scn = old_scene.lock())
            scene->SetPartyMems(o_scn->GetPartyMems());
        scene->Open();

        //Old scene *must* be closed next frame!
    }
    else
        cout << "That Scene does not exist!" << endl;
}

void Game::SetMusicVolume(float n_v) {
    Util::Clamp(n_v, 0, 100);
    music_volume = n_v;
    dj.SetVolume(music_volume);
}

void Game::SetSFXVolume(float n_v) {
    Util::Clamp(n_v, 0, 100);
    sfx_volume = n_v;
}

void Game::SetResolution(float res_scalar) {
    //Minimum resolution is 640 x 360
    if (res_scalar > 0) {
        float new_win_w = res_scalar * MINRESW;
        float new_win_h = res_scalar * MINRESH;
        while (new_win_w > SCREENW() or new_win_h > SCREENH()) {
            --res_scalar;
            new_win_w = res_scalar * MINRESW;
            new_win_h = res_scalar * MINRESH;
        }
        resolution.x = new_win_w;
        resolution.y = new_win_h;
    }
    else {
        res_scalar = 1;
        resolution.x = MINRESW;
        resolution.y = MINRESH;
    }

    window.close();
    if (resolution.x == SCREENW() and resolution.y == SCREENH())
        window.create(sf::VideoMode(resolution), "Seeds of Apocrypha", sf::Style::Default, sf::State::Fullscreen);
    else
        window.create(sf::VideoMode(resolution), "Seeds of Apocrypha", sf::Style::Default);
    window.setFramerateLimit(fps);

    auto scene = active_scene.lock();
    scene->ResizeMenus();
}

void Game::SetResolution(sf::Vector2u n_r) {
    if (n_r.x > 0 and n_r.y > 0) {
        n_r.x = n_r.x <= SCREENW() ? n_r.x : SCREENW();
        n_r.y = n_r.y <= SCREENH() ? n_r.y : SCREENH();

        resolution = n_r;

        window.close();
        if (resolution.x == SCREENW() and resolution.y == SCREENH())
            window.create(sf::VideoMode(resolution), "Seeds of Apocrypha", sf::Style::Default, sf::State::Fullscreen);
        else
            window.create(sf::VideoMode(resolution), "Seeds of Apocrypha", sf::Style::Default);
        window.setFramerateLimit(fps);

        auto scene = active_scene.lock();
        //scene->ResizeMenus();
    }
}
