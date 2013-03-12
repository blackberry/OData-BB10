#!/bin/bash

n=$(basename $PWD)
echo $n

cd src

for h in *.h
do
 b=$(basename $h .h)
 if grep -q Q_OBJECT $h
 then
  "moc.exe" $h -o moc_${b}.cpp
 fi

 if [ -f $b ]
 then
  cp $b $h ../public/$n
 fi
done