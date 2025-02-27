
#include "Game.h"
#include "Scene.h"
#include "Math.h"
#include "Utils/Input.h"

Game::Game(const char* title, uint init_fps) :
            fps(init_fps), debug_timer(fps) {
    //Initialize the window
    float r_scalar = (float)SCREENSIZE().x / (float)MINRES.x;
    SetResolution(floor(r_scalar));
    resolution = sf::Vector2u(floor(r_scalar) * sf::Vector2f(MINRES));

    //Initialize the camera
    camera.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    camera.setCenter({ window.getSize().x * .5f, window.getSize().y * .5f });
    window.setView(camera);

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
    if (!--debug_timer) {
        debug_timer = fps * 3;
    }
    
    if (++frames_elapsed > fps) frames_elapsed = 0;

    //Close the old scene if needed
    if (auto old_scn = old_scene.lock())
        old_scn->Open(false);

    auto scene = active_scene.lock();
    scene->Update();

    window.setMouseCursor(*cursor);
}

//Draw the game world
void Game::Render() {
    
    window.clear();

    if (auto scene = active_scene.lock())
        scene->Draw();
    else
        cerr << "ERROR: ACTIVE SCENE NO LONGER VALID!" << endl;

    if (debug) {
        sf::RectangleShape cam_pos;
        cam_pos.setPosition({ camera.getCenter().x - 2, camera.getCenter().y - 2 });
        cam_pos.setSize({ 4, 4 });
        cam_pos.setFillColor({ 0, 0, 255, 200 });
        window.draw(cam_pos);
    }


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
    Math::Clamp(n_v, 0, 100);
    music_volume = n_v;
    dj.SetVolume(music_volume);
}

void Game::SetSFXVolume(float n_v) {
    Math::Clamp(n_v, 0, 100);
    sfx_volume = n_v;
    auto scene = active_scene.lock();
    scene->SetEntitySFXVolume(sfx_volume);
}

void Game::SetResolution(float res_scalar) {
    //Minimum resolution is 640 x 360
    if (res_scalar > 0) {
        sf::Vector2u new_win_size = sf::Vector2u(res_scalar * sf::Vector2f(MINRES));
        while (new_win_size.x > SCREENSIZE().x or new_win_size.y > SCREENSIZE().y) {
            --res_scalar;
            new_win_size = sf::Vector2u(res_scalar * sf::Vector2f(MINRES));
        }
        resolution = new_win_size;
    }
    else {
        res_scalar = 1;
        resolution = MINRES;
    }

    window.close();
    if (resolution == SCREENSIZE())
        window.create(sf::VideoMode(resolution), "Seeds of Apocrypha", sf::Style::Close, sf::State::Fullscreen);
    else
        window.create(sf::VideoMode(resolution), "Seeds of Apocrypha", sf::Style::Close);
    window.setFramerateLimit(fps);

    if (auto scene = active_scene.lock())
        scene->ResizeMenus();
}

void Game::SetResolution(sf::Vector2u n_r) {
    if (n_r.x > 0 and n_r.y > 0) {
        n_r.x = n_r.x <= SCREENSIZE().x ? n_r.x : SCREENSIZE().x;
        n_r.y = n_r.y <= SCREENSIZE().y ? n_r.y : SCREENSIZE().y;

        resolution = n_r;

        window.close();
        if (resolution == SCREENSIZE())
            window.create(sf::VideoMode(resolution), "Seeds of Apocrypha", sf::Style::Close, sf::State::Fullscreen);
        else
            window.create(sf::VideoMode(resolution), "Seeds of Apocrypha", sf::Style::Close);
        window.setFramerateLimit(fps);

        if (auto scene = active_scene.lock())
            scene->ResizeMenus();
    }
}
