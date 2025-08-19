# DumpSource2

A C++ application that offline dumps Source 2 schema bindings. Heavily based on [ValveResourceFormat/DumpSource2](https://github.com/ValveResourceFormat/DumpSource2). 

## Usage

Run DumpSource2 from the rootbin folder of a Source 2 game:

```
cd "C:\Program Files (x86)\Steam\steamapps\common\Counter-Strike Global Offensive\game\bin\win64"
. "C:\Code\DumpSource2\build\Debug\DumpSource2-CS2.exe" "C:\Code\demofile-net\src\DemoFile.Game.Cs\Schema"
```

## Compilation

### Windows

```sh
mkdir build
cd build
cmake ..
msbuild ALL_BUILD.vcxproj
```

### Linux
```sh
mkdir build
cd build
cmake ..
make
```
