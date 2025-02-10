#! /usr/bin/bash

clear
echo Making things ready for you...
echo

cd depend
sudo apt install libglfw3-dev -y
sudo apt install libassimp-dev -y
sudo apt install libopenal-dev -y
cd ..
cp ./scripts/CMakeLists.txt.linux ./CMakeLists.txt

clear
echo "Done :)"