#!/usr/bin/env bash

if [ ${EUID:-${UID}} != 0 ]; then
    echo "This script must be run as root"
    exit 1
fi

sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-01 serval-dev01 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-02 serval-dev02 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-03 serval-dev03 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-04 serval-dev04 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-05 serval-dev05 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-06 serval-dev06 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-07 serval-dev07 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-08 serval-dev08 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-09 serval-dev09 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-10 serval-dev10 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-11 serval-dev11 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-12 serval-dev12 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-13 serval-dev13 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-14 serval-dev14 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-15 serval-dev15 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-16 serval-dev16 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-17 serval-dev17 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-18 serval-dev18 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-19 serval-dev19 /home/ec2-user/serval.cer &
sudo nohup /usr/bin/servalagt serval.red-squirrels.net 8080 /home/ec2-user/agentest/serval-dev-20 serval-dev20 /home/ec2-user/serval.cer &
