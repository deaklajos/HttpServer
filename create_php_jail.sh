#!/bin/bash
# TODO return error if error

J=.

mkdir -p $J/bin

# TODO php path could be obtained from "php -i"
cp -v /usr/bin/php $J/bin

# TODO This regex may fail in the future or on other systems
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
