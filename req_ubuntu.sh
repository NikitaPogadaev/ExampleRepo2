#!/bin/bash

TARGET="$1"
START_FOLDER=$(pwd)

apt-get update
apt-get install -y sudo

sudo apt-get install -y git
sudo apt-get install -y clang
sudo DEBIAN_FRONTEND=noninteractive apt-get install -y cmake
sudo apt-get install -y libboost-all-dev
sudo apt-get install -y libgtest-dev
