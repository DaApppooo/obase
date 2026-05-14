#ifndef H_TYPES
#define H_TYPES
#include <cstdint>
#include <type_traits>
#include <memory>
#include "raylib.h"
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using f32 = float;
using f64 = double;
template <class T>
using Own = T;
template <class T>
using Ref = T;
template <class T>
using uptr = std::unique_ptr<T>;
using Rect = Rectangle;
using Vec2 = Vector2;

constexpr bool operator == (Rect a, Rect b)
{
  return a.x == b.x
         and a.y == b.y
         and a.width == b.width
         and a.height == b.height;
}

constexpr Color operator * (float x, Color c)
{ return { u8(x*c.r), u8(x*c.g), u8(x*c.b), u8(x*c.a) }; }

constexpr Color operator + (Color a, Color b)
{ return { u8(a.r+b.r), u8(a.g+b.g), u8(a.b+b.b), u8(a.a+b.a) }; }

constexpr Rect reduce(Rect r, f32 red)
{
  return {
    r.x + red,
    r.y + red,
    r.width - red*2.f,
    r.height - red*2.f
  };
}

template <class... Ts>
struct TypeSet
{
  template <typename A, typename B, typename... C>
  struct _index_of
    : std::integral_constant < std::size_t,
                               std::is_same<A, B>{} ? 0 : 1+_index_of<A, C...>{} >
  {};

  template <typename A, typename B>
  struct _index_of<A, B>
    : std::integral_constant<std::size_t, 0>
  {
      constexpr operator std::size_t() const
      {
          return   std::is_same<A, B>{}
                 ? 0
                 : UINT32_MAX;
      }
  };

  template<typename What, typename ... Args>
  struct _is_present {
      static constexpr bool value {(std::is_same_v<What, Args> || ...)};
  };

  template <class T>
  static constexpr std::size_t index_of = _index_of<T, Ts...>();
  template <class T>
  static constexpr bool is_present = _is_present<T, Ts...>::value;
  

};

#endif
