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
    wget https://archive.ics.uci.edu/ml/machine-learning-databases/mushroom/agaricus-lepiota.data -O ./data/agaricus-lepiota.data
fi
