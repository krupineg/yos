

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
export TARGET=i686-elf
#ln -s /opt/homebrew/Cellar/gcc/12.2.0/bin/gcc-12 /opt/homebrew/Cellar/gcc/12.2.0/bin/gcc
#ln -s /opt/homebrew/Cellar/gcc/12.2.0/bin/g++-12 /opt/homebrew/Cellar/gcc/12.2.0/bin/g++

unlink /opt/homebrew/Cellar/i686-elf-gcc/12.2.0/bin/gcc
unlink /opt/homebrew/Cellar/i686-elf-gcc/12.2.0/bin/g++
ln -s /opt/homebrew/Cellar/i686-elf-gcc/12.2.0/bin/i686-elf-gcc /opt/homebrew/Cellar/i686-elf-gcc/12.2.0/bin/gcc
ln -s /opt/homebrew/Cellar/i686-elf-gcc/12.2.0/bin/i686-elf-g++ /opt/homebrew/Cellar/i686-elf-gcc/12.2.0/bin/g++
 


#export PATH="/opt/homebrew/Cellar/gcc/12.2.0/bin:$PREFIX/bin:$PATH"
export PATH="/opt/homebrew/Cellar/i686-elf-gcc/12.2.0/bin:$PATH"

#alias ld=$PREFIX/bin/i386-elf-ld
#export CC=/opt/homebrew/Cellar/gcc/12.2.0/bin/gcc-12
#export LD=$PREFIX/bin/i386-elf-ld


