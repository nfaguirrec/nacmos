#!/bin/bash

cd ./src/spa
qmake
make

cd ../umv
qmake
make

cd ../qge
qmake
make

cd ../dendrogram
qmake
make

cd ../
qmake
make


