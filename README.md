# Compiling grpc protocol
在编译grpc时候，项目需要gcc-6环境;
sudo apt-get update && \
sudo apt-get install build-essential software-properties-common -y && \
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
sudo apt-get update && \
sudo apt-get install gcc-snapshot -y && \
sudo apt-get update && \
sudo apt-get install gcc-6 g++-6 -y && \
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ g++ /usr/bin/g++-6
apt-get install libc-ares-dev   //-lcares
apt-get install zlib1g-dev

# Thanks
https://github.com/zrax/libvbox
