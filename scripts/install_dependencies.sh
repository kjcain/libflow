#!/bin/bash

sudo apt install -y libgtest-dev cmake

cd /usr/src/gtest
sudo cmake .
sudo make
sudo cp lib/libgtest*.a /usr/lib
