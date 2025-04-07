#include "Game/Game.h"
int main() {

    //Initialize
    Game game("Seeds of Apocrypha", 60);

    //Game loop
    while (game.running)
        game.Run();

    return 0;
}