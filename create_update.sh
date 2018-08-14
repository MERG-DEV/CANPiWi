#!/bin/bash

if [[ -d "updates" ]]; then
   echo "Update exists"
else
   mkdir "updates"
fi

filename="canpiwi-upgrade-$( date +%Y%m%d ).zip"
echo "Upgrade file name ${filename}"

echo "Files included"
cat list
echo "Creating upgrade file"
zip updates/${filename} -@ < list
