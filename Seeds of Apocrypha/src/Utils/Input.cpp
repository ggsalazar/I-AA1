#include "Input.h"

void Input::UpdateVars() {
    //Mouse
    {
        lmb_was_up = !LMBDOWN();
        lmb_was_down = LMBDOWN();
    }

    //Letters

    //Special keys
    {
        tab_was_up = !TABDOWN();
        tab_was_down = TABDOWN();
    }
}

bool Input::KeyPressed(sf::Keyboard::Key key) {
    switch (key) {
        case TAB:
            return tab_was_up and TABDOWN();
        break;
    }
    return false;
}

bool Input::KeyPressed(sf::Mouse::Button mb) {
    switch (mb) {
        case LMB:
            return lmb_was_up and LMBDOWN();
        break;
    }
    return false;
}

bool Input::KeyReleased(sf::Keyboard::Key key) {
    return false;
}

bool Input::KeyReleased(sf::Mouse::Button mb) {
    switch (mb) {
        case LMB:
            return lmb_was_down and !LMBDOWN();
        break;
    }
    return false;
}
