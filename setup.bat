@echo off

cls
echo Making things ready for you...
echo.

cd depend
git clone https://github.com/glfw/glfw.git
git clone https://github.com/kcat/openal-soft.git
git clone https://github.com/assimp/assimp.git
cd ..
copy scripts\\CMakeLists.txt.win CMakeLists.txt

cls
echo Done :)