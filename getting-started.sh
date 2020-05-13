
echo ""
echo "#################################################"
echo "Welcome! Project dependencies are initializing..."
echo "#################################################"
echo ""

sudo apt-get update -y

echo ""
echo "####################################"
echo "Downloading all required packages..."
echo "####################################"
echo ""

echo ""
echo "######################"
echo "Installing pacakges..."
echo "######################"
echo ""

sudo apt-get install -y libudev-dev

echo ""
echo "#########################################"
echo "Please wait, SNMP Client is installing..."
echo "#########################################"
echo ""

sudo apt-get install snmp snmp-mibs-downloader

echo ""
echo "###############################################"
echo "Please wait, SNMP Agent deamon is installing..."
echo "###############################################"
echo ""

sudo apt-get install snmpd

echo ""
echo "##################################################"
echo "Please wait, SNMP MIBS Downloader is installing..."
echo "##################################################"
echo ""


sudo apt-get install snmp-mibs-downloader


echo ""
echo "################################################"
echo "Please wait, SNMP dependencies are installing..."
echo "################################################"
echo ""

sudo apt install libsnmp-dev

echo ""
echo "#######################"
echo "Prtoject Dependencies Installation Completed."
echo "#######################"
echo ""



echo ""
echo "#######################"
echo "Installing Postgresql for C++ connectivity."
echo "#######################"
echo ""

git clone https://github.com/jtv/libpqxx

echo ""
echo ""
echo "moving in libpqxx folder"
echo ""
echo ""

cd ./libpqxx

echo ""
echo ""
echo "Installing..."

echo ""
echo ""
echo "Please wait, It will take couple of minutes to complete"
echo ""
echo ""



./configure --disable-documentation
sudo make
sudo make install



