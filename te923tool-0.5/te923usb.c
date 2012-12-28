/***************************************************************************
 *   Copyright (C) 2010 by Sebastian John                                  *
 *   te923@fukz.org                                                        *
 *   for more informations visit http://te923.fukz.org                     *
 *                                                                         *
 *   This file is part of the communication suite for te923 weather-       *
 *   stations. This file handels the USB connection and device handler.    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <usb.h>
#include <stdio.h>
#include "te923usb.h"

//extern unsigned short debug;

struct usb_device *find_te923() {
  usb_find_busses();
  usb_find_devices();
  struct usb_bus *bus;
  struct usb_device *dev;
  for ( bus = usb_get_busses(); bus; bus = bus->next ) {
    for ( dev = bus->devices; dev; dev = dev->next ) {
      if ( dev->descriptor.idVendor == TE923_VENDOR && dev->descriptor.idProduct == TE923_PRODUCT ) {
	return dev;
      }
    }
  }
  return NULL;
}


struct usb_dev_handle *te923_handle()  {
  int ret;
  int driver_name_len = 30;
  char driver_name[driver_name_len];
  struct usb_device *dev;
  struct usb_dev_handle *devh;

  usb_init();
  dev = find_te923();
  if ( dev == NULL ) {
    fprintf( stderr, "TE923 weather station cannot be found, sorry.\n");
    return NULL;
  }
  
  devh = usb_open( dev );
  if ( devh == NULL ) {
    fprintf( stderr, "Error while opening USB port and get device handler.\n"  );
    return NULL;
  }
  
  ret = usb_get_driver_np(devh, 0, driver_name, driver_name_len);
  if ( ret == 0 ) {
    usb_detach_kernel_driver_np( devh, 0 );
  }

  ret = usb_set_configuration( devh, 1 );
  if ( ret != 0 ) {
    fprintf( stderr, "Error while setting configuration (%d).\n" , ret );
    return NULL;
  }
  
  ret = usb_claim_interface( devh, 0 );
  if ( ret != 0 ) {
    fprintf( stderr, "Error while claiming interface (%d).\n" , ret );
    return NULL;
  }
  
  ret = usb_set_altinterface( devh, 0 );
  if ( ret != 0 ) {
    fprintf( stderr, "Error while setting alternative interface (%d).\n" , ret );
    return NULL;
  }
  
  sleep( 0.5 );
  return devh;
}


void te923_close( usb_dev_handle *devh ) {
  usb_release_interface( devh, 0 );
  usb_close( devh );
}



