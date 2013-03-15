#!/bin/bash

n=$(basename $PWD)
echo $n

cd src

headerFiles=$(find . -name "*.h")

for h in $headerFiles
do
 b=$(basename $h .h)
 if grep -q Q_OBJECT $h
 then
  "/Applications/bbndk/host_10_0_10_536/darwin/x86/usr/bin/moc" $h -o moc_${b}.cpp
 fi

 if [ -f $b ]
 then
  cp $b $h ../public/$n
 fi
done
