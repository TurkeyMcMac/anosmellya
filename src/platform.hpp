#ifndef ANOSMELLYA_PLATFORM_H_
#define ANOSMELLYA_PLATFORM_H_

// The following definitions are to paper over platform differences.

#ifndef _WIN32
#include <inttypes.h>
#endif

// PRIu64 should be the printf format for printing 64-bit unsigned integers.
#ifdef _WIN32
// Fix issue caused by Windows' stupid non-compliance:
#define ANOSMELLYA_UINT64_FMT "I64u"
#else
#define ANOSMELLYA_UINT64_FMT PRIu64
#endif

#endif /* ANOSMELLYA_PLATFORM_H_ */
