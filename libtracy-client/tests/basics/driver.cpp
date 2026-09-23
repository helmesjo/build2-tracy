#include <cstring>

#include <tracy/Tracy.hpp>
#include <common/TracySystem.hpp>

#undef NDEBUG
#include <cassert>

int main ()
{
  // Thread name round trip (non-inline functions).
  //
  tracy::SetThreadName ("driver");

#ifdef TRACY_ENABLE
  assert (std::strcmp (tracy::GetThreadName (tracy::GetThreadHandle ()),
                       "driver") == 0);
#endif

  // Instrumentation macros.
  //
  for (int i (0); i != 3; ++i)
  {
    ZoneScopedN ("iteration");
    TracyMessageL ("message");
    FrameMark;
  }
}
