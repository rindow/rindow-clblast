Rindow CLBlast PHP extension
============================
CLBlast binding for PHP.

CLBlast is BLAS library on OpenCL. [Click here for details](https://github.com/CNugteren/CLBlast).

Rindow-CLBlast allows you to harness the power of your GPU with Rindow-Neural-Network-Library. Take full advantage of your machine and feel free to use machine learning on your cheap laptop.

Requirements
============

- PHP7.2 or PHP7.3 or PHP7.4
- interop-phpobjects/polite-math 1.0.3 or later
- LinearBuffer implements for interop-phpobjects (rindow_openblas etc.)
- rindow_opencl PHP extension 0.1.0
- OpenCL 1.2 drivers/libraries.
- Windows 10

AMD GPU/APU drivers for windows are including OpenCL drivers.
Probably you can use Intel OpenCL dirivers.
Intel OpenCL drivers can be downloaded from site https://software.intel.com/content/www/us/en/develop/articles/opencl-drivers.html

Recommend environment
=====================

- LinearBuffer implements - rindow-openblas 0.1.6 or later. [sources](https://github.com/rindow/rindow-openblas), [binaries](https://github.com/rindow_openblas-binaries)
- Matrix PHP library - rindow-math-matrix 1.0.8 or later. [sources](https://github.com/rindow/rindow-math-matrix)
- Machine learning library on PHP - rindow-neuralnetworks [sources](https://github.com/rindow/rindow-neuralnetworks)

How to setup pre-build binaries
===============================
You can download and use pre-built Windows binaries.
Download the binary for your version of PHP.

- https://github.com/rindow/rindow-openblas/releases
- https://github.com/xianyi/OpenBLAS/releases
- https://github.com/rindow/rindow-opencl/releases
- https://github.com/rindow/rindow-clblast/releases
- https://github.com/CNugteren/CLBlast/releases

Please download the following two binaries and extract.

- The PHP extension of rindow-opencl that matches the php version.
- The PHP extension of rindow-openblas that matches the php version.
- The PHP extension of rindow-clblast that matches the php version.
- DLL of OpenBLAS library.
- DLL of CLBlast library.

Copy the shared library to the PHP extension directory and set it in php.ini.
And OpenBLAS DLL's path to Windows PATH environment variable.

```shell
C:\tmp>copy rindow_openblas.dll /path/to/php-installation-path/ext
C:\tmp>copy rindow_opencl.dll /path/to/php-installation-path/ext
C:\tmp>copy rindow_clblast.dll /path/to/php-installation-path/ext
C:\tmp>echo extension=rindow_openblas.dll >> /path/to/php-installation-path/php.ini
C:\tmp>echo extension=rindow_opencl.dll >> /path/to/php-installation-path/php.ini
C:\tmp>echo extension=rindow_clblast.dll >> /path/to/php-installation-path/php.ini
C:\tmp>PATH %PATH%;/path/to/OpenBLAS/bin;/path/to/CLBlast-Library/lib
C:\tmp>cd /same/app/directory
C:\app\dir>composer require rindow/rindow-math-matrix
```

How to build from source code on Windows
========================================

### Install VC15
Developing PHP extensions from php7.2 to php7.4 requires VC15 instead of the latest VC.

- Install Microsoft Visual Studio 2019 or later installer
- Run Installer with vs2017 build tools option.

### php sdk and devel-pack binaries for windows

- You must know that PHP 7.2,7.3 and 7.4 needs environment for the MSVC version vc15 (that means Visual Studio 2017). php-sdk releases 2.1.9 supports vc15.
- Download https://github.com/microsoft/php-sdk-binary-tools/releases/tag/php-sdk-2.1.9
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
C:\tmp>PATH %PATH%;/path/to/OpenBLAS-directory/bin;/path/to/CLBlast-1.x.x-Windows-x64/lib
```

### Exports OpenCL binding library
C:\visual\studio\path>vcvars64 -vcvars_ver=14.16
C:\visual\studio\path>cd /path/to/OpenCL
C:\some-path\OpenCL>mkdir lib
C:\some-path\OpenCL>cd lib
C:\some-path\OpenCL\lib>dumpbin /exports C:\Windows\System32\OpenCL.dll > OpenCL.def
C:\some-path\OpenCL\lib>notepad OpenCL.def                     #### edit def file
EXPORTS

clBuildProgram
.....

C:\path\OpenCL\lib>lib /def:OpenCL.def /machine:x64

The OpenCL.lib file is created.

### Install and setup rindow_openblas for test

In order to execute rindow_opencl, you need a buffer object that implements the LinearBuffer interface. Please install rindow_openblas to run the tests.
See https://github.com/rindow/rindow-openblas/ for more information.

```shell
C:\tmp>copy rindow_openblas.dll /path/to/php-installation-path/ext
C:\tmp>echo extension=rindow_openblas.dll >> /path/to/php-installation-path/php.ini
C:\tmp>PATH %PATH%;/path/to/OpenBLAS/bin
```

### start php-sdk for target PHP version

Open Visual Studio Command Prompt for VS for the target PHP version(see stepbystepbuild.)
Note that you must explicitly specify the version of vc15 for which php.exe was built.
The -vcvars_ver=14.16 means vc15.

```shell
C:\visual\studio\path>vcvars64 -vcvars_ver=14.16

C:\tmp>cd c:\php-sdk
C:\php-sdk>phpsdk-vc15-x64.bat
```

### Build

```shell
$ PATH %PATH%;/path/to/OpenBLAS/bin;/path/to/CLBlast-devel-directory/lib
$ cd /path/to/here
$ composer update
$ /path/to/php-devel-pack-7.x.x-Win32-VC15-x64/phpize.bat
$ configure --enable-rindow_clblast --with-prefix=/path/to/php-installation-path --with-opencl=/path/to/OpenCL-devel-directory --with-clblast=/path/to/CLBlast-devel-directory --with-rindow_opencl=/path/to/Rindow-OpenCL-sources-directory
$ nmake clean
$ nmake
$ nmake test
```
