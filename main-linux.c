/*
   08/16/2018
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

int main (void) {
  DIR *dp = NULL;
  FILE *fp = NULL;
  FILE *fp2 = NULL;
  char buf[256] = {""};
  unsigned int pidd = 0U;
  char pid_buf[256] = {""};
  char fp2_buf[256] = {""};
  struct dirent *entry = NULL;     

  pidd = (unsigned int)getpid();
  snprintf(pid_buf, 255, "%u", pidd);

  if (NULL == (dp = opendir("/proc"))){
    goto err;
  }
  if (NULL == (fp = fopen("/tmp/log", "a"))) {
    goto err;
  }
  while ((entry = readdir(dp))) {
    if (!(isdigit((unsigned char)*(entry->d_name)))) {
      continue;
    }
    if (0 == (strcmp(entry->d_name, pid_buf))) {
      continue;
    }
    snprintf(fp2_buf, 255, "/proc/%s/cmdline", entry->d_name);

    if (NULL == (fp2 = fopen(fp2_buf, "r"))) {
      continue;
    }
    fscanf(fp2, "%255s", buf);
    if (EOF == (fclose(fp2))) {
      goto err;
    }
    if (0 == (strcmp(buf, ""))) {
      snprintf(fp2_buf, 255, "/proc/%s/status", entry->d_name);
      if (NULL == (fp2 = fopen(fp2_buf, "r"))) {
        buf[0] = '\0';
        continue;
      }
      fscanf(fp2, "%*s %255s", buf);
      if (EOF == (fclose(fp2))) {
        goto err;
      }
    }
    fp2 = NULL;
    fprintf(fp, "%s %s\n", entry->d_name, buf);
    buf[0] = '\0';
  }
  if (EOF == (fclose(fp))) {
    goto err;
  }
  fp = NULL;
  if (-1 == (closedir(dp))) {
    goto err;
  }

  return EXIT_SUCCESS;

err:
  if (NULL != fp2) {
    fclose(fp2);
  }
  if (NULL != fp) {
    fclose(fp);
  }
  if (NULL != dp) {
    closedir(dp);
  }
  return EXIT_FAILURE;
}
