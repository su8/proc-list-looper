/*
   09/03/2018

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
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/user.h>
#include <kvm.h>

int main (void) {
  struct kinfo_proc *kp = NULL;
  int x = 1;
  int count = 0;
  FILE *fp = NULL;
  kvm_t *kd = kvm_open(NULL, NULL, NULL, KVM_NO_FILES, NULL);

  if (NULL == kd) {
    return EXIT_FAILURE;
  }
  if (NULL == (fp = fopen("/tmp/log", "a"))) {
    goto err;
  }

  kp = kvm_getprocs(kd, KERN_PROC_ALL, 0, sizeof(*kp), &count);
  for (; x < count; x++) {
    fprintf(fp, "%d %s\n", kp[x].p_pid, kp[x].p_comm);
  }

err:
  if (NULL != fp) {
    fclose(fp);
  }
  kvm_close(kd);
  return EXIT_SUCCESS;
}
