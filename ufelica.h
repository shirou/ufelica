#ifndef _UFELICA_H_
#define _UFELICA_H_

#define TIMEOUT 1000 /* msec */

#define USB_VENDOR_SONY 0x054C
#define USB_PRODUCT_SONY_PASORI 0x006C

#define USB_PASORI_ENDPOINT 0x81  // 0x81 from EPaddress

/*******************************************************************/
/* prototype                                                       */
/*******************************************************************/

struct usb_bus *  ufelica_init();
struct usb_dev_handle * ufelica_open(struct usb_device *dev);
void ufelica_close(struct usb_dev_handle* h_dev);
struct usb_device * ufelica_scan(struct usb_bus *busses, struct usb_device *dev);

int ufelica_send_ctl(struct usb_dev_handle * u_handle, int ctl_type, int size);

int ufelica_read(struct usb_dev_handle * u_handle, char *buf, int size);

int ufelica_get_id(struct usb_dev_handle * u_handle);
int ufelica_pasori_initalize(struct usb_dev_handle * u_handle);


#define PASORI_REQUEST_TYPE 0x40

#define GET_ID_SUICA        1
#define GET_ID_SUICA_SIZE  12
#define GET_ID_EDY          2
#define GET_ID_EDY_SIZE    12

#define INIT                3
#define INIT_SIZE          27

#define CTL_1              10
#define CTL_1_SIZE          8 
#define CTL_2              11
#define CTL_2_SIZE          8 
#define CTL_3              12
#define CTL_3_SIZE          9 
#define CTL_4              13
#define CTL_4_SIZE          9 
#define CTL_5              14
#define CTL_5_SIZE         12
#define CTL_6              15
#define CTL_6_SIZE          8
#define CTL_7              16
#define CTL_7_SIZE         12
#define CTL_8              17
#define CTL_8_SIZE         10

#define BREAK          99
#define BREAK_SIZE     6

#endif // _UFELICA_H_
