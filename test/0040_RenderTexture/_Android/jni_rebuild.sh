cd assets
rm data
ln -s ../../data data

cd ..
cd jni
rm src
ln -s ../../src src
rm _common
ln -s ../../../_common _common

ndk-build clean
ndk-build

cd ..



