# Installation

## Prerequisites
- CMake
- c/c++ compiler

Clone the repo. From the command line:

```
git clone https://github.com/rhayman/binaryFileModder
cd binaryFileModder
```

### Linux Installation
Create a build directory from which you'll run the build:

```
mkdir build
cd build
```

Now run cmake:

```
cmake ..
make
```

### Windows installation
Create a build directory

```
mkdir build
cd build
```

CMake command is a bit different and will change depending on what version of Visual Studio you have:

```
cmake -G "Visual Studio 17 2022" -A x64 ..
```

Check the version of Visual Studio in its Help menu and change the above numbers appropraitely

From Visual Studio go to "File" and "Open Project" (or similar) and navigate to the binaryFileModder.sln file and open that.

From within Visual Studio do ctrl-B or go to the "Build" menu and select "Build" (or similar)

You should have a binaryFileModder.exe file somewhere (I hope)

If this doesn't work might need to use WSL (Windows Subsystem for Linux) although I've never tried that...
