cd assets
rm data
ln -s ../../data data

cd ..
cd jni
rm src
ln -s ../../src src
rm _common
ln -s ../../../_common _common

rm SDL
ln -s ../../../../SDL SDL

rm SDL2_image
ln -s ../../../../SDL2_image SDL2_image

ndk-build clean
ndk-build

cd ..



