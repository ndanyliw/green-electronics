#!/bin/bash

# Ned Danyliw - September 2015. Adapted from littlebox env script by
# Nic McDonald.

# Nic McDonald - January 2015
# This script setups up the programming environment
#  for the Stanford Littlebox control system.

set -e # fail on any error

# this function runs a command using sudo before running it prints the command
#  and pauses so that the user can see what is about to run before accepting.
function ask_sudo() {
    echo ""
    echo "############### SUDO START #########################"
    while true; do
    read -p "run 'sudo $1'? (yes/no): " yn
    case $yn in
            [Yy]* ) sudo $1; break;;
            [Nn]* ) exit;;
            * ) echo "Please answer yes or no.";;
    esac
    done
    echo "############### SUDO DONE ##########################"
}

# check for missing dependencies
pkgs=()
deps=( \
    autoconf \
    pkg-config \
    libusb-1.0-0 \
    libusb-1.0-0-dev \
    git \
    libc6:i386 \
    libncurses5:i386 \
    libtool \
    python3)
for pkg in ${deps[*]}; do
    if ! dpkg -s ${pkg} 2>&1 | grep "Status: install ok installed" > /dev/null; then
    pkgs+=("${pkg}")
    echo "missing ${pkg}"
    fi
done
if [ "${#pkgs[@]}" -ne 0 ]; then
    ask_sudo "apt-get -y install ${pkgs[*]}"
fi

# create a "ee155_tools" directory
if [ -d ~/ee155_tools ]; then
    echo "~/ee155_tools already exists!"
    while true; do
    read -p "delete ~/ee155_tools? (yes/no): " yn
    case $yn in
            [Yy]* ) rm -rf ~/ee155_tools; break;;
            [Nn]* ) exit;;
            * ) echo "Please answer yes or no.";;
    esac
    done
fi
mkdir ~/ee155_tools

# arm-none-eabi
cd ~/ee155_tools
if [ ! -f /tmp/gcc-arm-none-eabi-4_9-2015q2-20150609-linux.tar.bz2 ]; then
    echo "gcc-arm-none-eabi tarball doesn't exist. fetching it now..."
    wget https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q2-update/+download/gcc-arm-none-eabi-4_9-2015q2-20150609-linux.tar.bz2 -P /tmp
fi
tar jxvf /tmp/gcc-arm-none-eabi-4_9-2015q2-20150609-linux.tar.bz2

# stlink
if true; then
    cd ~/ee155_tools
    git clone https://github.com/texane/stlink.git stlink_src
    cd stlink_src
    #git checkout f66332de8ec35aef  # some bug after this. check again later
    ./autogen.sh
    mkdir build
    cd build
    ../configure --prefix="${HOME}/ee155_tools/stlink"
    make
    make install
    cd ../
    ask_sudo "install -m 644 49-stlinkv2.rules /etc/udev/rules.d/"
fi

# openocd
if true; then
    cd ~/ee155_tools
    git clone git://git.code.sf.net/p/openocd/code openocd_src
    cd openocd_src
    ./bootstrap
    mkdir build
    cd build
    ../configure --prefix="${HOME}/ee155_tools/openocd" --enable-maintainer-mode --enable-stlink
    make
    make install
    cd ../contrib
    ask_sudo "install -m 644 99-openocd.rules /etc/udev/rules.d/"
fi

# pyserial
if true; then
    cd ~/ee155_tools
    wget https://pypi.python.org/packages/source/p/pyserial/pyserial-2.7.tar.gz
    tar xvf pyserial-2.7.tar.gz
    cd pyserial-2.7
    python3 setup.py install --user
fi

# create a setup script to be run each time the user wants to develop in this environment
cd ~/ee155_tools
cat > env.sh <<EOF
export PATH="\${HOME}/ee155_tools/gcc-arm-none-eabi-4_9-2015q2/bin:\${HOME}/ee155_tools/openocd/bin:\${HOME}/ee155_tools/stlink/bin:\${PATH}"
EOF

# ensure user is in the dialout group
if ! groups | grep dialout; then
    ask_sudo "usermod -a -G dialout ${USER}"
    echo ""
    echo "WARNING: You need to log out and log back in!!!"
    echo ""
fi

echo ""
echo "Wahoo!!!"
echo "The programming environment is now set up."
echo "Use 'source ~/ee155_tools/env.sh' to put the environment in your path."
