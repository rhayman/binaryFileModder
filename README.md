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

Now run cmake and then make:

```
cmake ..
make
```

### Windows installation

Create a build directory:

```
mkdir build
cd build
```

CMake command is a bit different compared to linux and will change depending on what version of Visual Studio you have (go to the "Help" menu and then "About" to find out which one).

Do one of the following appropriately:

```
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake -G "Visual Studio 16 2019" -A x64 ..
cmake -G "Visual Studio 15 2017 Win64" ..
cmake -G "Visual Studio 14 2015 Win64" ..
cmake -G "Visual Studio 12 2013 Win64" ..
```

In Visual Studio go to "File" -> "Open Project" (or similar) and navigate to the binaryFileModder.sln file in the build folder just created and open that.

Change the build type from "Debug" to "Release"

From within Visual Studio do ctrl-B or go to the "Build" menu and select "Build" (or similar, might be "BUILD ALL")

Hopefully you should have a binaryFileModder.exe file somewhere (it should be in a "bin" folder in the folder containing the repository).

### Usage

To see basic usage options invoke with the --help option and no other arguments

```
./binaryFileModder --help
```

#### Example
To add a single row at the end off each column with a fill value of 8:

```
./binaryFileModder -i "/path/to/some/data.dat" -aC 1 -f 8
```

### Help contents

```
Usage: binaryFileModder [--help] [--version] --input VAR [--output VAR] [--n_channels VAR] [--fill_value VAR] [--add_rows VAR] [--remove_rows VAR] [--add_columns VAR] [--remove_columns VAR]

Optional arguments:
  -h, --help             shows help message and exits 
  -v, --version          prints version information and exits 
  -i, --input            The input file name [required]
  -o, --output           The output file name (default=output.dat) 
  -n, --n_channels       The number of channels in the input file (default=64) 
  -f, --fill_value       The fill value for adding data file name (default=0) 
  -aR, --add_rows        The number of rows to add 
  -rR, --remove_rows     The number of rows to remove 
  -aC, --add_columns     The number of columns to add 
  -rC, --remove_columns  The number of columns to remove 
```
