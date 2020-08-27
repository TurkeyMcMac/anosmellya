#ifndef ANOSMELLYA_ASSERTIONS_H_
#define ANOSMELLYA_ASSERTIONS_H_

// The following assertions make sure Anosmellya doesn't compile on platforms it
// is unfixably incompatible with.

#include <limits>

static_assert(std::numeric_limits<float>::has_quiet_NaN,
    "Quiet NaN support is required for calculations");

#endif /* ANOSMELLYA_ASSERTIONS_H_ */
