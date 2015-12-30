cd assets
rm data
ln -s ../../data data

cd ..
cd jni
rm src
ln -s ../../src src

rm SDL
ln -s ../../../../../SDL SDL

rm SDL2_image
ln -s ../../../../../SDL_image SDL_image

ndk-build clean
ndk-build

cd ..



