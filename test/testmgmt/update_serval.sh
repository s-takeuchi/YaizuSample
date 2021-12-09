#!/usr/bin/env bash

if [ ${EUID:-${UID}} != 0 ]; then
    echo "This script must be run as root"
    exit 1
fi

systemctl stop serval

cp /etc/serval.conf /etc/serval.conf.bak
cp /etc/serval.dat /etc/serval.dat.bak
cp /etc/nginx/conf.d/serval_nginx.conf /etc/nginx/conf.d/serval_nginx.conf.bak

yum -y remove serval
yum -y install serval-1.0-0.el8.x86_64.rpm
systemctl stop serval

rm /etc/serval.conf
rm /etc/serval.dat
rm /etc/nginx/conf.d/serval_nginx.conf

mv /etc/serval.conf.bak /etc/serval.conf
mv /etc/serval.dat.bak /etc/serval.dat
mv /etc/nginx/conf.d/serval_nginx.conf.bak /etc/nginx/conf.d/serval_nginx.conf

systemctl start serval
systemctl reload nginx
