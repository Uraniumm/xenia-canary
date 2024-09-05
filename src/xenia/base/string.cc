/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2020 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/base/string.h"
#include <string.h>
#include <algorithm>
#include <locale>
#include "xenia/base/cvar.h"
#include "xenia/base/logging.h"
#include "xenia/base/platform.h"

#if !XE_PLATFORM_WIN32
#include <strings.h>
#endif  // !XE_PLATFORM_WIN32

#define UTF_CPP_CPLUSPLUS 201703L
#include "third_party/utfcpp/source/utf8.h"

namespace utfcpp = utf8;
DEFINE_bool(megahalogame_utf16to8, false,
            "Catch the invalid_utf16 exception and return an empty string. "
            "Crash occurs when a player approaches interactable objects in "
            "MegaHaloGame/Backyard Heroes. Only for use with MHG/BAM/Haggar.",
            "HACKS");
namespace xe {

int xe_strcasecmp(const char* string1, const char* string2) {
#if XE_PLATFORM_WIN32
  return _stricmp(string1, string2);
#else
  return strcasecmp(string1, string2);
#endif  // XE_PLATFORM_WIN32
}

int xe_strncasecmp(const char* string1, const char* string2, size_t count) {
#if XE_PLATFORM_WIN32
  return _strnicmp(string1, string2, count);
#else
  return strncasecmp(string1, string2, count);
#endif  // XE_PLATFORM_WIN32
}

char* xe_strdup(const char* source) {
#if XE_PLATFORM_WIN32
  return _strdup(source);
#else
  return strdup(source);
#endif  // XE_PLATFORM_WIN32
}

std::string to_utf8(const std::u16string_view source) {
  if (cvars::megahalogame_utf16to8) {
    try {
      return utfcpp::utf16to8(source);
    } catch (utfcpp::invalid_utf16& e) {
      XELOGE(
          "Attempted to convert an invalid UTF-16 string to UTF-8. Error: {}",
          e.what());
      std::string result;
      return result;
    }
  }
  return utfcpp::utf16to8(source);
}

std::u16string to_utf16(const std::string_view source) {
  return utfcpp::utf8to16(source);
}

}  // namespace xe
