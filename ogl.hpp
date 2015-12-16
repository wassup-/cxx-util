#ifndef CXX_UTIL_OGL_HPP_
#define CXX_UTIL_OGL_HPP_

#include <type_traits>

#include <GL/gl.h>

#define EXPECT_SEMICOLON static_assert(true, "this should not happen")

namespace ogl
{

namespace detail
{

template<typename>
struct dispatcher;

template<typename T>
using enable_if_fp = std::enable_if<std::is_floating_point<T>::value>;

#define OGL_TYPED_DISPATCHER(T, PF) \
template<> \
struct dispatcher<T> \
{ \
  static void vertex(T x, T y) { glVertex2##PF(x, y); } \
  static void vertex(T x, T y, T z) { glVertex3##PF(x, y, z); } \
  static void vertex(T x, T y, T z, T w) { glVertex4##PF(x, y, z, w); } \
  \
  static void color(T x, T y, T z) { glColor3##PF(x, y, z); } \
  static void color(T x, T y, T z, T a) { glColor4##PF(x, y, z, a); } \
  \
  static void normal(T x, T y, T z) { glNormal3##PF(x, y, z); } \
  \
  template<typename U = T> \
  static typename enable_if_fp<U>::type rotate(U x, U y, U z) { \
    glRotate##PF(x, 1, 0, 0); \
    glRotate##PF(y, 0, 1, 0); \
    glRotate##PF(z, 0, 0, 1); \
  } \
}; EXPECT_SEMICOLON

OGL_TYPED_DISPATCHER(short, s);
OGL_TYPED_DISPATCHER(int, i);
OGL_TYPED_DISPATCHER(float, f);
OGL_TYPED_DISPATCHER(double, d);

#undef OGL_TYPED_DISPATCHER
#undef EXPECT_SEMICOLON

} // namespace detail

// COLOR
template<typename T, typename U, typename V>
inline static void color(T x, U y, V z)
{
  using common_type = typename std::common_type<T, U, V>::type;
  return detail::dispatcher<common_type>::color(static_cast<common_type>(x),
                                                static_cast<common_type>(y),
                                                static_cast<common_type>(z));
}

template<typename T, typename U, typename V, typename W>
inline static void color(T x, U y, V z, W a)
{
  using common_type = typename std::common_type<T, U, V, W>::type;
  return detail::dispatcher<common_type>::color(static_cast<common_type>(x),
                                                static_cast<common_type>(y),
                                                static_cast<common_type>(z),
                                                static_cast<common_type>(a));
}

// NORMAL
template<typename T, typename U, typename V>
inline static void normal(T x, U y, V z)
{
  using common_type = typename std::common_type<T, U, V>::type;
  return detail::dispatcher<common_type>::normal(static_cast<common_type>(x),
                                                 static_cast<common_type>(y),
                                                 static_cast<common_type>(z));
}

// VERTEX
template<typename T, typename U>
inline static void vertex(T x, U y)
{
  using common_type = typename std::common_type<T, U>::type;
  return detail::dispatcher<common_type>::vertex(static_cast<common_type>(x),
                                                 static_cast<common_type>(y));
}

template<typename T, typename U, typename V>
inline static void vertex(T x, U y, V z)
{
  using common_type = typename std::common_type<T, U, V>::type;
  return detail::dispatcher<common_type>::vertex(static_cast<common_type>(x),
                                                 static_cast<common_type>(y),
                                                 static_cast<common_type>(z));
}

// ROTATE
template<typename T, typename U, typename V>
inline static void rotate(T x, U y, V z)
{
  using common_type = typename std::common_type<T, U, V>::type;
  return detail::dispatcher<common_type>::rotate(static_cast<common_type>(x),
                                                 static_cast<common_type>(y),
                                                 static_cast<common_type>(z));
}

} // namespace ogl

#endif // CXX_UTIL_OGL_HPP_
