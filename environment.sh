

#export HERE="$(cd "$(dirname "$1")"; pwd)/$(basename "$1")"
#export TMP_FOLDER=$HERE/build/tmp/compiler
#export STAGE=${TMP_FOLDER}/i386elfgcc

#mkdir -p $STAGE
#mkdir $STAGE/bin
#export PREFIX=/usr/local/i386elfgcc
#alias g++=$(which g++-12)
#alias cc=$(which g++-12)
#alias c++=$(which g++-12)
#sudo unlink $PREFIX
#sudo ln -s $PWD/build/tmp/compiler/i386elfgcc $PREFIX
export YOS_TARGET=i686-elf
export YOS_GCC_VERSION=12.2.0
export YOS_HOMEBREW_REPO=/opt/homebrew/Cellar
#ln -s /opt/homebrew/Cellar/gcc/12.2.0/bin/gcc-12 /opt/homebrew/Cellar/gcc/12.2.0/bin/gcc
#ln -s /opt/homebrew/Cellar/gcc/12.2.0/bin/g++-12 /opt/homebrew/Cellar/gcc/12.2.0/bin/g++

unlink $YOS_HOMEBREW_REPO/$YOS_TARGET-gcc/$YOS_GCC_VERSION/bin/gcc
unlink $YOS_HOMEBREW_REPO/$YOS_TARGET-gcc/$YOS_GCC_VERSION/bin/g++
ln -s $YOS_HOMEBREW_REPO/$YOS_TARGET-gcc/$YOS_GCC_VERSION/bin/$YOS_TARGET-gcc $YOS_HOMEBREW_REPO/$YOS_TARGET-gcc/$YOS_GCC_VERSION/bin/gcc
ln -s $YOS_HOMEBREW_REPO/$YOS_TARGET-gcc/$YOS_GCC_VERSION/bin/$YOS_TARGET-g++ $YOS_HOMEBREW_REPO/$YOS_TARGET-gcc/$YOS_GCC_VERSION/bin/g++
 


#export PATH="/opt/homebrew/Cellar/gcc/12.2.0/bin:$PREFIX/bin:$PATH"
export PATH="$YOS_HOMEBREW_REPO/$YOS_TARGET-gcc/$YOS_GCC_VERSION/bin:$PATH"

#alias ld=$PREFIX/bin/i386-elf-ld
#export CC=/opt/homebrew/Cellar/gcc/12.2.0/bin/gcc-12
#export LD=$PREFIX/bin/i386-elf-ld


