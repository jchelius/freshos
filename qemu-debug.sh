#!/bin/sh
set -e
. ./iso.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) -S -s -kernel isodir/boot/myos.kernel &

