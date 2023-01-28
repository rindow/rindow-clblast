#!/usr/bin/bash

CLBLASTVERSION=1.5.2

. /etc/os-release

FILENAME=CLBlast-${CLBLASTVERSION}-Linux-x64
PLATFORM=ubuntu${VERSION_ID}
TARGET=./pkgwork


wget https://github.com/CNugteren/CLBlast/releases/download/${CLBLASTVERSION}/${FILENAME}.tar.xz

xz -dc ${FILENAME}.tar.xz | tar xvf -
rm -rf ${TARGET}
mkdir ${TARGET}
mkdir ${TARGET}/DEBIAN
mkdir ${TARGET}/usr
mv ${FILENAME}/* ${TARGET}/usr

cat << EOS > ${TARGET}/DEBIAN/control
Package: clblast
Maintainer: CLBlast Developers <CNugteren@users.noreply.github.com>
Architecture: amd64
Depends: libc6 (>= 2.14), ocl-icd-libopencl1 | libopencl1, ocl-icd-libopencl1 (>= 1.0) | libopencl-1.1-1
Version: ${CLBLASTVERSION}-1+${PLATFORM}
Homepage: https://github.com/CNugteren/CLBlast/
Description: The tuned OpenCL BLAS library
 CLBlast is a modern, lightweight, performant and tunable OpenCL BLAS library
 written in C++11. It is designed to leverage the full performance potential
 of a wide variety of OpenCL devices from different vendors, including desktop
 and laptop GPUs, embedded GPUs, and other accelerators. 
EOS
mv ${TARGET}/usr/lib/pkgconfig/clblast.pc ./clblast.pc.orig
cat ./clblast.pc.orig  | sed -e s/^prefix=.*$/prefix=\\/usr/ > ${TARGET}/usr/lib/pkgconfig/clblast.pc
rm ./clblast.pc.orig
rm clblast_${CLBLASTVERSION}-1+${PLATFORM}_amd64.deb
fakeroot dpkg-deb --build pkgwork .
