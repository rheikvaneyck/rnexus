/***************************************************************************
 *   Copyright (C) 2010 by Sebastian John                                  *
 *   te923@fukz.org                                                        *
 *   for more informations visit http://te923.fukz.org                     *
 *                                                                         *
 *   This file is part of the communication suite for te923 weather-       *
 *   stations. This file get the data from the device and decoding it.     *
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


#ifndef TE923COM_H_
#define TE923COM_H_

#include "te923con.h"
#include "te923usb.h"

#define TRUE 1
#define FALSE 0

void printBuffer( unsigned char *buf, int size );
int bcd2int( char bcd );
int read_from_te923( usb_dev_handle *devh, int adr, unsigned char rbuf[BUFLEN] );
int decode_te923_data( unsigned char buf[], Te923DataSet_t *data );
int get_te923_memdata( usb_dev_handle *devh , Te923DataSet_t *data );
int get_te923_lifedata ( usb_dev_handle *devh , Te923DataSet_t *data );
int get_te923_devstate( usb_dev_handle *devh, Te923DevSet_t *dev );

#endif /* TE923COM_H_ */
