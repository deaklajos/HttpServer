#!/bin/bash

J=.

mkdir -p $J/bin
cp -v /usr/bin/php $J/bin

list="$(ldd /usr/bin/php | egrep -o '/(usr\/lib|lib).*\.[0-9]+')"
for i in $list
do 
    newfile="${J}${i}"
    folder=$(dirname "${newfile}")
    #echo "${newfile}"
    #echo "${folder}"
    mkdir -p "${folder}"
    cp  -v "$i" "${newfile}"
done
