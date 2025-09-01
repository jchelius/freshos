export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

mkdir -p ~/src
tar -xf ~/Downloads/gcc-15.2.0.tar.xz -C ~/src/
tar -xf ~/Downloads/binutils-2.45.tar.xz.sig -C ~/src/

cd $HOME/src

echo Building binutils...

mkdir -p build-binutils
cd build-binutils
../binutils-2.45/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j $(nproc)
make install

cd $HOME/src

echo Building gcc...

# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH

mkdir -p build-gcc
cd build-gcc
../gcc-15.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx
make all-gcc -j $(nproc)
make all-target-libgcc -j $(nproc)
make all-target-libstdc++-v3 -j $(nproc)
make install-gcc
make install-target-libgcc
make install-target-libstdc++-v3
