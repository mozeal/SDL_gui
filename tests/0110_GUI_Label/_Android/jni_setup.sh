mkdir -p app/src/main/assets
rm app/src/main/assets/data
ln -s ../../../../../data app/src/main/assets/data

rm app/jni/SDL2
ln -s ../../../../../SDL2 app/jni/

rm app/jni/SDL2_image
ln -s ../../../../../SDL2_image app/jni/

rm app/jni/SDL2_ttf_HarfBuzz
ln -s ../../../../../SDL2_ttf_HarfBuzz app/jni/

rm app/jni/src
ln -s ../../../src app/jni/
