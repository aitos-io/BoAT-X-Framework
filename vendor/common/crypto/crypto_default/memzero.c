#ifndef __STDC_WANT_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1  // C11's bounds-checking interface.
#endif
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef __unix__
#include <strings.h>
#include <sys/param.h>
#endif

void memzero(void *const pnt, const size_t len) {
  volatile unsigned char *volatile pnt_ = (volatile unsigned char *volatile)pnt;
  size_t i = (size_t)0U;

  while (i < len) {
    pnt_[i++] = 0U;
  }

  // explicitly mark the memory as overwritten for the Clang MemorySanitizer
  // this is only included at compile time if MemorySanitizer is enabled and
  // should not come with any downsides during regular builds
#if defined(__has_feature)
#if __has_feature(memory_sanitizer)
  memset(pnt, 0, len);
#endif
#endif
}
