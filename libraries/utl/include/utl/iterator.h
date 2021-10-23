#pragma once
#include "memory.h"
namespace utl {
template <class Container, class InputIt> class CycleIt {
public:
    CycleIt(Container& buffer, InputIt startIt) : buffer(buffer), workIt(startIt) {}
    virtual ~CycleIt() = default;

    /* gcc can be stupid at times and generate bad code if it chooses - for whatever reason - not to
     * inline this (verified with compiler explorer). Force inline */
    [[gnu::always_inline]] [[nodiscard]] InputIt get() const { return workIt; }

    [[gnu::always_inline]] CycleIt operator++(int) {
        CycleIt temp = *this;
        ++*this;
        return temp;
    }
    [[gnu::always_inline]] CycleIt& operator++() {
        if (++workIt == utl::end(buffer))
            workIt = utl::begin(buffer);
        return *this;
    }
    [[gnu::always_inline]] size_t operator-(const CycleIt& rhs) {
        if (workIt >= rhs.workIt)
            return (workIt - rhs.workIt);
        else
            return (utl::size(buffer) - (rhs.workIt - workIt));
    }
    [[gnu::always_inline]] [[nodiscard]] auto& operator*() const { return *workIt; }

protected:
    Container& buffer;
    InputIt workIt;
};

// template <class Container, class InputIt> class CycleIt_startFlag : public CycleIt<Container, InputIt>
// { public:
//     CycleIt_startFlag(Container& buffer, InputIt startIt, bool flag)
//         : CycleIt<Container, InputIt>(buffer, startIt), flag(flag) {}

// private:
//     bool flag;
// };

}  // namespace utl
