#!/bin/bash

export MAKEOBJDIRPREFIX=/home/strajabot/Data/code/freebsd-obj
export DESTDIR=/home/strajabot/Data/code/freebsd-riscv

mkdir -p $DESTDIR
./tools/build/make.py --bootstrap-toolchain KERNEL_CONF=QEMU TARGET=riscv TRY_GCC_BROKEN=yes TARGET_ARCH=riscv64 -DNO_ROOT DESTDIR=$DESTDIR installworld
./tools/build/make.py --bootstrap-toolchain KERNEL_CONF=QEMU TARGET=riscv TRY_GCC_BROKEN=yes TARGET_ARCH=riscv64 -DNO_ROOT DESTDIR=$DESTDIR distribution
./tools/build/make.py --bootstrap-toolchain KERNEL_CONF=QEMU TARGET=riscv TRY_GCC_BROKEN=yes TARGET_ARCH=riscv64 -DNO_ROOT DESTDIR=$DESTDIR installkernel

