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

//Mouse input
#define MOUSEPOS sf::Mouse::getPosition(window)
#define MOUSEPOS_E sf::Mouse::getPosition(engine.window)
constexpr sf::Mouse::Button LMB = sf::Mouse::Button::Left;
constexpr sf::Mouse::Button RMB = sf::Mouse::Button::Right;
inline bool LMBDOWN() { return sf::Mouse::isButtonPressed(LMB); }
inline bool RMBDOWN() { return sf::Mouse::isButtonPressed(RMB); }

//Letters
constexpr sf::Keyboard::Key A_K = sf::Keyboard::Key::A;
constexpr sf::Keyboard::Key D_K = sf::Keyboard::Key::D;
inline bool ADOWN() { return sf::Keyboard::isKeyPressed(A_K); }
inline bool DDOWN() { return sf::Keyboard::isKeyPressed(D_K); }

//Special keys
//Other
constexpr sf::Keyboard::Key LCTRL = sf::Keyboard::Key::LControl; constexpr sf::Keyboard::Key RCTRL = sf::Keyboard::Key::RControl;
constexpr sf::Keyboard::Key LSHIFT = sf::Keyboard::Key::LShift; constexpr sf::Keyboard::Key RSHIFT = sf::Keyboard::Key::RShift;
constexpr sf::Keyboard::Key TAB = sf::Keyboard::Key::Tab;
inline bool CTRLDOWN() { return sf::Keyboard::isKeyPressed(LCTRL) or sf::Keyboard::isKeyPressed(RCTRL); }
inline bool SHIFTDOWN() { return sf::Keyboard::isKeyPressed(LSHIFT) or sf::Keyboard::isKeyPressed(RSHIFT); }
inline bool TABDOWN() { return sf::Keyboard::isKeyPressed(TAB); }
//Arrows
inline bool UADOWN() { return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up); }
inline bool DADOWN() { return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down); }
inline bool LADOWN() { return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left); }
inline bool RADOWN() { return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right); }