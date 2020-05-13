#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <pqxx/pqxx> 

using namespace std;
using namespace pqxx;

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
        cout << "Answer = 6.1.1";
	}
    else
	if (argc == 2)
	{

        try {
            connection C("dbname = testdb user = postgres password = cohondob \
                hostaddr = 127.0.0.1 port = 5432");
                if (C.is_open()) {
                    cout << "Opened database successfully: " << C.dbname() << endl;
                } else {
                    cout << "Can't open database" << endl;
                    return 1;
                }
                /* Create SQL statement */
                sql = "SELECT signalValue from snmpSignals limit 1";

                /* Create a non-transactional object. */
                nontransaction N(C);
                
                /* Execute SQL query */
                result R( N.exec( sql ));
                
                /* List down all the records */
                for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
                    cout << "signalValue = " << c[0].as<int>() << endl;
                }
                cout << "" << endl;
                cout << "Operation done successfully" << endl;
                cout << "" << endl;
                C.disconnect ();

        } catch (const std::exception &e) {
            cerr << e.what() << std::endl;
            return 1;
        }

		postgres->executeQuery("SELECT signalValue from snmpSignals ORDER BY signalTime DESC limit 1", 
			QueryHandler::handleLatestSignalValueQuery);	
	}

	if (argc == 3)
	{
		cout << "Total Disk Space = ";
        system("sudo du -sh /var/lib/ ");

	}
	

	return 0;
}
