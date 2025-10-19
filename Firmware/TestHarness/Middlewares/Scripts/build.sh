#!/bin/bash

echo "--Clean-up--"
if [ -d ../build ]; then
	rm -r ../build
fi

if [ -d ./_CPack_Packages ]; then
	rm -r ./_CPack_Packages
fi


echo "--Configuration--"
cmake -B ../build -S .. -DCMAKE_BUILD_TYPE=Debug

echo "--Building--"
cmake --build ../build --config Debug 

echo "--Testing--"
#ctest --show-only
ctest -C Debug -VV --test-dir ../build

echo "--Deployement--"
#cmake --install ../build --prefix "/Users/sgabre/git/CrazyUncleLab/cmake-tuto/Install/"

echo "--Packing--"
#cpack -G ZIP -C Debug --build-config Debug --config ../build/CPackSourceConfig.cmake

echo "--Run Application--"
#../build/Tutorial
#../build/Tutorial 25.0

# Open a screen in new terminal and also log the output 
#screen /dev/tty.usbmodem103 115200 | tee -a output.log

#arm-none-eabi-gdb ../build/TestHarness.elf \
#-ex "target extended-remote localhost:61234" \
#-ex "monitor reset halt" \
#-ex "load" \
#-ex "monitor reset init" \
#-ex "monitor reset run" \
#-ex "detach" \
#-ex "quit"

echo "--Clean-up--"
if [ -d ./_CPack_Packages ]; then
	rm -r ./_CPack_Packages
fi