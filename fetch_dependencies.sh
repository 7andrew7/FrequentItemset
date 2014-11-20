#!/bin/bash

if [ -d "gtest-1.7.0" ];
then
    echo "Skipping gtest installation"
else
    echo "Fetching gtest"
    wget https://googletest.googlecode.com/files/gtest-1.7.0.zip
    unzip gtest-1.7.0.zip
fi

if [ -d "data" ];
then
    echo "Skipping data installation"
else
    echo "Installing data dependencies"
    mkdir data
    wget http://fimi.ua.ac.be/data/mushroom.dat -O ./data/mushroom.dat
fi
