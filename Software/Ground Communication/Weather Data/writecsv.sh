#!/bin/bash

# Before running this file
# Download WGRIB2 from the following website
# http://www.cpc.ncep.noaa.gov/products/wesley/wgrib2/
# Move the program to /bin or somewhere else in your path
# In the terminal use the command

# ./writecsv.sh folder/filename.f 0 48

# filename will end in f000 to f048, which are appended in this function
# 0 48 corresponds to time 0 to 48

# Get forecast data from the following website
# https://nomads.ncdc.noaa.gov/data/gfsanl/
# large file sizes, but the parsed text files are smaller because not all data is relevant

#(TMP|HGT|RH|UGRD|VGRD)
base=$1
first=$2
final=$3
n=9
nn=99

nums={$first..$final..3}

for ((val=$first; val<=$final; val+=3))
do
  if (("$val" > "$nn"))
  then
    wgrib2 $base$val'.grb2' -undefine out-box -90:-80 30:40 -match "mb:" -match ":(UGRD|VGRD):" -csv $base$val'.txt'
  elif (("$val" > "$n"))
  then
    wgrib2 $base'0'$val'.grb2' -undefine out-box -90:-80 30:40 -match "mb:" -match ":(UGRD|VGRD):" -csv $base'0'$val'.txt'
  else
    wgrib2 $base'00'$val'.grb2' -undefine out-box -90:-80 30:40 -match "mb:" -match ":(UGRD|VGRD):" -csv $base'00'$val'.txt'
  fi
done

echo Finished with files $first to $final 
