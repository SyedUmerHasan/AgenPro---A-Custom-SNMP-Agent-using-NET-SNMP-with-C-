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

### StartUp/Installation for Debian/Ubuntu
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
### Installing package for CentOS/RedHat

    1)  yum -y install net-snmp net-snmp-utils
    2)  mv /etc/snmp/snmpd.conf /etc/snmp/snmpd.conf.orig
    3)  git clone https://github.com/jtv/libpqxx
    4)  cd ./libpqxx
    5)  ./configure
    6)  sudo make
    7)  sudo make install
    8)  sudo cp ./snmpd.conf /etc/snmp
    9)  sudo service postgresql restart
    10) sudo service snmpd stop
    11) sudo service snmpd start


Open `/etc/snmp/snmpd.conf` replace content of the file with the following
```
# `public` is the community key
# use something else if you wish and remember it for later use
com2sec AllUser default public
group AllGroup v2c AllUser
view AllView included .1
access AllGroup "" any noauth exact AllView none none


# Map 'idv90we3rnov90wer' community to the 'ConfigUser'
# Map '209ijvfwer0df92jd' community to the 'AllUser'
#       sec.name        source          community
com2sec ConfigUser      default         idv90we3rnov90wer
com2sec AllUser         default         209ijvfwer0df92jd
# Map 'ConfigUser' to 'ConfigGroup' for SNMP Version 2c
# Map 'AllUser' to 'AllGroup' for SNMP Version 2c
#                       sec.model       sec.name
group   ConfigGroup     v2c             ConfigUser
group   AllGroup        v2c             AllUser
# Define 'SystemView', which includes everything under .1.3.6.1.2.1.1 (or .1.3.6.1.2.1.25.1)
# Define 'AllView', which includes everything under .1
#                       incl/excl       subtree
view    SystemView      included        .1.3.6.1.2.1.1
view    SystemView      included        .1.3.6.1.2.1.25.1.1
view    AllView         included        .1
# Give 'ConfigGroup' read access to objects in the view 'SystemView'
# Give 'AllGroup' read access to objects in the view 'AllView'
#                       context model   level   prefix  read            write   notify
access  ConfigGroup     ""      any     noauth  exact   SystemView      none    none
access  AllGroup        ""      any     noauth  exact   AllView         none    none
```

## Restart Service and set to auto-run:
    1) service snmpd restart
    2) chkconfig snmpd on
## TEST
### snmpwalk -v 2c -c <community> 127.0.0.1 .1.3.6.1.4.1.2021.11.50.0
### snmpwalk -v 2c -c <community> 127.0.0.1 .1.3.6.1.4.1.2021.4.6.0
### snmpwalk -v 2c -c idv90we3rnov90wer -O e 127.0.0.1
### snmpwalk -v 2c -c 209ijvfwer0df92jd -O e 127.0.0.1


## Configure Firewall:
```
firewall-cmd --permanent --zone=public --add-port=161/udp
firewall-cmd --permanent --zone=public --add-port=162/udp
firewall-cmd --reload
firewall-cmd --list-all
```

# Run this Command   
``` bash
$ sudo ./getting-started.sh
```

## Create MIB
Lets create our first mib
inside `/usr/share/snmp/mibs` folder create a **file** named `GET-LATEST-SIGNALS-MIB.txt` and add following content to it

```
GET-LATEST-SIGNALS-MIB DEFINITIONS ::= BEGIN

IMPORTS
    MODULE-IDENTITY, OBJECT-TYPE, enterprises FROM SNMPv2-SMI
;

signalsInfo MODULE-IDENTITY
    LAST-UPDATED "202005130000Z"
    ORGANIZATION "Sample Organization"
    CONTACT-INFO    
         "postal:   umerhasan2010

          email:    umerhasan2010@gmail.com"
    DESCRIPTION
        "Mib description "
    REVISION     "202005130000Z"
    DESCRIPTION
        "My sample description"
    REVISION     "202005130000Z"
    DESCRIPTION
        "First draft"
    ::= { enterprises 53864 }

--
-- top level structure
--
latestSignalValue       OBJECT IDENTIFIER ::= { signalsInfo 1 }

--
-- Example scalars
--

oversightInteger OBJECT-TYPE
    SYNTAX      OCTET STRING
    MAX-ACCESS  read-only
    STATUS      current
    DESCRIPTION
        "the latest value of signal"
    DEFVAL { "hello" }
    ::= { latestSignalValue 1 }


END
```
# Test
    snmptranslate -m +GET-LATEST-SIGNALS-MIB -IR -On oversightInteger
# Result 
    .1.3.6.1.4.1.53864.1.1

    
# Answers
    1)      snmpget -v 2c -c public 127.0.0.1 1.3.6.1.4.1.53864.1.0
    Result) iso.3.6.1.4.1.53864.1.0 = STRING: "6.1.1"

    2)      snmpget -v 2c -c public 127.0.0.1 1.3.6.1.4.1.53864.3.0
    Result) iso.3.6.1.4.1.53864.1.0 = STRING: "10"

    3)      snmpget -v 2c -c public 127.0.0.1 1.3.6.1.4.1.53864.2.0
    Result) iso.3.6.1.4.1.53864.2.0 = INTEGER: 606191616


    
##### NOTE 
###### This Implmentation was done on CentOS Virtual Machine on GCP. You can test this with this IP Address - 34.71.230.62 


## Time Distribution
  1) R&D on SNMP - almost 1 day
  2) Learning go Language - 3 hours (But due to time limit, decide to go back with C++)
  3) Downloaded Open Source SNMP Softwares. (Trial of SNMP real time examples)
  4) Resolving Dependency 
  
