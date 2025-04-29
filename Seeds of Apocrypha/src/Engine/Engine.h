#pragma once
#include <memory>
#include "Core/Collision.h"
#include "Core/Enums.h"
#include "Core/Math.h" //Includes Vector2 (& alias for unsigned int)
//#include "Core/Vector3.h"
#include "Core/Geometry.h" //Includes Vector2
#include "Core/Entity.h"
#include "Graphics/Renderer.h" //Includes Sprite and Text (& Font)
//#include "Graphics/TileMap.h"
#include "Graphics/Window.h"
//#include "Audio/DJ.h"
//#include "Audio/Soundboard.h"

using namespace Engine;

template<typename T>
using u_ptr = std::unique_ptr<T>;
template<typename T, typename... Args>
constexpr auto make_u(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

template<typename T>
using s_ptr = std::shared_ptr<T>;
template<typename T, typename... Args>
constexpr auto make_s(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }

template<typename T>
using w_ptr = std::weak_ptr<T>;