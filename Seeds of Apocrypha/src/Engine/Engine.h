#pragma once
#include <memory>
//#include "Core/Collision.h"
#include "Core/Enums.h"
//#include "Core/Input.h"
#include "Core/Math.h" //Includes Vector2 (& aliases for unsigned int and namespace std)
//#include "Core/Vector3.h"
#include "Core/Geometry.h" //Includes Vector2
//#include "Entities/Entity.h"
//#include "Entities/UI/UI.h"
//#include "Entities/UI/Button.h"
//#include "Entities/UI/Confirm.h"
//#include "Entities/UI/Picker.h"
//#include "Entities/UI/Slider.h"
//#include "Entities/UI/Toggle.h"
//#include "Entities/UI/Tooltip.h"
#include "Graphics/Renderer.h" //Includes Sprite (& Spritesheet) and Text (& Font)
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