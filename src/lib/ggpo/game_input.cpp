/* -----------------------------------------------------------------------
 * GGPO.net (http://ggpo.net)  -  Copyright 2009 GroundStorm Studios, LLC.
 *
 * Use of this software is governed by the MIT license that can be found
 * in the LICENSE file.
 */

#include "types.h"
#include "game_input.h"
#include "log.h"

void
GameInput::init(int iframe, char *ibits, int isize, int offset)
{
   ASSERT(isize);
   ASSERT(isize <= GAMEINPUT_MAX_BYTES);
   frame = iframe;
   size = isize;
   memset(bits, 0, sizeof(bits));
   if (ibits) {
      memcpy(bits + (offset * isize), ibits, isize);
   }
}

void
GameInput::init(int iframe, char *ibits, int isize)
{
   ASSERT(isize);
   ASSERT(isize <= GAMEINPUT_MAX_BYTES * GAMEINPUT_MAX_PLAYERS);
   frame = iframe;
   size = isize;
   memset(bits, 0, sizeof(bits));
   if (ibits) {
      memcpy(bits, ibits, isize);
   }
}

void
GameInput::desc(char *buf, size_t buf_size, bool show_frame) const
{
   ASSERT(size);
   size_t remaining = buf_size;
   if (show_frame) {
      #if defined(_WINDOWS)
      remaining -= snprintf(buf, buf_size, "(frame:%d size:%d ", frame, size);
      #else
      remaining -= sprintf(buf, "(frame:%d size:%d \0", frame, size);
      #endif
   } else {
      #if defined(_WINDOWS)
      remaining -= snprintf(buf, buf_size, "(size:%d ", size);
      #else
      remaining -= sprintf(buf, "(size:%d \0", size);
      #endif
   }

   for (int i = 0; i < size * 8; i++) {
      char buf2[16];
      if (value(i)) {
         #if defined(_WINDOWS)
         int c = snprintf(buf2, ARRAY_SIZE(buf2), "%2d ", i);
         strncat_s(buf, remaining, buf2, ARRAY_SIZE(buf2));
         #else
         int c = sprintf(buf2, "%2d \0", i);
         strncat(buf, buf2, ARRAY_SIZE(buf2));
         #endif
         remaining -= c;
      }
   }
   #if defined(_WINDOWS)
   strncat_s(buf, remaining, ")", 1);
   #else
   strncat(buf, ")", 1);
   #endif
}

void
GameInput::log(char *prefix, bool show_frame) const
{
	char buf[1024];
   size_t c = strlen(prefix);
   #if defined(_WINDOWS)
	strcpy_s(buf, prefix);
	desc(buf + c, ARRAY_SIZE(buf) - c, show_frame);
   strncat_s(buf, ARRAY_SIZE(buf) - strlen(buf), "\n", 1);
   #else
   strcpy(buf, prefix);
	desc(buf + c, ARRAY_SIZE(buf) - c, show_frame);
   strncat(buf, "\n", 1);
   #endif
	Log(buf);
}

bool
GameInput::equal(GameInput &other, bool bitsonly)
{
   if (!bitsonly && frame != other.frame) {
      Log("frames don't match: %d, %d\n", frame, other.frame);
   }
   if (size != other.size) {
      Log("sizes don't match: %d, %d\n", size, other.size);
   }
   if (memcmp(bits, other.bits, size)) {
      Log("bits don't match\n");
   }
   ASSERT(size && other.size);
   return (bitsonly || frame == other.frame) &&
          size == other.size &&
          memcmp(bits, other.bits, size) == 0;
}

