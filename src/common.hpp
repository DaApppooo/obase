#pragma once
#include <QRectF>
#include <memory>
#include <qpalette.h>
#include <QSize>
#define private public

extern QPalette palette;

constexpr QRectF size2rect(QSize size)
{
    return QRectF(0, 0, size.width(), size.height());
}

template <class T>
using ptr = std::unique_ptr<T>;

template <class T>
struct bad_ptr
{
  T* _ptr;

  bad_ptr() : _ptr(nullptr) {}
  bad_ptr(T* data) : _ptr(data) {}

  T* get() { return _ptr; }

  T* operator -> () { return _ptr; }
  T* operator -> () const { return _ptr; }
};

template <class T>
constexpr void force_move(T& src, T& dst)
{
  std::copy(
    reinterpret_cast<char*>(&src),
    reinterpret_cast<char*>(1+&src),
    reinterpret_cast<char*>(&dst)
  );
  std::construct_at(&src);
}
