#!/bin/sh

cd build/Release
rm -rf *.dSYM
cp -rf "../../Read Me First.rtfd" ./
cd ../../

IMAGENAME="Structure23"

rm $IMAGENAME.dmg

hdiutil create -srcdir build/Release -volname "Structure" $IMAGENAME.dmg