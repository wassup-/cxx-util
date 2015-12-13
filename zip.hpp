#ifndef CXX_UTIL_ZIP_HPP_
#define CXX_UTIL_ZIP_HPP_

#include <iterator>
#include <tuple>
#include <type_traits>

namespace range
{

template<int...>
struct index_sequence { };

template<int N, int ...I>
struct gen_indices : gen_indices<(N - 1), (N - 1), I...> { };
template<int ...I>
struct gen_indices<0, I...> : index_sequence<I...> { };

template<typename Range>
struct iterator_type { using type = typename Range::iterator; };
template<typename Range>
struct iterator_type<const Range> { using type = typename Range::const_iterator; };
template<typename Range>
struct iterator_type<Range &> : iterator_type<Range> { };

template<typename Range>
using iterator_type_t = typename iterator_type<Range>::type;

template<typename Iterator>
using iterator_value_type_t = typename std::iterator_traits<Iterator>::value_type;

template<typename Iterator>
using iterator_reference_t = typename std::iterator_traits<Iterator>::reference;

template<typename Iterator>
using iterator_pointer_t = typename std::iterator_traits<Iterator>::pointer;

template<typename Iterator>
using iterator_difference_type_t = typename std::iterator_traits<Iterator>::difference_type;

template<typename Iterator>
using iterator_category_t = typename std::iterator_traits<Iterator>::iterator_category;

namespace detail
{

template<typename U, typename ...T>
struct type_sequence_has : std::false_type { };
template<typename U, typename ...T>
struct type_sequence_has<U, U, T...> : std::true_type { };
template<typename U, typename V, typename ...T>
struct type_sequence_has<U, V, T...> : type_sequence_has<U, T...> { };

} // namespace detail

template<typename ...T>
struct type_sequence
{
  template<typename U>
  using has = typename detail::type_sequence_has<U, T...>::type;
};

template<typename... Tag>
struct common_iterator_category;
template<typename Tag>
struct common_iterator_category<Tag> { using type = Tag; };
template<typename Tag1, typename Tag2, typename ...Tag>
struct common_iterator_category<Tag1, Tag2, Tag...>
{
  using seq = type_sequence<Tag1, Tag2>;
  using common_tag =  std::conditional_t<
                        seq::template has<std::input_iterator_tag>::value,
                        std::input_iterator_tag,
                        std::conditional_t<
                          seq::template has<std::output_iterator_tag>::value,
                          std::output_iterator_tag,
                          std::conditional_t<
                            seq::template has<std::forward_iterator_tag>::value,
                            std::forward_iterator_tag,
                            std::conditional_t<
                              seq::template has<std::bidirectional_iterator_tag>::value,
                              std::bidirectional_iterator_tag,
                              std::conditional_t<
                                seq::template has<std::random_access_iterator_tag>::value,
                                std::random_access_iterator_tag,
                                void> > > > >;
  using type = typename common_iterator_category<common_tag, Tag...>::type;
};

template<typename ...Tag>
using common_iterator_category_t = typename common_iterator_category<Tag...>::type;


template<typename ...Iterator>
struct zipped_iterators
{
public:
  using base_type = std::iterator<
                      common_iterator_category_t<iterator_category_t<Iterator>...>,
                      std::tuple<iterator_reference_t<Iterator>...> >;
  using value_type = iterator_value_type_t<base_type>;
  using reference = iterator_reference_t<base_type>;
  using pointer = iterator_pointer_t<base_type>;
  using difference_type = iterator_difference_type_t<base_type>;
  using iterator_category = iterator_category_t<base_type>;

protected:
  using indices = gen_indices<sizeof...(Iterator)>;

public:
  zipped_iterators(Iterator ...iterator)
  : iterator_ { iterator... }
  { }

  bool operator==(const zipped_iterators &other) const
  { using std::get; return get<0>(iterator_) == get<0>(other.iterator_); }
  bool operator!=(const zipped_iterators &other) const
  { return !(*this == other); }

  zipped_iterators operator++(int)
  { zipped_iterators copy { *this }; increment(indices{}, 1); return copy; }
  zipped_iterators& operator++() { increment(indices{}, 1); return *this; }

  value_type operator*() { return dereference(indices{}); }

protected:
  template<int ...Idx>
  value_type dereference(index_sequence<Idx...>)
  {
    using std::get;

    return value_type{ *get<Idx>(iterator_)... };
  }

  template<int ...Idx>
  void increment(index_sequence<Idx...>, difference_type n)
  {
    using std::get;
    using std::advance;

    using expand = int[];
    (void)expand { (advance(get<Idx>(iterator_), n), void(), 0)... };
  }

  template<int ...Idx>
  void decrement(index_sequence<Idx...>, difference_type n)
  {
    using std::get;
    using std::advance;

    using expand = int[];
    (void)expand { (advance(get<Idx>(iterator_), -n), void(), 0)... };
  }

private:
  std::tuple<Iterator...> iterator_;
};

template<typename ...Iterator>
zipped_iterators<Iterator...> zip_iterators(Iterator ...iterator)
{ return { iterator... }; }

template<typename ...Range>
struct zipped_range
{
public:
  using iterator = zipped_iterators<iterator_type_t<Range>...>;

protected:
  using indices = gen_indices<sizeof...(Range)>;

public:
  zipped_range(Range &&...range)
  : range_ { range... }
  { }

  iterator begin() { return begin(indices{}); }
  iterator end() { return end(indices{}); }

protected:
  template<int ...Idx>
  iterator begin(index_sequence<Idx...>)
  { using std::get; using std::begin; return zip_iterators(begin(get<Idx>(range_))...); }
  template<int ...Idx>
  iterator end(index_sequence<Idx...>)
  { using std::get; using std::end; return zip_iterators(end(get<Idx>(range_))...); }

private:
  std::tuple<Range...> range_;
};

template<typename ...Range>
zipped_range<Range...> zip(Range &&...range)
{ return { std::forward<Range>(range)... }; }

} // namespace range

#endif
