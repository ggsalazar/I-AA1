#pragma once
#include <memory>

template<typename T>
using u_ptr = std::unique_ptr<T>;
template<typename T, typename... Args>
constexpr auto make_u(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

template<typename T>
using s_ptr = std::shared_ptr<T>;
template<typename T, typename... Args>
constexpr auto make_s(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }