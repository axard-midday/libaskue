#!/bin/sh

if [ -d "/usr/local/include/libaskue" ]
then
    rm -Rf "/usr/local/include/libaskue"
fi

cp -R ../bin/libaskue /usr/local/include/libaskue

if [ -f "/usr/local/include/libaskue.h" ]
then
    rm -f "/usr/local/include/libaskue.h"
fi

cp ../bin/libaskue.h /usr/local/include/libaskue.h

if [ -f "/usr/local/lib/libaskue.so.2.0.0" ]
then
    rm -f "/usr/local/lib/libaskue.so.2.0.0"
fi

cp ../bin/libaskue.so.2.0.0 /usr/local/lib/libaskue.so.2.0.0

if [ -f "/usr/local/lib/libaskue.so.2.0" ]
then
    rm -f "/usr/local/lib/libaskue.so.2.0"
fi

ln -s /usr/local/lib/libaskue.so.2.0.0 /usr/local/lib/libaskue.so.2.0 

if [ -f "/usr/local/lib/libaskue.so.2" ]
then
    rm -f "/usr/local/lib/libaskue.so.2"
fi

ln -s /usr/local/lib/libaskue.so.2.0.0 /usr/local/lib/libaskue.so.2

if [ -f "/usr/local/lib/libaskue.so" ]
then
    rm -f "/usr/local/lib/libaskue.so"
fi

ln -s /usr/local/lib/libaskue.so.2.0.0 /usr/local/lib/libaskue.so 
