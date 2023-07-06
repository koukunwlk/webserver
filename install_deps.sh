#!/bin/bash

if [[ -r /etc/os-release ]]; then
    source /etc/os-release
    if [[ "$ID" == "debian" || "$ID" == "ubuntu" || "$ID" == "linuxmint" ]]; then
        echo "Debian-based OS detected. Installing dependencies..."
		sudo apt-get -y install googletest
        exit 0
    fi
fi

if [[ -r /etc/os-release ]]; then
    source /etc/os-release
    if [[ "$ID" == "arch" || "$ID" == "manjaro" ]]; then
        echo "Arch-based OS detected. Installing dependencies..."
		pacman -S gtest
        exit 0
    fi
fi

echo "Unknown OS detected."
exit 1
