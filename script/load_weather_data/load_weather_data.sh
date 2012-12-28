#!/bin/sh
XDIR=`dirname "${0}"`
APP="${XDIR}/te923con"
LOG="${XDIR}/../../data/weather.data
USB=/dev/usb/hiddev0

echo -n "Check if $APP exists and is executable"
if [ -x $APP ];
then
	echo "..yes"
else
	echo "..no"
	exit 1
fi

touch $LOG
echo -n "Check if $LOG exists and is writable"
if [ -w $LOG ];
then
	echo "...yes"
else
	echo "..no"
	exit 1
fi

echo -n "Check if $USB exists and is readable"
if [ -r $USB ];
then
	echo "...yes"
else
	echo "...no"
	exit 1
fi

$APP >> $LOG
