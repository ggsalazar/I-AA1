#pragma once
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


using uint = unsigned int;
template <typename T>
std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& vec) { return os << vec.x << ", " << vec.y; }

//Resolution
constexpr sf::Vector2u MINRES = { 640, 360 };
inline sf::Vector2u SCREENSIZE() { return sf::VideoMode::getDesktopMode().size; }

//Tile Size
constexpr uint TS = 16;
constexpr uint M = 32; //Meter

//Mouse input
#define MOUSEPOS sf::Mouse::getPosition(window)
#define MOUSEPOS_E sf::Mouse::getPosition(engine.window)
#define MOUSEPOS_W window.mapPixelToCoords(sf::Mouse::getPosition(window))
#define MOUSEPOS_W_E engine.window.mapPixelToCoords(sf::Mouse::getPosition(engine.window))
constexpr sf::Mouse::Button LMB = sf::Mouse::Button::Left;
constexpr sf::Mouse::Button RMB = sf::Mouse::Button::Right;
inline bool BUTTONDOWN(sf::Mouse::Button button) { return sf::Mouse::isButtonPressed(button); }

//Letters
constexpr sf::Keyboard::Key A_K = sf::Keyboard::Key::A;
constexpr sf::Keyboard::Key C_K = sf::Keyboard::Key::C;
constexpr sf::Keyboard::Key D_K = sf::Keyboard::Key::D;
constexpr sf::Keyboard::Key I_K = sf::Keyboard::Key::I;
constexpr sf::Keyboard::Key J_K = sf::Keyboard::Key::J;
constexpr sf::Keyboard::Key M_K = sf::Keyboard::Key::M;
constexpr sf::Keyboard::Key O_K = sf::Keyboard::Key::O;
//Special keys
//Other
constexpr sf::Keyboard::Key LCTRL = sf::Keyboard::Key::LControl; constexpr sf::Keyboard::Key RCTRL = sf::Keyboard::Key::RControl;
constexpr sf::Keyboard::Key LSHIFT = sf::Keyboard::Key::LShift; constexpr sf::Keyboard::Key RSHIFT = sf::Keyboard::Key::RShift;
constexpr sf::Keyboard::Key TAB = sf::Keyboard::Key::Tab;
//Arrows
constexpr sf::Keyboard::Key UP = sf::Keyboard::Key::Up;
constexpr sf::Keyboard::Key DOWN = sf::Keyboard::Key::Down;
constexpr sf::Keyboard::Key LEFT = sf::Keyboard::Key::Left;
constexpr sf::Keyboard::Key RIGHT = sf::Keyboard::Key::Right;
inline bool BUTTONDOWN(sf::Keyboard::Key button) { return sf::Keyboard::isKeyPressed(button); }