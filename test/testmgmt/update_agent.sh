#!/usr/bin/env bash

if [ ${EUID:-${UID}} != 0 ]; then
    echo "This script must be run as root"
    exit 1
fi

systemctl stop servalagt

cp /etc/servalagt.conf /etc/servalagt.conf.bak

yum -y remove servalagt
yum -y install servalagt-1.0-0.el8.x86_64.rpm
systemctl stop servalagt

rm /etc/servalagt.conf
mv /etc/servalagt.conf.bak /etc/servalagt.conf

systemctl start servalagt
