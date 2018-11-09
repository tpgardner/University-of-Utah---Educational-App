#update and upgrade software
sudo apt-get update
sudo apt-get upgrade
sudo apt-get dist-upgrade

#install dependencies
sudo apt-get install build-essential cmake make git checkinstall leafpad gksudo 

#install qt dep
sudo apt-get install mesa-common-dev libfontconfig1 

#download and install qt (32 bit)
wget http://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-linux-x32-5.7.0.run
chmod +x qt-opensource-linux-x32-5.7.0.run
./qt-opensource-linux-x32-5.7.0.run

#install sfml
sudo apt-get install libsfml-dev

#install sql
sudo apt-get install mysql-server mysql-client libmysqlclient-dev
#setup sql
sudo mysql_secure_installation
sudo mysql_install_db

#dependencies for box2D
sudo apt-get install libglew-dev libglfw3-dev libglfw-dev

#install 
mkdir ~/Git/test/Box2D
cd ~/Git/test/Box2D
git clone https://github.com/erincatto/Box2D.git Box2D
cd Box2D/Box2D/Build
cmake ../ -DBOX2D_INSTALL=ON -DBOX2D_BUILD_SHARED=ON -DCMAKE_CXX_FLAGS=-std=c++11 .. 
make
# it will throw an error. But will will ignore it.

#install the shared libraries
sudo cp ./Box2D/libBox2D.so /usr/local/lib
sudo cp ./Box2D/libBox2D.so.2.3.2 /usr/local/lib

# and your done. 






