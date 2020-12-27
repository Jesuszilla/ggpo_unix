/* -----------------------------------------------------------------------
 * GGPO.net (http://ggpo.net)  -  Copyright 2009 GroundStorm Studios, LLC.
 *
 * Use of this software is governed by the MIT license that can be found
 * in the LICENSE file.
 */

#include "platform_linux.h"

struct timespec Platform::start = { 0L };

uint32_t
Platform::GetCurrentTimeMS() {
    if (start.tv_sec == 0 && start.tv_nsec == 0) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        return 0;
    }
    struct timespec current;
    clock_gettime(CLOCK_MONOTONIC, &current);

    return ((current.tv_sec - start.tv_sec) * 1000) +
           ((current.tv_nsec  - start.tv_nsec ) / 1000000);
}

int
Platform::GetConfigInt(const char* name)
{
   char buf[1024];
   char *env = getenv(name);
   if (env == NULL) {
      return 0;
   }
   strcpy(buf, env);
   int res = atoi(buf);
   free(buf);
   return res;
}

bool Platform::GetConfigBool(const char* name)
{
   char buf[1024];
   char *env = getenv(name);
   if (env == NULL) {
      return false;
   }
   strcpy(buf, env);
   int res = atoi(buf);
   bool res2 = false;
   if (env != NULL)
      res2 = strcasecmp(buf, "true") == 0;
   free(buf);
   return res != 0 || res2;
}
