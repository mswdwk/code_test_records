#!/bin/bash
#only display the last dir name
#on Ubuntu 16.04 Desktop System
sed -i 's/\\w/\\W/g' ~/.bashrc
source ~/.bashrc
