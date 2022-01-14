//
// Created by skyitachi on 2020/2/25.
//

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <cstdlib>

int main() {
  libusb_context* usb_context;
  int ret = libusb_init(&usb_context);
  if (ret != 0) {
    printf("init error");
    exit(1);
  }
  libusb_device **device_list;
  int cnt = libusb_get_device_list(usb_context, &device_list);
//  for (int i = 0; i < cnt; i++) {
//    libusb_device** device = device_list[cnt];
//  }
  printf("devices count: %d\n", cnt);
  libusb_free_device_list(device_list, true);
}



