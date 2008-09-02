#!/bin/bash


cd ../apps/examples

for example in $( ls . )
do
echo "-----------------------------------------------------------------"
echo "building " + $example
cd $example
#codeblocks --build --target="Debug" "$example.cbp"
codeblocks --build --target="Release" "$example.cbp"
cd ../
echo "-----------------------------------------------------------------"
echo ""
done
