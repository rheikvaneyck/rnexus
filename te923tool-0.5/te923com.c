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

#include "te923com.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

extern unsigned short debug;


void printBuffer( unsigned char *buf, int size ) {
  int i;
  printf( "[DEBUG] BUF |" );
  for ( i = 0; i < size; i++ )
    printf( "%02d|", i );
  printf( "\n[DEBUG] BUF |" );
  for ( i = 0; i < size; i++ )
    printf( "%02x|", buf[i] );
  printf( "\n" );
}



int bcd2int( char bcd ) {
  return (( int )(( bcd & 0xF0 ) >> 4 ) * 10 + ( int )( bcd & 0x0F ) );
}



int read_from_te923( usb_dev_handle *devh, int adr, unsigned char rbuf[BUFLEN] ) {
  int timeout = 50;
  int i, ret, bytes;
  int count = 0;
  unsigned char crc;
  char buf[] = {0x05, 0x0AF, 0x00, 0x00, 0x00, 0x00, 0xAF, 0xFE};
  buf[4] = adr / 0x10000;
  buf[3] = ( adr - ( buf[4] * 0x10000 ) ) / 0x100;
  buf[2] = adr - ( buf[4] * 0x10000 ) - ( buf[3] * 0x100 );
  buf[5] = ( buf[1] ^ buf[2] ^ buf[3] ^ buf[4] );
  ret = usb_control_msg( devh, 0x21, 0x09, 0x0200, 0x0000, buf, 0x08, timeout );
  if ( ret < 0 )
    return -1;
  sleep( 0.3 );
  while ( usb_interrupt_read( devh, 0x01, buf, 0x8, timeout ) > 0 ) {
    sleep( 0.15 );
    bytes = ( int )buf[0];
    if ( debug > 0 ) {
      printf( "[DEBUG] RAW |" );
      int x;
      for ( x = 0; x <= 7; x++ )
	printf( "%02x|", buf[x] );
      printf( "\n" );
    }
    if (( count + bytes ) < BUFLEN )
      memcpy( rbuf + count, buf + 1, bytes );
    count += bytes;
  }
  crc = 0x00;
  for ( i = 0; i <= 32; i++ )
    crc = crc ^ rbuf[i];
  if ( crc != rbuf[33] )
    return -2;
  if ( rbuf[0] != 0x5a )
    return -3;
  return count;
}



int decode_te923_data( unsigned char buf[], Te923DataSet_t *data ) {
  //decode temperature and humidity from all sensors
  int i;
  for ( i = 0; i <= 5; i++ ) {
    int offset = i * 3;
    data->_t[i] = 0;
    if ( debug > 0 )
      printf( "[DEBUG] TMP %d BUF[%02d]=%02x BUF[%02d]=%02x BUF[%02d]=%02x\n",i, 0+offset, buf[0+offset], 1 + offset, buf[1+offset],  2+offset, buf[2+offset] );
    if ( bcd2int( buf[0+offset] & 0x0F ) > 9 ) {
      if ( debug > 0 )
	printf( "[DEBUG] TMP buffer 0 & 0x0F > 9\n" );
      if ((( buf[0+offset] & 0x0F ) == 0x0C ) || (( buf[0+offset] & 0x0F ) == 0x0B ) ) {
	if ( debug > 0 )
	  printf( "[DEBUG] TMP buffer 0 & 0x0F = 0x0C or 0x0B\n" );
	data->_t[i] = -2;
      } else {
	data->_t[i] = -1;
	if ( debug > 0 )
	  printf( "[DEBUG] TMP other error in buffer 0\n" );
      }
    }
    if ((( buf[1+offset] & 0x40 ) != 0x40 ) && i > 0 ) {
      if ( debug > 0 )
	printf( "[DEBUG] TMP buffer 1 bit 6 set\n" );
      data->_t[i] = -2;
    }
    if ( data->_t[i] == 0 ) {
      data->t[i] = ( bcd2int( buf[0+offset] ) / 10.0 ) + ( bcd2int( buf[1+offset] & 0x0F ) * 10.0 );
      if ( debug > 0 )
	printf( "[DEBUG] TMP %d before is %0.2f\n", i, data->t[i] );
      if (( buf[1+offset] & 0x20 ) == 0x20 )
	data->t[i] += 0.05;
      if (( buf[1+offset] & 0x80 ) != 0x80 )
	data->t[i] *= -1;
      if ( debug > 0 )
	printf( "[DEBUG] TMP %d after is %0.2f\n", i, data->t[i] );
    } else
      data->t[i] = 0;
    
    if ( data->_t[i] <= -2 ) {
      data->_h[i] = -2;
      data->h[i] = 0;
    } else if ( bcd2int( buf[2+offset] & 0x0F ) > 9 ) {
      data->_h[i] = -3;
      data->h[i] = 0;
    } else {
      data->h[i] = bcd2int( buf[2+offset] );
      data->_h[i] = 0;
    }
  }
  
  //decode value from UV sensor
  if ( debug > 0 )
    printf( "[DEBUG] UVX BUF[18]=%02x BUF[19]=%02x\n", buf[18], buf[19] );
  if (( buf[18] == 0xAA ) && ( buf[19] == 0x0A ) ) {
    data->_uv = -3;
    data->uv = 0;
  } else if (( bcd2int( buf[18] ) > 99 ) || ( bcd2int( buf[19] ) > 99 ) ) {
    data->_uv = -1;
    data->uv = 0;
  }
  
  else {
    data->uv = bcd2int( buf[18] & 0x0F ) / 10.0 + bcd2int( buf[18] & 0xF0 ) + bcd2int( buf[19] & 0x0F ) * 10.0;
    data->_uv = 0;
  }
  
  //decode pressure
  if ( debug > 0 )
    printf( "[DEBUG] PRS BUF[20]=%02x BUF[21]=%02x\n", buf[20], buf[21] );
  if (( buf[21] & 0xF0 ) == 0xF0 ) {
    data->press = 0;
    data->_press = -1;
  } else {
    data->press = ( int )( buf[21] * 0x100 + buf[20] ) * 0.0625;
    data->_press = 0;
  }
  
  //decode weather status and storm warning
  if ( debug > 0 )
    printf( "[DEBUG] STT BUF[22]=%02x\n", buf[22] );
  if (( buf[22] & 0x0F ) == 0x0F ) {
    data->_storm = -1;
    data->_forecast = -1;
    data->storm = 0;
    data->forecast = 0;
  } else {
    data->_storm = 0;
    data->_forecast = 0;
    if (( buf[22] & 0x08 ) == 0x08 )
      data->storm = 1;
    else
      data->storm = 0;
    
    data->forecast = ( int )( buf[22] & 0x07 );
  }
  
  //decode windchill
  if ( debug > 0 )
    printf( "[DEBUG] WCL BUF[23]=%02x BUF[24]=%02x\n", buf[23], buf[24] );
  if (( bcd2int( buf[23] & 0xF0 ) > 90 ) || ( bcd2int( buf[23] & 0x0F ) > 9 ) ) {
    if (( buf[23] == 0xAA ) && ( buf[24] == 0x8A ) )
      data->_wChill = -1;
    else if (( buf[23] == 0xBB ) && ( buf[24] == 0x8B ) )
      data->_wChill = -2;
    else if (( buf[23] == 0xEE ) && ( buf[24] == 0x8E ) )
      data->_wChill = -3;
    else
      data->_wChill = -4;
  } else
    data->_wChill = 0;
  if ((( buf[24] & 0x40 ) != 0x40 ) )
    data->_wChill = -2;
  if ( data->_wChill == 0 ) {
    data->wChill = ( bcd2int( buf[23] ) / 10.0 ) + ( bcd2int( buf[24] & 0x0F ) * 10.0 );
    if (( buf[24] & 0x20 ) == 0x20 )
      data->wChill += 0.05;
    if (( buf[24] & 0x80 ) != 0x80 )
      data->wChill *= -1;
    data->_wChill = 0;
  } else
    data->wChill = 0;
  
  //decode windgust
  if ( debug > 0 )
    printf( "[DEBUG] WGS BUF[25]=%02x BUF[26]=%02x\n", buf[25], buf[26] );
  if (( bcd2int( buf[25] & 0xF0 ) > 90 ) || ( bcd2int( buf[25] & 0x0F ) > 9 ) ) {
    data->_wGust = -1;
    if (( buf[25] == 0xBB ) && ( buf[26] == 0x8B ) )
      data->_wGust = -2;
    else if (( buf[25] == 0xEE ) && ( buf[26] == 0x8E ) )
      data->_wGust = -3;
    else
      data->_wGust = -4;
  } else
    data->_wGust = 0;
  
  if ( data->_wGust == 0 ) {
    
    int offset = 0;
    if (( buf[26] & 0x10 ) == 0x10 )
      offset = 100;
    data->wGust = (( bcd2int( buf[25] ) / 10.0 ) + ( bcd2int( buf[26] & 0x0F ) * 10.0 ) + offset ) / 2.23694;
  } else
    data->wGust = 0;
  
  //decode windspeed
  if ( debug > 0 )
    printf( "[DEBUG] WSP BUF[27]=%02x BUF[28]=%02x\n", buf[27], buf[28] );
  if (( bcd2int( buf[27] & 0xF0 ) > 90 ) || ( bcd2int( buf[27] & 0x0F ) > 9 ) ) {
    data->_wSpeed = -1;
    if (( buf[27] == 0xBB ) && ( buf[28] == 0x8B ) )
      data->_wSpeed = -2;
    else if (( buf[27] == 0xEE ) && ( buf[28] == 0x8E ) )
      data->_wSpeed = -3;
    else
      data->_wSpeed = -4;
  } else
    data->_wSpeed = 0;
  
  if ( data->_wSpeed == 0 ) {
    
    int offset = 0;
    if (( buf[28] & 0x10 ) == 0x10 )
      offset = 100;
    data->wSpeed = (( bcd2int( buf[27] ) / 10.0 ) + ( bcd2int( buf[28] & 0x0F ) * 10.0 ) + offset ) / 2.23694;
  } else
    data->wSpeed = 0;
  
  //decode wind direction
  if ( debug > 0 )
    printf( "[DEBUG] WDR BUF[29]=%02x\n", buf[29] & 0x0F );
  if (( data->_wGust <= -3 ) || ( data->_wSpeed <= -3 ) ) {
    data->_wDir = -3;
    data->wDir = 0;
  } else {
    data->wDir = ( int )buf[29] & 0x0F;
    data->_wDir = 0;
  }
  
  //decode rain counter
  //don't know to find out it sensor link missing, but is no problem, because the counter is inside
  //the sation, not in the sensor.
  if ( debug > 0 )
    printf( "[DEBUG] RNC BUF[29]=%02x BUF[30]=%02x BUF[31]=%02x\n", buf[29] & 0xF0, buf[30], buf[31] );  
  data->_RainCount = 0;
  data->RainCount = ( int )( buf[31] * 0x100 + buf[30] );
  return 0;
}



int get_te923_memdata( usb_dev_handle *devh , Te923DataSet_t *data ) {
  int adr, ret;
  unsigned char buf[BUFLEN];
  unsigned char databuf[BUFLEN];
  if ( data->__src == 0 ) {
    //first read, get oldest data
    int last_adr = 0x0000FB;
    do
      ret = read_from_te923( devh, last_adr, buf );
    while ( ret <= 0 );
    adr = (((( int )buf[3] ) * 0x100 + ( int )buf[5] ) * 0x26 ) + 0x101;
  } else
    adr = data->__src;
  
  time_t tm = time( NULL );
  struct tm *timeinfo = localtime( &tm );
  int sysyear = timeinfo->tm_year;
  int sysmon = timeinfo->tm_mon;
  do
    ret = read_from_te923( devh, adr, buf );
  while ( ret <= 0 );
  int day = bcd2int( buf[2] );
  int mon = ( int )( buf[1] & 0x0F );
  int year = sysyear;
  if ( mon > sysmon + 1 )
    year--;
  int hour = bcd2int( buf[3] );
  int minute = bcd2int( buf[4] );
  
  struct tm newtime;
  newtime.tm_year = year;
  newtime.tm_mon  = mon - 1;
  newtime.tm_mday = day;
  newtime.tm_hour = hour;
  newtime.tm_min  = minute;
  newtime.tm_sec  = 0;
  newtime.tm_isdst = -1;
  
  data->timestamp = mktime( &newtime );
  memcpy( databuf, buf + 5, 11 );
  adr += 0x10;
  do
    ret = read_from_te923( devh, adr, buf );
  while ( ret <= 0 );
  
  memcpy( databuf + 11, buf + 1, 21 );
  decode_te923_data( databuf, data );
  adr += 0x16;
  if ( adr > 0x001FBB )
    adr = 0x000101;
  data->__src = adr;
  
  return 0;
}



int get_te923_lifedata( usb_dev_handle *devh , Te923DataSet_t *data ) {
  int ret;
  int adr = 0x020001;
  unsigned char buf[BUFLEN];
  do
    {
      ret = read_from_te923( devh, adr, buf );
    }
  while ( ret <= 0 );
  if ( buf[0] != 0x5A )
    return -1;
  memmove( buf, buf + 1, BUFLEN - 1 );
  data->timestamp = ( int )time( NULL );
  if ( debug > 0 )
    printBuffer(buf, sizeof(buf) );
  decode_te923_data( buf, data );
  return 0;
}




int get_te923_devstate( usb_dev_handle *devh, Te923DevSet_t *dev ) {
  int ret;
  int adr = 0x000098;
  unsigned char buf[BUFLEN];
  do
    ret = read_from_te923( devh, adr, buf );
  while ( ret <= 0 );
  if ( buf[0] != 0x5A )
    return -1;
  dev->SysVer = buf[5];
  dev->BarVer = buf[1];
  dev->UvVer = buf[2];
  dev->RccVer = buf[3];
  dev->WindVer = buf[4];
  adr = 0x00004C;
  do
    ret = read_from_te923( devh, adr, buf );
  while ( ret <= 0 );
  if ( buf[0] != 0x5A )
    return -1;
  if (( buf[1] & 0x80 ) == 0x80 )
    dev->batteryRain = TRUE;
  else
    dev->batteryRain = FALSE;
  
  if (( buf[1] & 0x40 ) == 0x40 )
    dev->batteryWind = TRUE;
  else
    dev->batteryWind = FALSE;
  
  if (( buf[1] & 0x20 ) == 0x20 )
    dev->batteryUV = TRUE;
  else
    dev->batteryUV = FALSE;
  
  if (( buf[1] & 0x10 ) == 0x10 )
    dev->battery5 = TRUE;
  else
    dev->battery5 = FALSE;
  
  if (( buf[1] & 0x08 ) == 0x08 )
    dev->battery4 = TRUE;
  else
    dev->battery4 = FALSE;
  
  if (( buf[1] & 0x04 ) == 0x04 )
    dev->battery3 = TRUE;
  else
    dev->battery3 = FALSE;
  
  if (( buf[1] & 0x02 ) == 0x02 )
    dev->battery2 = TRUE;
  else
    dev->battery2 = FALSE;
  
  if (( buf[1] & 0x01 ) == 0x01 )
    dev->battery1 = TRUE;
  else
    dev->battery1 = FALSE;
  return 0;
}

