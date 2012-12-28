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

#ifndef TE923USB_H_
#define TE923USB_H_

#include <usb.h>

#define TE923_VENDOR    0x1130
#define TE923_PRODUCT   0x6801
#define BUFLEN          35


//extern unsigned short debug;

struct usb_device *find_te923();
struct usb_dev_handle *te923_handle();
void te923_close( usb_dev_handle *devh );

#endif  /* TE923USB_H_ */
