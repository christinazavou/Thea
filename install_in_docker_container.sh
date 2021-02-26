# I will use OSMesa instead of OpenGL, thus CPU instead of GPU:

apt-get update
apt-get install -y git make cmake unzip vim
apt update
apt install -y gcc-7 g++-7 # or apt install -y g++ gcc
apt-get install -y libgtk2.0-dev libfreeimage-dev
apt install libarpack2
apt-get -y install libgl1 freeglut3-dev


cd TheaInstallationWithDocker
git clone --recursive https://bitbucket.org/sidch/theadepsunix TheaDepsUnix
cd TheaDepsUnix/Source/
./install-defaults.sh --with-osmesa --use-root -j6
./install.sh --use-root --with-expat --with-zlib
# or ./install-defaults.sh --with-osmesa --prefix /TheaInstallationWithDocker/TheaDepsUnix/Source/Installations -j6
#./install.sh --with-eigen3 --prefix=/TheaInstallationWithDocker/TheaDepsUnix/Source/Installations


cd /TheaInstallationWithDocker
git clone --recursive https://github.com/sidch/Thea
cd Thea/Code/Build
mkdir /TheaInstallationWithDocker/MyTheaInstallation
cmake -DWITH_OSMESA=true -DWITH_TESTS=false -DCMAKE_INSTALL_PREFIX=/TheaInstallationWithDocker/MyTheaInstallation -DCMAKE_BUILD_TYPE=Release .
#cmake -DTHEA_DEPS_ROOT=/TheaInstallationWithDocker/TheaDepsUnix/Source/Installations -DCMAKE_BUILD_TYPE=Release .
make -j4


