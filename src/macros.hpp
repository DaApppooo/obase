#ifndef H_MACROS
#define H_MACROS
#include <cassert>
#include <functional>
#include <ostream>
#include <print>
#include <type_traits>
#include "types.hpp"
#include <generator>
#include <sstream>
#include <iostream>
#include <concepts>
#include <format>

#define _DEBUG
#ifdef _DEBUG
#define IFDEBUG(EXPR) EXPR
#else
#define IFDEBUG(EXPR)
#endif

// avoiding include of cstdlib for the random library
extern void* malloc(size_t);
extern void* realloc(void*, size_t);
extern void free(void*);

#define let const auto
#define mut auto
#define mod namespace
#define trait template <class T> concept
#define macro constexpr
#define yield co_yield

#ifdef __GNUG__
// true if compiled with the gcc toolchain
static macro bool __gnug__ = true;
#else
// false if not compiled with the gcc toolchain
static macro bool __gnug__ = false;
#endif

// allows you to get a pointer to a usually big prvalue.
template <class T>
  requires (sizeof(T) > 8)
macro const T* operator & (const T&& prval)
{ return &prval; }

template <class T>
inline macro T* alloc(u32 s)
{ return (T*)malloc(sizeof(T)*s); }
template <class T>
inline macro T* alloc(T* p, u32 s)
{
  if (p)
    return (T*)realloc(p, sizeof(T)*s);
  else
    return (T*)malloc(sizeof(T)*s);
}

template <class T>
macro std::vector<T>& operator << (std::vector<T>& vec, T&& val)
{ vec.push_back(val); return vec; }

macro size_t str_len_s(const char* s)
{ size_t c = 0; if (!s) return c; while (*s) { c++; s++; }; return c; }

#define FORMATTER(T, FUNC) \
template <> \
struct std::formatter<T> \
{ \
  template<class ParseContext> \
  constexpr ParseContext::iterator parse(ParseContext& ctx) \
  { \
    auto it = ctx.begin(); \
    if (it == ctx.end()) \
      return it; \
         \
    while (it != ctx.end() && *it != '}') \
      ++it; \
         \
    return it; \
  } \
 \
  template<class FmtContext> \
  FmtContext::iterator format(const T& obj, FmtContext& ctx) const \
  { \
    std::ostringstream out; \
    (FUNC)(out, obj); \
    return std::ranges::copy(std::move(out).str(), ctx.out()).out; \
  } \
}



FORMATTER(Rect, [](std::ostream& out, const Rect& r)
{
  out << "Rect("
      << r.x << ", "
      << r.y << ", "
      << r.width << ", "
      << r.height << ")";
});

FORMATTER(Vec2, [](std::ostream& out, const Vec2& r)
{
  out << "Vec2("
      << r.x << ", "
      << r.y << ")";
});

inline void __consteval_fail() {}

using std::print;
using std::println;
template <class T>
using generator = std::generator<T>;

template <class Sig>
struct FuncSig;

template <class R, class C, class... A>
struct FuncSig<R(C::*)(A...)>
{
  using rtype = R;
  using method_of = C;
};

static macro struct _DefaultType
{
  using Self = _DefaultType;
  macro _DefaultType() = default;
  macro _DefaultType(const _DefaultType&) = default;
  macro _DefaultType(_DefaultType&&) = default;
  macro Self& operator = (const _DefaultType&) = default;
  macro Self& operator = (_DefaultType&&) = default;
  template <class T>
  macro _DefaultType(const T&) {}
  template <class T>
  macro const _DefaultType& operator = (const T&) const { return *this; }
} _;

#ifdef FINAL
#define need(test) [[assume(test)]]
#else
#define need(test) \
  std::is_constant_evaluated() ? \
    ((test) ? void(0) : __consteval_fail()) \
    : assert(test)
#endif

template<typename T, size_t... I>
auto _tuple_reverse_impl(T&& t, std::index_sequence<I...>)
{
  return std::make_tuple(std::get<sizeof...(I) - 1 - I>(std::forward<T>(t))...);
}

template<typename T>
auto _tuple_reverse(T&& t)
{
  return _tuple_reverse_impl(std::forward<T>(t),
                      std::make_index_sequence<std::tuple_size<T>::value>());
}

// UNSTABLE UTILITY (unavailable for non-gcc compilers)
static macro struct {
  template <class... Ts>
  struct TiePack
  {
    std::tuple<Ts...> _pack;
    macro TiePack() = default;
    macro TiePack(const TiePack&) = default;
    macro TiePack(TiePack&&) = default;
    macro TiePack(Ts... args)
      : _pack(args...) {}
    macro TiePack& operator = (const TiePack&) = default;
    macro TiePack& operator = (TiePack&&) = default;
    template <class U>
      requires (sizeof(U) == sizeof(std::tuple<typename std::remove_reference<Ts>::type...>))
    macro TiePack& operator = (const U& other)
      requires __gnug__ // tuple layout is implementation specific
    {
      auto into_tuple = *reinterpret_cast<
        const std::tuple<typename std::remove_reference<Ts>::type...>*
      >(&other);
      auto reversed_vers = _tuple_reverse(std::move(into_tuple));
      _pack = reversed_vers;
      return *this;
    }
  };
  template <class... Ts>
  macro TiePack<Ts&...> operator [] (Ts&... args) const
  { return TiePack<Ts&...>(args...); }
} tie;

#endif
