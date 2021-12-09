#!/usr/bin/env bash

if [ ${EUID:-${UID}} != 0 ]; then
    echo "This script must be run as root"
    exit 1
fi

sudo kill -KILL `ps -fe|grep "servalagt serval"|awk '{print $2}'`
