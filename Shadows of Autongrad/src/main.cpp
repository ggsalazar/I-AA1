#include "Game/Game.h"

int main() {

    //Initialize
    Game game("Shadows of Autongrad", 60);

    //Game loop
    while (game.running)
        game.Run();

    return 0;
}