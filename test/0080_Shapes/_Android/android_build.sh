adb shell am force-stop org.libsdl.app 
ant debug
ant debug install
adb shell am start -n org.libsdl.app/.SDLActivity

