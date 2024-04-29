IMPORTANT
=========
Development of this program has ended.
Please migrate to the program below instead.

- https://github.com/rindow/rindow-clblast-ffi

We stopped using PHP extensions because it was too difficult to prepare binary files for each PHP version and Linux version.


Rindow CLBlast PHP extension
============================
CLBlast binding for PHP.

CLBlast is BLAS library on OpenCL. [Click here for details](https://github.com/CNugteren/CLBlast).

Rindow-CLBlast allows you to harness the power of your GPU with Rindow-Neural-Network-Library. Take full advantage of your machine and feel free to use machine learning on your cheap laptop.

Requirements
============

- PHP7.2 or PHP7.3 or PHP7.4 or PHP8.0 or PHP8.1 or PHP8.2 or PHP8.3
- interop-phpobjects/polite-math 1.0.4 or later
- LinearBuffer implements for interop-phpobjects (rindow_openblas etc.)
- rindow_opencl PHP extension 0.1.4
- OpenCL 1.1/1.2 drivers/libraries.
- Windows 10 or Linux (Ubuntu 20.04, Debian 12) or later

AMD GPU/APU drivers for windows are including OpenCL drivers.
Intel Integrated GPU for windows are including OpenCL drivers.


Recommend environment
=====================

- PHP8.1 or PHP8.2 or PHP8.3
- OpenBLAS [sources](https://github.com/OpenMathLib/OpenBLAS), [binaries](https://github.com/OpenMathLib/OpenBLAS/releases)
- LinearBuffer implements - rindow-openblas 0.4.0 or later. [sources](https://github.com/rindow/rindow-openblas), [binaries](https://github.com/rindow_openblas-binaries)
- OpenCL binding for PHP - rindow-opencl 0.2.0. [sources](https://github.com/rindow/rindow-opencl), [binaries](https://github.com/rindow-opencl/releases)
- BLAS libray for OpenCL implements - rindow-clblast 0.2.0. [sources](https://github.com/rindow/rindow-clblast), [binaries](https://github.com/rindow-clblast/releases)
- Matrix PHP library - rindow-math-matrix 2.0.0 or later. [sources](https://github.com/rindow/rindow-math-matrix)
- Driver Pack - rindow-math-matrix-matlibext 1.0.0 or later. [sources](https://github.com/rindow/rindow-math-matrix-matlibext)
- Machine learning library on PHP - rindow-neuralnetworks [sources](https://github.com/rindow/rindow-neuralnetworks)

How to build from source code on Linux
========================================
You can also build and use from source code.

### Install OpenCL ICD and Tool
```shell
$ sudo apt install clinfo
```

### Install Hardware-dependent OpenCL library.
For example, in the case of Ubuntu standard AMD driver, install as follows

```shell
$ sudo apt install mesa-opencl-icd
$ sudo mkdir -p /usr/local/usr/lib
$ sudo ln -s /usr/lib/clc /usr/local/usr/lib/clc
```

In addition, there are the following drivers.

- mesa-opencl-icd
- beignet-opencl-icd
- intel-opencl-icd
- nvidia-opencl-icd-xxx
- pocl-opencl-icd

### Check OpenCL status
How to check the installation status

```shell
$ clinfo
Number of platforms                               1
  Platform Name                                   Clover
  Platform Vendor                                 Mesa
  Platform Version                                OpenCL 1.1 Mesa 21.2.6
  Platform Profile                                FULL_PROFILE
  Platform Extensions                             cl_khr_icd
  Platform Extensions function suffix             MESA
....
...
..
.
```

### Install build tools and libray
Install gcc development environment and opencl library. Then install the php development environment according to the target php version.

```shell
$ sudo apt install build-essential autoconf automake libtool bison re2c
$ sudo apt install pkg-config
$ sudo apt install phpX.X-dev (ex. php8.1-dev)
$ sudo apt install ocl-icd-opencl-dev
$ sudo apt install ./rindow-openblas-phpX.X_X.X.X_amd64.deb
$ sudo apt install ./rindow-opencl-phpX.X_X.X.X_amd64.deb
```

### Build
Run the target php version of phpize and build.

```shell
$ git clone https://github.com/rindow/rindow-opencl
$ git clone https://github.com/rindow/rindow-clblast
$ cd rindow-clblast
```

If you use Ubuntu 22.04 or Debian 12, You can install libclblast from distribution package.
```shell
$ sudo apt install libclblast-dev
```
If you use Ubuntu 20.04 or Debian 11, You have to download github.com and make a deb file, and then install.
```shell
$ sh ./clblast-packdeb.sh
$ sudo apt install ./clblast_X.X.X_amd64.deb
```

Next, Setup the environment and build.
```shell
$ composer update
$ phpizeX.X (ex. phpize8.1)
$ mv build/Makefile.global build/Makefile.global.orig
$ sed -f Makefile.global.patch < build/Makefile.global.orig > build/Makefile.global
$ ./configure --enable-rindow_clblast --with-rindow_opencl=../rindow-opencl --with-php-config=php-configX.X (ex. php-config8.1)
$ make clean
$ make
$ make test
```

### Install from built directory

```shell
$ sudo make install
or
$ sh ./packaging.sh X.X (ex. sh ./packaging.sh 8.1)
$ suto apt install ./rindow-clblast-phpX.X_X.X.X_amd64.deb (ex. ./rindow-clblast-php8.1_...)
```
Add the "extension=rindow_clblast" entry to php.ini


How to build from source code on Windows
========================================

### Install VC15 or VS16
Developing PHP extensions from php7.2 to php7.4 requires VC15 instead of the latest VC.

- Install Microsoft Visual Studio 2019 or later installer
- Run Installer with vs2017 build tools option.

Developing PHP extensions from php8.0/8.1/8.2 requires VS16. You can use Visual Studio 2019.

### php sdk and devel-pack binaries for windows

- You must know that PHP 7.2,7.3 and 7.4 needs environment for the MSVC version vc15 (that means Visual Studio 2017). php-sdk releases 2.1.9 supports vc15.
- Download https://github.com/microsoft/php-sdk-binary-tools/releases/tag/php-sdk-2.1.9
- If you want to build for PHP 8.0, you needs environment for the MSVC version vs16 (that means Visual Studio 2019). php-sdk releases 2.2.0 supports vs16.
- Download https://github.com/microsoft/php-sdk-binary-tools/releases/tag/php-sdk-2.2.0
- Extract to c:\php-sdk
- Download target OpenCL headers from https://github.com/KhronosGroup/OpenCL-Headers
- Extract to /path/to/OpenCL/include
- Download target CLBlast sdk for Windows-x64 binary from https://github.com/CNugteren/CLBlast/releases
- Extract to /path/to/OpenCL/include
- Download target Rindow-OpenCL headers from https://github.com/rindow/rindow-opencl
- Extract to /path/to/Rindow-OpenCL

For execution
- Download Rindow-OpenBLAS binaries from https://github.com/rindow/rindow-openblas/releases
- Download OpenBLAS binaries from https://github.com/xianyi/OpenBLAS/releases
- Extract and install to php extension and some directory

### Install and setup rindow_opencl and CLBlast

In order to execute rindow_clblast, you need CLBlast and the rindow_opencl.

Please install rindow_opencl and CLBlast to execute.
See https://github.com/rindow/rindow-opencl/ for more information.
And then download binaries of CLBlast for windows from https://github.com/CNugteren/CLBlast/releases and extract and set path to DLLs.

```shell
C:\tmp>copy rindow_opencl.dll /path/to/php-installation-path/ext
C:\tmp>echo extension=rindow_opencl.dll >> /path/to/php-installation-path/php.ini
C:\tmp>PATH %PATH%;/path/to/OpenBLAS-directory/bin;/path/to/CLBlast-1.x.x-Windows-x64/bin
```

### Download OpenCL-SDK

- Download OpenCL SDK form https://github.com/KhronosGroup/OpenCL-SDK/releases
- extract and copy to opencl development directory


### Install and setup rindow_openblas for test

In order to execute rindow_opencl, you need a buffer object that implements the LinearBuffer interface. Please install rindow_openblas to run the tests.
See https://github.com/rindow/rindow-openblas/ for more information.

```shell
C:\tmp>copy rindow_openblas.dll /path/to/php-installation-path/ext
C:\tmp>echo extension=rindow_openblas >> /path/to/php-installation-path/php.ini
C:\tmp>PATH %PATH%;/path/to/OpenBLAS/bin
```

### start php-sdk for target PHP version

Open Visual Studio Command Prompt for VS for the target PHP version(see stepbystepbuild.)
Note that you must explicitly specify the version of vc15 for which php.exe was built.
The -vcvars_ver=14.16 means vc15.

```shell
C:\visual\studio\path>vcvars64 -vcvars_ver=14.16
or
C:\visual\studio\path>vcvars64

C:\tmp>cd c:\php-sdk
C:\php-sdk>phpsdk-vxxx-x64.bat
```

### Build

```shell
$ PATH %PATH%;/path/to/OpenBLAS/bin;/path/to/CLBlast-devel-directory/lib
$ cd /path/to/here
$ composer update
$ /path/to/php-devel-pack-x.x.x-Win32-Vxxx-x64/phpize.bat
$ configure --enable-rindow_clblast --with-prefix=/path/to/php-installation-path --with-opencl=/path/to/OpenCL-SDK-directory --with-clblast=/path/to/CLBlast-devel-directory --with-rindow_opencl=/path/to/Rindow-OpenCL-sources-directory
$ nmake clean
$ nmake
$ nmake test
```

### Install from built directory

- Copy the php extension binary(.dll) to the php/ext directory from here/arch/Releases_TS/php_rindow_clblast.dll
- Add the "extension=php_rindow_clblast" entry to php.ini
