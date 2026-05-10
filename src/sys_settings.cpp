#include "sys_settings.hpp"
#include "types.hpp"

// Comment this line to use system settings:
#define TEST_OVERRIDE

#ifdef TEST_OVERRIDE

f32 double_click_time()
{ return 0.2f; }
Theme system_theme()
{ return THEME_DARK; }

#else
#ifdef WIN32
#error Not implemented yet.
#else
#error Not implemented yet.
#endif
#endif


