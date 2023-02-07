source ./environment.sh

echo installing compiling gcc
export GCC_VERSION=12.2.0
cd $TMP_FOLDER
if [ ! -f ./gcc-$GCC_VERSION.tar.gz ]
then
	echo downloading gcc
	curl -O https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.gz
fi
echo unarchiving gcc
tar xf gcc-$GCC_VERSION.tar.gz
cd gcc-$GCC_VERSION
sh ./contrib/download_prerequisites
cd ..
echo building gcc
mkdir gcc-build
cd gcc-build
echo target: $TARGET
echo prefix: $PREFIX
../gcc-$GCC_VERSION/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-libssp --enable-languages=c,c++ --without-headers --build=$TARGET
make -j8
make all-gcc 
make install-gcc 
make all-target-libgcc 
make install-target-libgcc