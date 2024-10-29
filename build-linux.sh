#!/bin/bash

export MAKEOBJDIRPREFIX=/home/strajabot/Data/code/freebsd-obj

./tools/build/make.py --debug --bootstrap-toolchain -d e -j 5 buildworld KERNEL_CONF=QEMU TARGET_ARCH=riscv64 TARGET=riscv TRY_GCC_BROKEN=yes
