#!/bin/sh

# удалить папку назначения со всем содержимым
rm -rf "../bin"

export LIBRARY_PATH=/usr/lib/i386-linux-gnu:$LIBRARY_PATH

# создать папку назначения бинарников
if [ ! -d "../bin" ] 
then
    mkdir "../bin"
fi

# собрать библиотеку
make

# создать папку назначения для исходников
if [ ! -d "../bin/libaskue" ] 
then
    mkdir "../bin/libaskue"
fi

# поиск заголовков
all=$( find ../src/ -type f -name "*.h" )

# главный заголовок
main="libaskue.h"

# копировать заголовочные файлы в папку назначения
for src in $all
do
    filename=$( basename $src )
    if [ "$filename" = "$main" ]
    then
        cp $src ../bin/$filename
    else
        cp $src ../bin/libaskue/$filename
    fi
done

