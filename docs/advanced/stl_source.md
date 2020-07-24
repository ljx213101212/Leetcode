
## typedef Compare key_compare;

### value_compare
libc++/trunk/include/map::class _LIBCPP_TEMPLATE_VIS

```cpp
class _LIBCPP_TEMPLATE_VIS value_compare
        : public binary_function<value_type, value_type, bool>
    {
        friend class map;
    protected:
        key_compare comp;

        _LIBCPP_INLINE_VISIBILITY value_compare(key_compare c) : comp(c) {}
    public:
        _LIBCPP_INLINE_VISIBILITY
        bool operator()(const value_type& __x, const value_type& __y) const
            {return comp(__x.first, __y.first);}
    };
```

### std::less  
libc++/trunk/include/__functional_base
```cpp
#if _LIBCPP_STD_VER > 11
template <class _Tp = void>
#else
template <class _Tp>
#endif
struct _LIBCPP_TEMPLATE_VIS less : binary_function<_Tp, _Tp, bool>
{
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator()(const _Tp& __x, const _Tp& __y) const
        {return __x < __y;}
};

```