# Python wrapper for Emergent Cameras being used at NYU CUSP


## Install instructions:
Linux (Ubuntu):
- Install the Emergent SDK -- only works on Ubuntu 12.04 w/ kernel 3.11.0-24-generic
- Install BOOST python lib:
```
wget boost_1_63_0.tar.bz2 https://downloads.sourceforge.net/project/boost/boost/1.63.0/boost_1_63_0.tar.bz2?r=http%3A%2F%2Fwww.boost.org%2Fusers%2Fhistory%2Fversion_1_63_0.html&ts=1488517764&use_mirror=svwh

tar --bzip2 -xf /path/to/boost_1_63_0.tar.bz2

mkdir usr/include/boost

./bootstrap --prefix=/usr/include/boost --with-python=python

./b2 install -d0

echo "export LD_LIBRARY_PATH=/usr/include/boost/lib" >> ~/.bashrc

source ~/.bashrc
```
- Install CMAKE:
`sudo apt-get install cmake`

MAC:
- Install BOOST python lib:
`brew install boost`

WINDOWS:
- *Refer `www.boost.org/doc/libs/1_58_0/more/getting_started/windows.html`*

## Implemented Functions:
Refer [help_file.md](https://github.com/Mohitsharma44/CEmergent/blob/master/help_file.md)

## Developer Notes:
*Instructions for compiling and creating shared object*

### Development Version:
- **sh run.sh**
- **Open ipython and import the module**

### Final Version
- **Create cmake (`CMakeLists.txt`) file in the same directory as the cxx code**
- **create a directory `build` and enter it**
- **run `cmake ..`**
- **run `make`**
- **Open ipython and import the module**

*Copyright 2017 Mohit Sharma*
