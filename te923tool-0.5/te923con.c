/***************************************************************************
 *   Copyright (C) 2010 by Sebastian John                                  *
 *   te923@fukz.org                                                        *
 *   for more informations visit http://te923.fukz.org                     *
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

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "te923con.h"
#include "te923usb.h"
#include "te923com.h"


unsigned short debug = FALSE;
enum output { rtdata, memdump, status };


void usage() {
  printf( "te923con, connector for TE923 weatherstations v%s - (c) by Sebastian John 2010 (te923@fukz.org)\n", VERSION );
  printf( "This program is free software, distributed under the terms of the GNU General Public Licens.\n\n");
  printf( "usage: te923con [options]\n" );
  printf( "  -D - Debug output (please send this output for bugfixing)\n" );
  printf( "  -d - dump all values from internal memory\n" );
  printf( "  -s - get status from weather station\n" );
  printf( "  -i - set the output for invalid values (unreachable sensors), default it is i\n" );
  printf ("  -h - print this help informations\n" );
  printf( "  -v - print version number of this program\n" );
  printf( "\n" );
  printf( "Output format is colon separatet values. Invalid values or values from not present sensors\n" );
  printf( "are hidden by default. You can set output for invalid with -i option. Output format:\n\n" );
  printf( "T0:H0:T1:H1:T2:H2:T3:H3:T4:H4:T5:H5:PRESS:UV:FC:STORM:WD:WS:WG:WC:RC\n\n" );
  printf( "  T0    - temperature from internal sensor in °C\n" );
  printf( "  H0    - humidity from internal sensor in %% rel\n" );
  printf( "  T1..5 - temperature from external sensor 1..4 in °C\n" );
  printf( "  H1..5 - humidity from external sensor 1...4 in %% rel\n" );
  printf( "  PRESS - air pressure in mBar\n" );
  printf( "  UV    - UV index from UV sensor\n" );
  printf( "  FC    - station forecast, see below for more details\n" );
  printf( "  STORM - stormwarning; 0 - no warning, 1 - fix your dog \n" );
  printf( "  WD    - wind direction in n x 22.5°; 0 -> north\n" );
  printf( "  WS    - wind speed in m/s\n" );
  printf( "  WG    - wind gust speed in m/s\n" );
  printf( "  WC    - windchill temperature in °C\n" );
  printf( "  RC    - rain counter (maybe since station starts measurement) as value\n" );
  printf( "\n" );
  printf(  "    weather forecast means (as precisely as possible)\n" );
  printf(  "      0 - heavy snow\n" );
  printf(  "      1 - little snow\n" );
  printf(  "      2 - heavy rain\n" );
  printf(  "      3 - little rain\n" );
  printf(  "      4 - cloudy\n" );
  printf(  "      5 - some clouds\n" );
  printf(  "      6 - sunny\n" );
  printf( "\n" );
  printf( "Status output means:\n\n" );
  printf( "SYSSW:BARSW:EXTSW:RCCSW:WINSW:BATR:BATU:BATW:BAT5:BAT4:BAT5:BAT2:BAT1\n\n" );
  printf( "  SYSSW  - software version of system controller\n" );
  printf( "  BARSW  - software version of barometer\n" );
  printf( "  EXTSW  - software version of UV and channel controller\n" );
  printf( "  RCCSW  - software version of rain controller\n" );
  printf( "  WINSW  - software version of wind controller\n" );
  printf( "  BATR   - battery of rain sensor (1-good (not present), 0-low)\n" );
  printf( "  BATU   - battery of UV sensor (1-good (not present), 0-low)\n" );
  printf( "  BATW   - battery of wind sensor (1-good (not present), 0-low)\n" );
  printf( "  BAT5   - battery of sensor 5 (1-good (not present), 0-low)\n" );
  printf( "  BAT4   - battery of sensor 4 (1-good (not present), 0-low)\n" );
  printf( "  BAT3   - battery of sensor 3 (1-good (not present), 0-low)\n" );
  printf( "  BAT2   - battery of sensor 2 (1-good (not present), 0-low)\n" );
  printf( "  BAT1   - battery of sensor 1 (1-good (not present), 0-low)\n" );
  printf("\n");
  printf( "For updates, bugfixes and support visit http://te923.fukz.org.\n" );
}



void printData( Te923DataSet_t *data, char *iText ) {
  int i;
  printf( "%d:"  , data->timestamp );
  for ( i = 0; i <= 5; i++ ) {

    if ( data->_t[i] == 0 )  
      printf( "%0.2f:", data->t[i] );
    else
      printf( "%s:", iText );
    
    if ( data->_h[i] == 0 )  
      printf( "%d:", data->h[i] );
    else
      printf( "%s:", iText );
  }
  
  if ( data->_press == 0 ) 
    printf( "%0.1f:", data->press );
  else
    printf( "%s:", iText );

  if ( data->_uv == 0 ) 
    printf( "%0.1f:", data->uv );
  else
    printf( "%s:", iText );

  if ( data->_forecast == 0 ) 
    printf( "%d:", data->forecast );
  else
    printf( "%s:", iText );

  if ( data->_storm == 0 ) 
    printf( "%d:", data->storm );
  else 
    printf( "%s:", iText );

  if ( data->_wDir == 0 ) 
    printf( "%d:", data->wDir );
  else 
    printf( "%s:", iText );

  if ( data->_wSpeed == 0 ) 
    printf( "%0.1f:", data->wSpeed );
  else 
    printf( "%s:", iText );

  if ( data->_wGust == 0 ) 
    printf( "%0.1f:", data->wGust );
  else 
    printf( "%s:", iText );

  if ( data->_wChill == 0 ) 
    printf( "%0.1f:", data->wChill );
  else 
    printf( "%s:", iText );

  if ( data->_RainCount == 0 ) 
    printf( "%d", data->RainCount );
  else 
    printf( "%s:", iText );

  printf( "\n" );
}




int main( int argc, char **argv ) {
  
  char *iText = NULL;
  signed int opt;
  enum output output = rtdata;
  
  while (( opt = getopt( argc, argv, "Ddhi:sv" ) ) != -1 ) {
    switch ( opt ) {
    case 'h':
      usage();
      return 0;
    case 'v':
      printf( "te923con version %s\n" , VERSION );
      return 0;
    case 'D':
      debug = TRUE;
      break;
    case 's':
      output = status;
      break;
    case 'd':
      output = memdump;
      break;
    case 'i':
      iText = optarg;
      break;
    case '?':
      if ( optopt == 'i' )
	fprintf( stderr, "Option -%c requires an argument.\n", optopt );
      else if ( isprint( optopt ) )
      	fprintf( stderr, "Unknown option `-%c'.\n", optopt ); 
      else   
      	fprintf( stderr, "Unknown option character `\\x%x'.\n", optopt );
      return 1;
      break;
    }
  }

  if ( iText == NULL )
    iText = "i";
  
  struct usb_dev_handle *devh;
  devh = te923_handle();
  if ( devh == NULL )
    return 1;      
  
  if (debug > 0)
    printf("[DEBUG] VER %s\n",VERSION);
  
  if ( ( output == rtdata ) || ( output == memdump ) ) {
    Te923DataSet_t *data;
    data = ( Te923DataSet_t* )malloc( sizeof( Te923DataSet_t ) );
    memset ( data, 0, sizeof ( data ) );
    if ( output == rtdata ) {
      get_te923_lifedata( devh, data );
      printData( data, iText);
    }
    if ( output == memdump ) {
      int i;
      for ( i = 0; i < 208; i++ ) {
	get_te923_memdata( devh, data );
	printData( data, iText);
      }
    }
  }
  
  if ( output == status ) {
    Te923DevSet_t *status;
    status = ( Te923DevSet_t* )malloc( sizeof( Te923DevSet_t ) ); 
    get_te923_devstate( devh, status );
    printf( "0x%x:0x%x:0x%x:0x%x:0x%x:%d:%d:%d:%d:%d:%d:%d:%d\n",
	    status->SysVer, status->BarVer, status->UvVer, status->RccVer, status->WindVer,
	    status->batteryRain, status->batteryUV, status->batteryWind, status->battery5,
	    status->battery4, status->battery3, status->battery2, status->battery1
	    );
  }
  
  te923_close( devh );
  return 0;
}

  


