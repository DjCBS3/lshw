/*
 * abi.cc
 *
 *
 */

#include "version.h"
#include "abi.h"
#include "osutils.h"
#include <unistd.h>
#include <dirent.h>

__ID("@(#) $Id: mem.cc 1352 2006-05-27 23:54:13Z ezix $");

#define PROC_SYS "/proc/sys"

bool scan_abi(hwNode & system)
{
  pushd(PROC_SYS);

  if(exists("kernel/vsyscall64"))
  {
    system.addCapability("vsyscall64");
    system.setWidth(64);
  }

  if(chdir("abi") == 0)
  {
    int i,n;
    struct dirent **namelist;

    n = scandir(".", &namelist, selectfile, alphasort);
    for(i=0; i<n; i++)
    {
      system.addCapability(namelist[i]->d_name);
      free(namelist[i]);
    }
    if(namelist)
      free(namelist);
  }

  popd();

  system.describeCapability("vsyscall32", "32-bit processes");
  system.describeCapability("vsyscall64", "64-bit processes");
  return true;
}