#!/bin/sh

te923con="/opt/te923/te923con"
figlet="/usr/bin/figlet"

tmp=`$te923con|cut -d ":" -f4|sed -e "s/\./ . /"`

echo $tmp" C"|$figlet