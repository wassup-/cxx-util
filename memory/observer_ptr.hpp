#ifndef CXX_UTIL_MEMORY_OBSERVERPTR_
#define CXX_UTIL_MEMORY_OBSERVERPTR_

#include <iterator>
#include <functional>
#include <type_traits>
#include <utility>

namespace cxx
{

template<typename T>
struct observer_ptr;

template<typename T>
struct observer_ptr
{
public:
  static_assert(!std::is_reference<T>::value, "T may not be a reference type");

  using element_type = T;
  using pointer = typename std::add_pointer<T>::type;
  using const_pointer = typename std::add_pointer<const T>::type;
  using reference = typename std::add_lvalue_reference<T>::type;
  using const_reference = typename std::add_lvalue_reference<const T>::type;

public:
  constexpr observer_ptr() noexcept : ptr_ { nullptr } { }
  constexpr observer_ptr(std::nullptr_t) noexcept : ptr_ { nullptr } { }
  observer_ptr(pointer ptr) noexcept : ptr_{ ptr } { }

  template<typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
  observer_ptr(U *ptr) noexcept : ptr_{ ptr } { }

  observer_ptr(const observer_ptr&) = default;
  observer_ptr(observer_ptr&&) = default;

  observer_ptr& operator=(pointer ptr) noexcept { ptr_ = ptr; return *this; }
  template<typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
  observer_ptr& operator=(U *ptr) noexcept { ptr_ = ptr; return *this; }

  // observers
  pointer get() const noexcept { return ptr_; }
  pointer operator->() const noexcept { return ptr_; }
  reference operator*() const noexcept { return *ptr_; }
  explicit operator bool() const noexcept { return static_cast<bool>(ptr_); }

  // conversions
  explicit operator pointer() const noexcept { return get(); }
  explicit operator const_pointer() const noexcept { return get(); }

  // modifiers
  pointer release() noexcept { pointer ptr { ptr_ }; ptr_ = nullptr; return ptr; }
  void reset(pointer ptr = nullptr) noexcept { ptr_ = ptr; }
  void swap(observer_ptr &other) noexcept { using std::swap; swap(ptr_, other.ptr_); }

private:
  pointer ptr_;
}; // observer_ptr<T>

template<typename T, size_t N>
struct observer_ptr<T[N]>
{
public:
  static_assert(!std::is_reference<T>::value, "T may not be a reference type");

  using element_type = T;
  using pointer = typename std::add_pointer<T>::type;
  using const_pointer = typename std::add_pointer<const T>::type;
  using reference = typename std::add_lvalue_reference<T>::type;
  using const_reference = typename std::add_lvalue_reference<const T>::type;

  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

public:
  constexpr observer_ptr() noexcept : ptr_ { nullptr } { }
  constexpr observer_ptr(std::nullptr_t) noexcept : ptr_ { nullptr } { }
  observer_ptr(pointer ptr) noexcept : ptr_{ ptr } { }

  template<typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
  observer_ptr(U *ptr) noexcept : ptr_{ ptr } { }

  observer_ptr(const observer_ptr&) = default;
  observer_ptr(observer_ptr&&) = default;

  observer_ptr& operator=(pointer ptr) noexcept { ptr_ = ptr; return *this; }
  template<typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
  observer_ptr& operator=(U *ptr) noexcept { ptr_ = ptr; return *this; }

  // observers
  pointer get() const noexcept { return ptr_; }
  pointer operator->() const noexcept { return ptr_; }
  reference operator*() const noexcept { return *ptr_; }
  explicit operator bool() const noexcept { return static_cast<bool>(ptr_); }

  // conversions
  explicit operator pointer() const noexcept { return get(); }
  explicit operator const_pointer() const noexcept { return get(); }
  explicit operator observer_ptr<T>() const noexcept { return { get() }; }
  explicit operator observer_ptr<const T>() const noexcept { return { get() }; }

  // modifiers
  pointer release() noexcept { pointer ptr { ptr_ }; ptr_ = nullptr; return ptr; }
  void reset(pointer ptr = nullptr) noexcept { ptr_ = ptr; }
  void swap(observer_ptr &other) noexcept { using std::swap; swap(ptr_, other.ptr_); }

  // arithmetic
  observer_ptr& operator++() { ++ptr_; return *this; }
  observer_ptr operator++(int) { observer_ptr copy { *this }; ++ptr_; return copy; }
  observer_ptr& operator--() { --ptr_; return *this; }
  observer_ptr operator--(int) { observer_ptr copy { *this }; --ptr_; return copy; }
  observer_ptr operator+() const { return { ptr_ }; }
  observer_ptr operator+(difference_type d) const { return { ptr_ + d}; }
  observer_ptr operator-(difference_type d) const {  return { ptr_ - d }; }
  difference_type operator-(const observer_ptr &other) const { return { ptr_ - other.ptr_ }; }

  reference operator[](difference_type d) { return *ptr_[d]; }
  const_reference operator[](difference_type d) const { return *ptr_[d]; }
private:
  pointer ptr_;
}; // observer_ptr<T[N]>

template<typename T>
bool operator==(const observer_ptr<T> &left, const observer_ptr<T> &right)
{
  using pointer = typename observer_ptr<T>::pointer;
  using equal = std::equal_to<pointer>;
  return equal{}(left.get(), right.get());
}

template<typename T>
bool operator!=(const observer_ptr<T> &left, const observer_ptr<T> &right)
{
  return !(left == right);
}

template<typename T>
bool operator<(const observer_ptr<T> &left, const observer_ptr<T> &right)
{
  using pointer = typename observer_ptr<T>::pointer;
  using less = std::less<pointer>;
  return less{}(left.get(), right.get());
}

template<typename T>
bool operator<=(const observer_ptr<T> &left, const observer_ptr<T> &right)
{
  return !(right < left);
}

template<typename T>
bool operator>(const observer_ptr<T> &left, const observer_ptr<T> &right)
{
  return (right < left);
}

template<typename T>
bool operator>=(const observer_ptr<T> &left, const observer_ptr<T> &right)
{
 return !(left < right);
}


template<typename T>
void swap(observer_ptr<T> &left, observer_ptr<T> &right) { left.swap(right); }

template<typename T>
observer_ptr<T> make_observer(T *ptr) { return { ptr }; }

template<typename T, typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
observer_ptr<T> static_pointer_cast(const observer_ptr<U> &ptr)
{
  using pointer = typename std::add_pointer<T>::type;
  return { static_cast<pointer>(ptr.get()) };
}

template<typename T, typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
observer_ptr<T> dynamic_pointer_cast(const observer_ptr<U> &ptr)
{
  using pointer = typename std::add_pointer<T>::type;
  return { dynamic_cast<pointer>(ptr.get()) };
}

template<typename T, typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
observer_ptr<T> const_pointer_cast(const observer_ptr<U> &ptr)
{
  using pointer = typename std::add_pointer<T>::type;
  return { const_cast<pointer>(ptr.get()) };
}

} // namespace cxx

#endif // CXX_UTIL_MEMORY_OBSERVERPTR_
