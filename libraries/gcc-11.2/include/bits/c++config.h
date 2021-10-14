#include <stddef.h>
#if __cplusplus >= 201703L
#define _GLIBCXX20_CONSTEXPR constexpr
#endif

#define _GLIBCXX20_DEPRECATED(x)
#define _GLIBCXX_VISIBILITY(default)
#define _GLIBCXX_DEPRECATED_SUGGEST(str)
namespace std {
using size_t = ::size_t;
using nullptr_t = decltype(nullptr);
}
