mkdir -p assets
cd assets

rm data
ln -s ../../data data

cd ..
cd app/jni

rm src
ln -s ../../../src src
ln -s ../../../../../../SDL SDL

cd ../..
