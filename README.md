# SDL_gui
GUI library for SDL2

## ***** This is work in progress! *****
Stay tuned.

## Features
- Base on SDL, SDL_image, SDL_ttf
- Support HarffBuzz to display international text correctly.
- Support FontAwesome Icon
- Support Unicode (UTF-8)
- Written in C++

## Multi-platform
- MacOS
- Windows
- iOS
- Android
- WebAssembly (via Emscripten)
- Raspberry Pi
- Linux (Build with CMake)

## How to compile
- MacOSX - use Xcode open the *.xcodeproj then compile and run
- iOS - use Xcode open the *.xcodeproj then compile and run
- Android - use android studio open the _Android directory then compile and run it. You may need to config the local.properties to specific sdk and ndk directories.
- Windows - use Visual Studio 2017 open the *.sln then compile and run. You need to use vspkg to install HarfBuzz first.
- Emscripten - run make (you need to set path for emsdk tools first)
- Cmake - just run cmake then make.

![Alt text](/screenshot/screenshot_02.png?raw=true)

![Alt text](/screenshot/SDL_gui.png?raw=true)
