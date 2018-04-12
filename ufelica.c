#include <stdio.h>
#include <usb.h>
#include <string.h>

#ifndef _UFELICA_H_
#include "ufelica.h"
#endif // _UFELICA_H_

// config = 1 from bCOnfigurationValue
// interface = 0  from bInterfaceNumber
// altsetting = 0 from bAlternateSetting

extern int option_verbose;
extern int option_loop;
extern int query_type;
extern char *program_name;

struct usb_bus *
ufelica_init(){

  usb_init();
  usb_find_busses();
  usb_find_devices();

  return (usb_get_busses());
}

usb_dev_handle *
ufelica_open(struct usb_device *dev){

  struct usb_dev_handle *h_dev = NULL;

  if((h_dev = usb_open(dev)) == NULL){
    printf("%s\n","ufelica:usb_open:cannot open device\n");
    exit(0);
  }

  if(usb_set_configuration
     (h_dev,dev->config->bConfigurationValue) < 0){
    printf("%s\n",usb_strerror());
  }

  if(usb_claim_interface
     (h_dev,dev->config->interface->altsetting->bInterfaceNumber)< 0){
    printf("%s\n",usb_strerror());
  }

  /*
  if(usb_set_altinterface(h_dev,0x0)< 0){ // 0x0 comes from udesc_dump
    printf("%s\n",usb_strerror());
    }
  */
 

  return (h_dev);
}

void
ufelica_close(struct usb_dev_handle * h_dev){

  if(usb_release_interface(h_dev,0x0) < 0){ // what is 0x0 ?
    printf("%s\n",usb_strerror());
  }

  /* usb_reset unimplemented on BSD
    if(usb_reset(h_dev) < 0){
    printf("%s\n",usb_strerror());
  }
  */

  if(usb_close(h_dev) < 0){
    printf("%s\n",usb_strerror());
  }
}

struct usb_device *
ufelica_scan(struct usb_bus *busses, struct usb_device *dev){

  struct usb_bus *bus;

  for (bus = busses; bus != NULL; bus = bus->next){
    for (dev = bus->devices; dev != NULL; dev = dev->next){
      if((dev->descriptor.idVendor == USB_VENDOR_SONY) &&
         (dev->descriptor.idProduct == USB_PRODUCT_SONY_PASORI) ){

	if (option_verbose > 1) { 
	  printf("class=0x%02X, vendor=0x%04X, product=0x%04X\n",
		 dev->descriptor.bDeviceClass,
		 dev->descriptor.idVendor,
		 dev->descriptor.idProduct);
	}
        return dev;
      }
    }
  }

  printf("Pasori can not be found.\n");
  exit(0);
}

int
ufelica_send_ctl(struct usb_dev_handle * u_handle, int ctl_type, int size){
  int request_type,ret;
  unsigned char ctl[30];
  char msg[128];

  request_type = PASORI_REQUEST_TYPE;

  switch (ctl_type){
  case GET_ID_SUICA:
    strcpy(msg,"-- send get_id_suica: 00 00 ff 05 fb 50 00 03 00 00 ad 00");
    memcpy(ctl,"\x00\x00\xff\x05\xfb\x50\x00\x03\x00\x00\xad\x00",size);
    break;
  case GET_ID_EDY:
    strcpy(msg,"-- send get_id_edy  : 00 00 ff 05 fb 50 fe 00 00 00 b2 00");
    memcpy(ctl,"\x00\x00\xff\x05\xfb\x50\xfe\x00\x00\x00\xb2\x00",size);
    break;
  case INIT:
    strcpy(msg,"-- send init  : 00 00 ff 14 ec 62 29 89 11 25 f0 a0 02 a1 00 a2 89 a3 10 a4 03 a5 89 a6 10 1a 00");
    memcpy(ctl,"\x00\x00\xff\x14\xec\x62\x29\x89\x11\x25\xf0\xa0\x02\xa1\x00\xa2\x89\xa3\x10\xa4\x03\xa5\x89\xa6\x10\x1a\x00",size);
    break;
  case CTL_1:
    strcpy(msg,"-- send ctl_1: 00 00 ff 01 ff 58 a8 00");
    memcpy(ctl,"\x00\x00\xff\x01\xff\x58\xa8\x00",size);
    break;
  case CTL_2:
    strcpy(msg,"-- send ctl_2: 00 00 ff 01 ff 54 ac 00");
    memcpy(ctl,"\x00\x00\xff\x01\xff\x54\xac\x00",size);
    break;
  case CTL_3:
    strcpy(msg,"-- send ctl_3: 00 00 ff 02 fe 5a 01 a5 00");
    memcpy(ctl,"\x00\x00\xff\x02\xfe\x5a\x01\xa5\x00",size);
    break;
  case CTL_4:
    strcpy(msg,"-- send ctl_4: 00 00 ff 02 fe 5a 04 a2 00");
    memcpy(ctl,"\x00\x00\xff\x02\xfe\x5a\x04\xa2\x00",size);
    break;
  case CTL_5:
    strcpy(msg,"-- send ctl_5: 00 00 ff 05 fb 50 00 03 00 00 ad 00 00");
    memcpy(ctl,"\x00\x00\xff\x05\xfb\x50\x00\x03\x00\x00\xad\x00",size);
    break;
  case CTL_6: 
    strcpy(msg,"-- send ctl_6: ");
    memcpy(ctl,"\x00\x00\xff",size);
    break;
  case CTL_7: 
    strcpy(msg,"-- send ctl_7: 00 00 ff 83 7d 52 00 80\n");
    memcpy(ctl,"\x00\x00\xff\x83\x7d\x52\x00\x80",size);
    break;
  case CTL_8: 
    strcpy(msg,"-- send 8: 00 00 ff 03 fd 62 01 82 1b 00");
    memcpy(ctl,"\x00\x00\xff\x03\xfd\x62\x01\x82\x1b\x00",size);
    break;
  case BREAK: // this data may be break signal.
    strcpy(msg,"-- send break: 00 00 ff 00 ff 00\n");
    memcpy(ctl,"\x00\x00\xff\x00\xff\x00",size);
    break;

  default:
    printf("-- Undefined control packet\n");
    return -1;
    break;
  }
  
  if (option_verbose > 1){printf ("%s/%d\n",msg,size);}

  ret = usb_control_msg(u_handle,
                        request_type, USB_REQ_GET_STATUS,
                        0x00,0x00,
			//                        ctl,sizeof(ctl),
                        ctl,size,
                        TIMEOUT);
  if(ret < 0){ printf("%s\n",usb_strerror());}

  return ret;
}


int 
ufelica_pasori_initalize(struct usb_dev_handle * u_handle){
  int ret;
  unsigned char result[24];

  ret = ufelica_send_ctl(u_handle, INIT, INIT_SIZE);
  ufelica_read(u_handle,result,sizeof(result)); 

  return ret;
}

int
ufelica_read(struct usb_dev_handle * u_handle, char *buf, int size){
  int ret;
  
  ret = usb_bulk_read(u_handle,USB_PASORI_ENDPOINT,
                    buf,size,TIMEOUT);

  if (option_verbose > 1) { // error does'nt print in default to script use.
    if( ret < 0) {printf("%s\n",usb_strerror());}
  }

  return ret;
}

int
ufelica_get_id(struct usb_dev_handle * u_handle){
  int ret,i,type,size;
  unsigned char result[24];

  bzero(result,sizeof(result));

  if (option_verbose > 1) { printf("start getting ID.\n"); }

  switch(query_type){
  case 0:
    type = GET_ID_EDY; size = GET_ID_EDY_SIZE;
    break;
  case 1:
    type = GET_ID_SUICA; size = GET_ID_SUICA_SIZE;
    break;
  case 2:
    type = CTL_1; size = CTL_1_SIZE;
    break;
  case 3:
    type = CTL_2; size = CTL_2_SIZE;
    break;
  case 4:
    type = CTL_3; size = CTL_3_SIZE;
    break;
  case 5:
    type = CTL_4; size = CTL_4_SIZE;
    break;
  case 6:
    type = CTL_5; size = CTL_5_SIZE;
    break;
  case 7:
    type = BREAK;
    size = BREAK_SIZE;
    break;
  case 8:
    type = CTL_6; size = CTL_6_SIZE;
    break;
  case 9:
    type = CTL_7; size = CTL_7_SIZE;
    break;
  case 10:
    type = CTL_8; size = CTL_8_SIZE;
    break;
    
  default:
    type = GET_ID_EDY;
    size = GET_ID_EDY_SIZE;
    break;
  }

//  ret = ufelica_send_ctl(u_handle, GET_ID_SUICA, GET_ID_SUICA_SIZE);
//  ret = ufelica_send_ctl(u_handle, GET_ID_EDY, GET_ID_EDY_SIZE);
//  ret = ufelica_send_ctl(u_handle, CTL_1, CTL_1_SIZE);
  ret = ufelica_send_ctl(u_handle, type, size);

  if (option_verbose > 1) { printf("control msg sent.\n"); }

  ufelica_read(u_handle,result,sizeof(result));
  ufelica_read(u_handle,result,sizeof(result)); // Need twice reading

  if (option_verbose > 1) { // print all getting ID
    for (i=0; i< 23; i++){ // XXX Too Bad Hard Coding...
      printf("%02x ",result[i]);
    }
  } else { // print only different part of all bits. This is default mode.
    for (i=5; i< 15 ; i++){ // print 10 bytes
      if (option_loop == 1){
	printf("%02x",result[i]); // if loop mode, no white space.
      }else{
	printf("%02x ",result[i]);
      }
    }
  }
  printf("\n");
  fflush(stdout);
  
  return ret;
}
