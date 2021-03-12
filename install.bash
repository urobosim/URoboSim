#!/bin/bash

BASE_DIR=`pwd`

echo $BASE_DIR
cd ..

if [ ! -d URoboVision/.git ]
then
    git clone git@github.com:K4R-IAI/K4R_Development.git
fi

if [ ! -d UROSBridge/.git ]
then
    git clone git@github.com:urobosim/UROSBridge.git
fi

if [ ! -d UROSWorldControl/.git ]
then
    git clone git@github.com:urobosim/UROSWorldControl.git
fi

if [ ! -d UTFPublisher/.git ]
then
    git clone git@github.com:NeumannM89/UTFPublisher.git
fi

if [ ! -d UUtils/.git ]
then
    git clone git@github.com:urobosim/UUtils.git
fi
