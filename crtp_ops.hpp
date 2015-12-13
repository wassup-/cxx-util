#ifndef CXX_UTIL_CRTP_OPS_HPP_
#define CXX_UTIL_CRTP_OPS_HPP_

/**
 * Define CRTP_OPS_RECURSE to enable recursive casting
 */

namespace crtp
{

template<typename Value>
struct explicit_caster
{
  template<typename Class>
  static Value cast(const Class& c)
  { return static_cast<Value>(c); }
};

template<typename Derived,
         typename Value,
         typename Caster = explicit_caster<Value> >
struct ctrp_ops
{
private:
  friend Value extract_value(const ctrp_ops& self)
  {
    const Derived& d = static_cast<const Derived&>(self);
    return Caster::cast(d);
  }

public:
  friend bool operator==(const ctrp_ops& l, const ctrp_ops& r)
  {
    return (extract_value(l) == extract_value(r));
  }

  friend bool operator==(const ctrp_ops& l, Value r)
  {
    return (extract_value(l) == r);
  }

  friend bool operator==(Value l, const ctrp_ops& r)
  {
    return (l == extract_value(r));
  }

#if defined(CRTP_OPS_RECURSE)
  template<typename ODerived, typename OValue, typename OCaster>
  friend bool operator==(const ctrp_ops& l, const ctrp_ops<ODerived, OValue, OCaster>& r)
  {
    return (extract_value(l) == extract_value(r));
  }
#endif

  friend bool operator<(const ctrp_ops& l, const ctrp_ops& r)
  {
    return (extract_value(l) < extract_value(r));
  }

  friend bool operator<(const ctrp_ops& l, Value r)
  {
    return (extract_value(l) < r);
  }

  friend bool operator<(Value l, const ctrp_ops& r)
  {
    return (l < extract_value(r));
  }

#if defined(CRTP_OPS_RECURSE)
  template<typename ODerived, typename OValue, typename OCaster>
  friend bool operator<(const ctrp_ops& l, const ctrp_ops<ODerived, OValue, OCaster>& r)
  {
    return (extract_value(l) < extract_value(r));
  }
#endif

  // Relative operators (based on std::rel_ops)

  friend bool operator!=(const ctrp_ops& l, const ctrp_ops& r)
  { return !(l == r); }

  friend bool operator!=(const ctrp_ops& l, Value r)
  { return !(l == r); }

  friend bool operator!=(Value l, const ctrp_ops& r)
  { return !(l == r); }

#if defined(CRTP_OPS_RECURSE)
  template<typename ODerived, typename OValue, typename OCaster>
  friend bool operator!=(const ctrp_ops& l, const ctrp_ops<ODerived, OValue, OCaster>& r)
  { return (l != extract_value(r)); }
#endif

  friend bool operator>(const ctrp_ops& l, const ctrp_ops& r)
  { return (r < l); }

  friend bool operator>(const ctrp_ops& l, Value r)
  { return (r < l); }

  friend bool operator>(Value l, const ctrp_ops& r)
  { return (r < l); }

#if defined(CRTP_OPS_RECURSE)
  template<typename ODerived, typename OValue, typename OCaster>
  friend bool operator>(const ctrp_ops& l, const ctrp_ops<ODerived, OValue, OCaster>& r)
  { return (l > extract_value(r)); }
#endif

  friend bool operator<=(const ctrp_ops& l, const ctrp_ops& r)
  { return !(r < l); }

  friend bool operator<=(const ctrp_ops& l, Value r)
  { return !(r < l); }

  friend bool operator<=(Value l, const ctrp_ops& r)
  { return !(r < l); }

#if defined(CRTP_OPS_RECURSE)
  template<typename ODerived, typename OValue, typename OCaster>
  friend bool operator<=(const ctrp_ops& l, const ctrp_ops<ODerived, OValue, OCaster>& r)
  { return (l <= extract_value(r)); }
#endif

  friend bool operator>=(const ctrp_ops& l, const ctrp_ops& r)
  { return !(l < r); }

  friend bool operator>=(const ctrp_ops& l, Value r)
  { return !(l < r); }

  friend bool operator>=(Value l, const ctrp_ops& r)
  { return !(l < r); }

#if defined(CRTP_OPS_RECURSE)
  template<typename ODerived, typename OValue, typename OCaster>
  friend bool operator>=(const ctrp_ops& l, const ctrp_ops<ODerived, OValue, OCaster>& r)
  { return (l >= extract_value(r)); }
#endif
};

} // namespace crtp

#endif // CXX_UTIL_CRTP_OPS_HPP_
