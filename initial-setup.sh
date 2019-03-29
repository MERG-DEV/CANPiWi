#!/bin/bash

PATH=/sbin:/bin:/usr/sbin:/usr/bin
. /lib/lsb/init-functions

dir="/home/pi"
canpidir="/home/pi/canpi"

append_to_file(){
   val=$1
   file=$2
   grep -q "$val" "$file" || echo "$val" >> $file
}

config_boot_config(){
   bootconf="/boot/config.txt"
   cp $bootconf "/boot/config.txt.$RANDOM"
   sed -i "s/dtparam=spi=off/dtparam=spi=on/" $bootconf
   #in case the entry is not there
   append_to_file "dtparam=spi=on" $bootconf
   ls /boot/overlays/mcp2515-can0-overlay*
   if [ $? == 0 ];then
       append_to_file "dtoverlay=mcp2515-can0-overlay,oscillator=16000000,interrupt=25" $bootconf
   else
       append_to_file "dtoverlay=mcp2515-can0,oscillator=16000000,interrupt=25" $bootconf
   fi
}

add_can_interface(){
  f="/etc/network/interfaces"
  append_to_file "auto can0" $f
  append_to_file "iface can0 can static" $f
  append_to_file "bitrate 125000 restart-ms 1000" $f
}

create_default_canpi_config(){
  conf="$canpidir/canpi.cfg"
  echo "#log config" > $conf
  echo "logfile=\"canpi.log\"" >> $conf
  echo "#INFO,WARN,DEBUG" >> $conf
  echo "loglevel=\"INFO\"" >> $conf
  echo "logappend=\"false\"" >> $conf
  echo "log_console=\"false\"" >> $conf
  echo "#ed service config" >> $conf
  echo "tcpport=5555" >> $conf
  echo "candevice=\"can0\"" >> $conf
  echo "#cangrid config" >> $conf
  echo "can_grid=\"true\"" >> $conf
  echo "cangrid_port=5550" >> $conf
  echo "#Bonjour config" >> $conf
  echo "service_name=\"MYLAYOUT\"" >> $conf
  echo "#network config" >> $conf
  echo "ap_mode=\"False\"" >> $conf
  echo "ap_ssid=\"pi\"" >> $conf
  echo "ap_password=\"12345678\"" >> $conf
  echo "ap_channel=6" >> $conf
  echo "#if not running in ap mode use this" >> $conf
  echo "router_ssid=\"SSID\"" >> $conf
  echo "router_password=\"PASSWORD\"" >> $conf
  echo "node_number=4321" >> $conf
  echo "node_mode=0" >> $conf
  echo "turnout_file=\"turnout.txt\"" >> $conf
  echo "fn_momentary=\"2\"" >> $conf
  echo "canid=100" >> $conf
  echo "button_pin=17" >> $conf
  echo "green_led_pin=24" >> $conf
  echo "yellow_led_pin=23" >> $conf
  echo "red_led_pin=22" >> $conf
  echo "ap_no_password=\"True\"" >> $conf
  echo "create_log_file=\"False\"" >> $conf
  echo "start_event_id=1" >> $conf
}

message_header(){
  echo ""
  echo "-----------------------------------------------------"
  echo "################ ${1} ################"
  echo "-----------------------------------------------------"
}

echo "Type the Wifi SSID followed by [ENTER]:"
read wssid

echo "Type the Wifi password followed by [ENTER]:"
read wpassword

#echo "#############  Stop swap services and delete swap file ###############"

#swapoff -a
#update-rc.d dphys-swapfile remove
#rm /var/swap
#apt-get -y purge dphys-swapfile
#apt-get -y autoremove

message_header "APT UPDATE"
apt-get -y update

message_header "GIT"
apt-get -y install git

message_header "HOSTAPD"
apt-get -y install hostapd
systemctl unmask hostapd.service
systemctl enable hostapd
service hostapd restart

message_header "DHCP"
apt-get -y install isc-dhcp-server

message_header "CAN UTILS"
apt-get -y install can-utils

message_header "CAN INTERFACE"
add_can_interface

message_header "GET CANPI CODE"
cd $dir
git clone https://github.com/MERG-DEV/CANPiWi.git canpi

message_header "BOOT CONFIG"
config_boot_config

message_header "COMPILE CANPI"
cd canpi
make clean
make all

message_header "CANPI CONFIG"
if [ -f "$canpidir/canpi.cfg" ];
then
    echo "Config file already exists"
else
    echo "Config file does not exist. Creating a default one"
    create_default_canpi_config
fi

#change the router ssid and password
sed -i 's/SSID/'"$wssid"'/' "$canpidir/canpi.cfg"
sed -i 's/PASSWORD/'"$wpassword"'/' "$canpidir/canpi.cfg"

message_header "WEB SERVER"
#install the webpy
tar xvf webpy.tar.gz
mv webpy-webpy-770baf8 webpy
cd webpy
python setup.py install

message_header "CHANGE DIR OWNER"
cd $dir
chown -R pi.pi canpi

message_header "MOVE CONFIG FILES"
#backup and move some basic files
FILE="/etc/hostapd/hostapd.conf"
FILEBAK="${FILE}.bak"
if [ -f $FILE ];
then
    mv $FILE $FILEBAK
fi
cp "$canpidir/hostapd.conf" /etc/hostapd/


FILE="/etc/default/hostapd"
FILEBAK="${FILE}.bak"
if [ -f $FILE ];
then
    mv $FILE $FILEBAK
fi

cp "$canpidir/hostapd" /etc/default/

FILE="/etc/dhcp/dhcpd.conf"
FILEBAK="${FILE}.bak"
if [ -f $FILE ];
then
    mv $FILE $FILEBAK
fi

cp "$canpidir/dhcpd.conf" /etc/dhcp/

mv /etc/default/isc-dhcp-server /etc/default/isc-dhcp-server.old
cp "$canpidir/isc-dhcp-server" /etc/default
systemctl enable isc-dhcp-server

message_header "CONFIGURE SCRIPT FILES"
cp "$canpidir/start_canpi.sh" /etc/init.d/
chmod +x /etc/init.d/start_canpi.sh
update-rc.d start_canpi.sh defaults

message_header "CONFIGURE THE RASPBERRY PI AND REBOOT"
/etc/init.d/start_canpi.sh configure

