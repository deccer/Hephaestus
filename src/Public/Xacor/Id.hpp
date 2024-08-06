#pragma once

#include <cstdint>

template<class TTag>
struct SIdImpl {
    enum class Id : std::size_t { Invalid = SIZE_MAX };

    bool operator==(const SIdImpl&) const noexcept = default;
};

template<class TTag>
using SId = typename SIdImpl<TTag>::Id;

template<class TTag>
struct SIdGenerator {
public:
    auto Next() -> SId<TTag> {
        _counter += 1;
        return { _counter };
    }
private:
    std::size_t _counter = 0;
};