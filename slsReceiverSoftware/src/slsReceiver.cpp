/********************************************//**
 * @file slsReceiver.cpp
 * @short creates the UDP and TCP class objects
 ***********************************************/

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <getopt.h>

#include "slsReceiver.h"
#include "gitInfoReceiver.h"

using namespace std;



slsReceiver::slsReceiver(int argc, char *argv[], int &success):
		tcpipInterface (NULL),
		udp_interface (NULL)
{
	success=OK;

	// options
	map<string, string> configuration_map;
	int tcpip_port_no = 1954;
	string fname = "";
	int64_t tempval = 0;

	//parse command line for config
	static struct option long_options[] = {
			// These options set a flag.
			//{"verbose", no_argument,       &verbose_flag, 1},
			// These options don’t set a flag. We distinguish them by their indices.
			{"config",     	required_argument,  0, 'f'},
			{"rx_tcpport",  required_argument,  0, 't'},
			{"version",  	no_argument,  		0, 'v'},
			{"help",  		no_argument,       	0, 'h'},
			{0, 			0, 					0, 	0}
	};


	// getopt_long stores the option index here.
	int option_index = 0;
	int c = 0;

	while ( c != -1 ){
		c = getopt_long (argc, argv, "hvf:t:", long_options, &option_index);

		// Detect the end of the options.
		if (c == -1)
			break;

		switch(c){

		case 'f':
			fname = optarg;
#ifdef VERYVERBOSE
			FILE_LOG(logDEBUG) << long_options[option_index].name << " " << optarg << endl;
#endif
			break;

		case 't':
			sscanf(optarg, "%d", &tcpip_port_no);
			break;

		case 'v':
			tempval = GITREV;
			tempval = (tempval <<32) | GITDATE;
			cout << "SLS Receiver " << GITBRANCH << " (0x" << hex << tempval << ")" << endl;
			success = FAIL; // to exit
			break;

		case 'h':
		default:
			string help_message = "\n"
					+ string(argv[0]) + "\n"
					+ "Usage: " + string(argv[0]) + " [arguments]\n"
					+ "Possible arguments are:\n"
					+ "\t-f, --config <fname>    : Loads config from file\n"
					+ "\t-t, --rx_tcpport <port> : TCP Communication Port with client. \n"
					+ "\t                          Default: 1954. Required for multiple \n"
					+ "\t                          receivers\n\n";

			FILE_LOG(logINFO) << help_message << endl;
			break;

		}
	}

	if( !fname.empty() ){
		try{
			FILE_LOG(logINFO) << "config file name " << fname;
			success = read_config_file(fname, &tcpip_port_no, &configuration_map);
			//VERBOSE_PRINT("Read configuration file of " + iline + " lines");
		}
		catch(...){
			FILE_LOG(logERROR) << "Coult not open configuration file " << fname ;
			success = FAIL;
		}
	}

	if (success==OK){
		FILE_LOG(logINFO) << "SLS Receiver starting TCP Server on port " << tcpip_port_no << endl;
		tcpipInterface = new slsReceiverTCPIPInterface(success, udp_interface, tcpip_port_no);
	}
}


slsReceiver::~slsReceiver() {
	if(tcpipInterface)
		delete tcpipInterface;
}


int slsReceiver::start() {
	return tcpipInterface->start();
}


void slsReceiver::stop() {
	tcpipInterface->stop();
}


int64_t slsReceiver::getReceiverVersion(){
	return tcpipInterface->getReceiverVersion();
}


void slsReceiver::registerCallBackStartAcquisition(int (*func)(char*, char*, uint64_t, uint32_t, void*),void *arg){
  //tcpipInterface
	if(udp_interface)
		udp_interface->registerCallBackStartAcquisition(func,arg);
	else
		tcpipInterface->registerCallBackStartAcquisition(func,arg);
}



void slsReceiver::registerCallBackAcquisitionFinished(void (*func)(uint64_t, void*),void *arg){
  //tcpipInterface
	if(udp_interface)
		udp_interface->registerCallBackAcquisitionFinished(func,arg);
	else
		tcpipInterface->registerCallBackAcquisitionFinished(func,arg);
}


void slsReceiver::registerCallBackRawDataReady(void (*func)(uint64_t, uint32_t, uint32_t, uint64_t, uint64_t, uint16_t, uint16_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, uint8_t,
		char*, uint32_t, void*),void *arg){
	//tcpipInterface
	if(udp_interface)
		udp_interface->registerCallBackRawDataReady(func,arg);
	else
		tcpipInterface->registerCallBackRawDataReady(func,arg);
}


