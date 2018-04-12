/*
 * ufelica main
 * 
 */

#include <stdio.h>
#include <usb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // for getopt

#include "main.h"
#include "ufelica.h"

/*******************************************************************/
/* main part                                                       */
/*******************************************************************/

static void
usage(int retcode)
{
  fprintf(stderr,
	  "usage: %s [-vVl] [-s sec] [-t type]\n", program_name);
  fprintf(stderr, "-v: verbose mode\n");
  fprintf(stderr, "-V: show version\n");
  fprintf(stderr, "-l: loop mode\n");
  fprintf(stderr, "-s: set sleep time(sec) when in the loop mode\n");
  fprintf(stderr, "type:\n");
  fprintf(stderr, "  0 : GET_ID_EDY (default)\n");
  fprintf(stderr, "  1 : GET_ID_SUICA\n");
  fprintf(stderr, "  2 : CTL_1\n");
  fprintf(stderr, "  3 : CTL_2\n");
  fprintf(stderr, "  4 : CTL_3\n");
  fprintf(stderr, "  5 : CTL_4\n");
  fprintf(stderr, "  6 : CTL_5\n");
  fprintf(stderr, "  7 : BREAK\n");
  
  exit(retcode);
}

int main(int argc, char **argv)
{
  int opt; // for getopt
  int ret;
  unsigned int sleep_time=1;

  struct usb_bus * u_bus;
  struct usb_device * u_dev = NULL;
  struct usb_dev_handle * u_handle = NULL;

  while ((opt = getopt (argc, argv, "hvVls:t:")) != -1) {
    switch(opt) {
    case 't':
      query_type = atoi(optarg);
      break;
    case 's':
      sleep_time = atoi(optarg);
      break;
    case 'h':
      show_help++;
      break;
    case 'v':
      option_verbose++;
      break;
    case 'l':
      option_loop = 1;
      break;
    case 'V':
      show_version++;
      break;
    default:
      usage(1);
    }
  }

  if (show_help) usage(0);

  if (show_version) { fprintf(stderr, "%s\n", version); exit(0); }

  u_bus = ufelica_init();

  if (option_verbose > 1) { printf ("usb_bus found.\n");}

  u_dev = ufelica_scan(u_bus, u_dev);

  if (option_verbose > 1) { printf ("usb_device found.\n");}

  if (option_verbose > 1) { 
    printf("%s/%s     %04X/%04X\n", u_bus->dirname, u_dev->filename,
	   u_dev->descriptor.idVendor, u_dev->descriptor.idProduct);
  }

  u_handle = ufelica_open(u_dev);
  
  if (option_verbose > 1) { printf("open successed...\n"); }
  
  if (option_verbose > 1) { printf ("Query Type = %d\n",query_type);}

  ret = ufelica_pasori_initalize(u_handle);
  if (option_verbose > 1) { printf ("Pasori Initializing: ret=%d\n",ret);}

  if (option_loop == 0){
    ufelica_get_id(u_handle);
  }else{ // loop mode
    for (;;){
      ufelica_get_id(u_handle);
      sleep(sleep_time);
    }
  }
  
  if (option_verbose >  1) { printf("begin closing...\n"); }
  
  ufelica_close(u_handle);
  return 0;
}

