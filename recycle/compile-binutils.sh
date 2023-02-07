source ./environment.sh

# old binutils
#export BINUTILS_VERSION=2.24
#echo installing compiling binutils
#cd $TMP_FOLDER
#curl -O http://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz # If the link 404's, look for a more recent version
#tar xf binutils-$BINUTILS_VERSION.tar.gz
#mkdir binutils-build-$BINUTILS_VERSION
#cd binutils-build-$BINUTILS_VERSION
#../binutils-$BINUTILS_VERSION/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$PREFIX 2>&1 | tee configure.log
#make all install 2>&1 | tee make.log

#new binutils
export BINUTILS_VERSION=2.40
export GCC_VERSION=12.2.0
cd $TMP_FOLDER
echo downloading binutils
curl -O http://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz # If the link 404's, look for a more recent version

echo unarchiving binutils
tar xf binutils-$BINUTILS_VERSION.tar.gz
mkdir binutils-build-$BINUTILS_VERSION
cd binutils-build-$BINUTILS_VERSION

echo building binutils
../binutils-$BINUTILS_VERSION/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$PREFIX 2>&1 | tee configure.log
make all install 2>&1 | tee make.log