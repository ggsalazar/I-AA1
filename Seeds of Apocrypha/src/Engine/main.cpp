#include "Game.h"

int main() {

    //Initialize
    Game game("Seeds of Apocrypha", 60);

    //Game loop
    while (game.window.isOpen()) {
        //Handle events
        game.HandleEvents();
        
        //Process Input
        game.ProcessInput();

        //Update game state
        game.Update();

        //Draw the gameS
        game.Render();
    }

    return 0;
}