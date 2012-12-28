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


#ifndef TE923CON_H_
#define TE923CON_H_


#define VERSION         "0.5"
#define TRUE            1
#define FALSE           0


typedef struct {
  // this are the values from station
  unsigned int timestamp;                 // timestamp for this dataset
  float t[6];                             // temperature of the sensors in °C
  signed short h[6];                      // humidity of the sensors in % rel
  float uv;                               // UV index
  float press;                            // air pressure in mBar
  unsigned char forecast;                 // weather forecast from station
  //                                      0 => heavy snow
  //                                      1 => snow
  //                                      2 => heavy rain
  //                                      3 => rain
  //                                      4 => cloudy
  //                                      5 => some clouds
  //                                      6 => sunny
  unsigned char storm;                    // storm warning if value = 1 else 0
  float wChill;                           // windchill in °C
  float wGust;                            // wind gusts in m/s
  float wSpeed;                           // wind speed in m/s
  unsigned char wDir;                     // wind direction in x*22.5°; 0 => north
  unsigned int RainCount;                 // Raincounter of station as number up to 65535 * 0.7 mm/m²
  // status of sensors, names are _<sensor>; if status is <> 0, the value should be 0
  // 0  => value of sensor should be ok
  // -1 => data is invalid
  // -2 => sensor is out of range
  // -3 => missing link
  // -4 => any other error
  signed char _t[6], _h[6];
  signed char _press, _uv, _wDir, _wSpeed, _wGust, _wChill, _RainCount, _storm, _forecast;
  unsigned int __src;                     // source adress of dataset (needed for dump)
  } Te923DataSet_t;



typedef struct {
  unsigned char SysVer;                   // programversion system controller
  unsigned char BarVer;                   // programversion barometer
  unsigned char UvVer;                    // programversion UV sensor and ext. channels
  unsigned char RccVer;                   // programversion remote controlled clock and sunset 
  unsigned char WindVer;                  // programversion wind- rain sensor
  _Bool battery1 :    1;                  // battery in sensor 1
  _Bool battery2 :    1;                  // battery in sensor 2
  _Bool battery3 :    1;                  // battery in sensor 3
  _Bool battery4 :    1;                  // battery in sensor 4
  _Bool battery5 :    1;                  // battery in sensor 5
  _Bool batteryUV :   1;                  // battery in UV sensor
  _Bool batteryWind : 1;                  // battery in wind sensor
  _Bool batteryRain : 1;                  // battery in rain sensor
} Te923DevSet_t;

void usage();
void printData( Te923DataSet_t *data, char *iText );


#endif /* TE923CON_H_ */
