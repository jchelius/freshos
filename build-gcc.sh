export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

which -- $TARGET-as || echo $TARGET-as is not in the PATH

cd $HOME/src

echo Building gcc...

mkdir -p build-gcc
cd build-gcc
../gcc-15.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx
make all-gcc -j $(nproc)
make all-target-libgcc -j $(nproc)
make all-target-libstdc++-v3 -j $(nproc)
make install-gcc
make install-target-libgcc
make install-target-libstdc++-v3
