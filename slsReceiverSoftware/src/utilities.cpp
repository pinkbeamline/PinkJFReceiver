#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <fstream>

#include <map>

#include "utilities.h"


using namespace std;


int read_config_file(string fname, int *tcpip_port_no, map<string, string> * configuration_map ){
	
	ifstream infile;
	string sLine,sargname, sargvalue;
	int iline = 0;
	int success = slsReceiverDefs::OK;



	infile.open(fname.c_str(), ios_base::in);
	if (infile.is_open()) {
		while(infile.good()){
			getline(infile,sLine);
			iline++;
			
			//VERBOSE_PRINT(sLine);
			
			if(sLine.find('#') != string::npos)
				continue;

			else if(sLine.length()<2)
				continue;

			else{
				istringstream sstr(sLine);
				
				//parameter name
				if(sstr.good()){
					sstr >> sargname;
				
					if (! sstr.good())
						continue;

					sstr >> sargvalue;
					(*configuration_map)[sargname] = sargvalue;
				}
				//tcp port
				if(sargname=="rx_tcpport"){
					if(sstr.good()) {
						sstr >> sargname;
						if(sscanf(sargname.c_str(),"%d",tcpip_port_no))
							cprintf(RESET, "dataport: %d\n" , *tcpip_port_no);
						else{
							cprintf(RED, "could not decode port in config file. Exiting.\n");
							success = slsReceiverDefs::FAIL;
						}
					}
				}
			}
		}
		infile.close();
	}
	
	return success;
}




