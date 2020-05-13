# AgenPro - A Custom SNMP Agent using NET-SNMP with C++

[![N|Solid](https://cldup.com/dTxpPi9lDf.thumb.png)](https://nodesource.com/products/nsolid)

[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://travis-ci.org/joemccann/dillinger)

Agenpro is an implmentation of Simple Network Management Protocol. Simple Network Management Protocol (SNMP) is a widely used protocol for monitoring the health and welfare of network equipment (eg. routers), computer equipment and even devices like UPSs. Net-SNMP is a suite of applications used to implement SNMP v1, SNMP v2c and SNMP v3 using both IPv4 and IPv6. The suite includes: 

  - Command-line applications to
    - Retrieve information from an SNMP-capable device, either using single requests (snmpget, snmpgetnext), or multiple requests (snmpwalk, snmptable, snmpdelta).
    - Manipulate configuration information on an SNMP-capable device.
    - Retrieve a fixed collection of information from an SNMP-capable device.
    - Convert between numerical and textual forms of MIB OIDs, and display MIB content and structure 
  - A graphical MIB browser (tkmib), using Tk/perl.
  - A daemon application for receiving SNMP notifications (snmptrapd). Selected notifications can be logged (to syslog, the NT Event Log, or a plain text file), forwarded to another SNMP management system, or passed to an external application.
  - An extensible agent for responding to SNMP queries for management information (snmpd). This includes built-in support for a wide range of MIB information modules, and can be extended using dynamically loaded modules, external scripts and commands, and both the SNMP multiplexing (SMUX) and Agent Extensibility (AgentX) protocols.
  - A library for developing new SNMP applications, with both C and perl APIs.
  

So here we are using the implmentation on both C and C++. 
#### Since SNMP is majorly composed of two thing, 
    - SNMP CLIENT - Used to manage and recieve all the properties of the phsycial devices. 
    - SNMP AGENT Deamon - Used to send trap to Client/Manager Machine.

# Requirements!

    - Linux Machine. (Most preferably Redhat, CentOs or Ubuntu)
    - PostgreSQL
    - C++ / GCC Compiler
    - Alot of packages...

# Database
The database script is included in The repository as `database.sql`. Kindly import this database in the Postgresql server.

    1) Database Name    =   afinitiTest
    2) Database User    =   root
    3) Database User    =   password
    4) Database Table   =   snmpSignals

### StartUp/Installation
    1)  sudo apt-get update -y
    2)  sudo apt-get install -y libudev-dev
    3)  sudo apt-get install snmp snmp-mibs-downloader
    4)  sudo apt-get install snmpd
    5)  sudo apt install libsnmp-dev
    6)  git clone https://github.com/jtv/libpqxx
    7)  cd ./libpqxx
    8)  ./configure
    9)  sudo make
    10) sudo make install
    11) sudo cp ./snmpd.conf /etc/snmp
    12) sudo service postgresql restart
    13) sudo service snmpd stop
    14) sudo service snmpd start
    
# Run this Command   
``` bash
$ sudo ./getting-started.sh
```
    
# Answers
    1)      snmpget -v 2c -c public localhost 1.3.6.1.4.1.53864.1.0
    Result) iso.3.6.1.4.1.53864.1.0 = STRING: "6.1.1"

    2)      snmpget -v 2c -c public localhost 1.3.6.1.4.1.53864.3.0
    Result) iso.3.6.1.4.1.53864.1.0 = STRING: "10"

    3)      snmpget -v 2c -c public localhost 1.3.6.1.4.1.53864.2.0
    Result) iso.3.6.1.4.1.53864.2.0 = INTEGER: 606191616

    
##### NOTE 
###### This Implmentation was done on CentOS Virtual Machine on GCP. You can test this with this IP Address - 34.71.230.62 
