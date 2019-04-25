/********************************************//**
 * @file UDPStandardImplementation.cpp
 * @short does all the functions for a receiver, set/get parameters, start/stop etc.
 ***********************************************/


#include "UDPStandardImplementation.h"
#include "GeneralData.h"
#include "Listener.h"
#include "DataProcessor.h"
#include "DataStreamer.h"
#include "Fifo.h"
#include "ZmqSocket.h" 		//just for the zmq port define

#include <cstdlib>			//system
#include <cstring>			//strcpy
#include <errno.h>			//eperm
using namespace std;


/** cosntructor & destructor */

UDPStandardImplementation::UDPStandardImplementation() {
	InitializeMembers();
}


UDPStandardImplementation::~UDPStandardImplementation() {
	DeleteMembers();
}


void UDPStandardImplementation::DeleteMembers() {
	if (generalData) { delete generalData; generalData=0;}
	for (vector<Listener*>::const_iterator it = listener.begin(); it != listener.end(); ++it)
		delete(*it);
	listener.clear();
	for (vector<DataProcessor*>::const_iterator it = dataProcessor.begin(); it != dataProcessor.end(); ++it)
		delete(*it);
	dataProcessor.clear();
	for (vector<DataStreamer*>::const_iterator it = dataStreamer.begin(); it != dataStreamer.end(); ++it)
		delete(*it);
	dataStreamer.clear();
	for (vector<Fifo*>::const_iterator it = fifo.begin(); it != fifo.end(); ++it)
		delete(*it);
	fifo.clear();
}


void UDPStandardImplementation::InitializeMembers() {
	UDPBaseImplementation::initializeMembers();
	acquisitionPeriod = SAMPLE_TIME_IN_NS;

	//*** receiver parameters ***
	numThreads = 1;
	numberofJobs = 1;

	//** class objects ***
	generalData = 0;
}



/*** Overloaded Functions called by TCP Interface ***/

uint64_t UDPStandardImplementation::getTotalFramesCaught() const {
	uint64_t sum = 0;
	uint32_t flagsum = 0;

	vector<DataProcessor*>::const_iterator it;
	for (vector<DataProcessor*>::const_iterator it = dataProcessor.begin(); it != dataProcessor.end(); ++it) {
		flagsum += ((*it)->GetMeasurementStartedFlag() ? 1 : 0);
		sum += (*it)->GetNumTotalFramesCaught();
	}

	//no data processed
	if (flagsum != dataProcessor.size()) return 0;

	return (sum/dataProcessor.size());
}

uint64_t UDPStandardImplementation::getFramesCaught() const {
	uint64_t sum = 0;
	uint32_t flagsum = 0;

	for (vector<DataProcessor*>::const_iterator it = dataProcessor.begin(); it != dataProcessor.end(); ++it) {
		flagsum += ((*it)->GetAcquisitionStartedFlag() ? 1 : 0);
		sum += (*it)->GetNumFramesCaught();
	}

	//no data processed
	if (flagsum != dataProcessor.size()) return 0;

	return (sum/dataProcessor.size());
}

int64_t UDPStandardImplementation::getAcquisitionIndex() const {
	uint64_t sum = 0;
	uint32_t flagsum = 0;

	for (vector<DataProcessor*>::const_iterator it = dataProcessor.begin(); it != dataProcessor.end(); ++it){
		flagsum += ((*it)->GetAcquisitionStartedFlag() ? 1 : 0);
		sum += (*it)->GetActualProcessedAcquisitionIndex();
	}

	//no data processed
	if (flagsum != dataProcessor.size()) return -1;

	return (sum/dataProcessor.size());
}


void UDPStandardImplementation::setFileFormat(const fileFormat f){
	switch(f){
#ifdef HDF5C
	case HDF5:
		fileFormatType = HDF5;
		break;
#endif
	default:
		fileFormatType = BINARY;
		break;
	}
	//destroy file writer, set file format and create file writer
	for (vector<DataProcessor*>::const_iterator it = dataProcessor.begin(); it != dataProcessor.end(); ++it)
		(*it)->SetFileFormat(f);

	FILE_LOG(logINFO) << "File Format:" << getFileFormatType(fileFormatType);
}


void UDPStandardImplementation::setFileWriteEnable(const bool b){

	if (fileWriteEnable != b){
		fileWriteEnable = b;
		for (unsigned int i = 0; i < dataProcessor.size(); ++i) {
				dataProcessor[i]->SetupFileWriter(fileWriteEnable, (int*)numDet, fileName, filePath, &fileIndex, &frameIndexEnable,
					&overwriteEnable, &detID, &numThreads, &numberOfFrames, &dynamicRange, &udpPortNum[i], generalData);
		}
	}

	FILE_LOG(logINFO) << "File Write Enable: " << stringEnable(fileWriteEnable);
}




int UDPStandardImplementation::setShortFrameEnable(const int i) {
	if (myDetectorType != GOTTHARD) {
		cprintf(RED, "Error: Can not set short frame for this detector\n");
		return FAIL;
	}

	if (shortFrameEnable != i) {
		shortFrameEnable = i;

		if (generalData)
			delete generalData;
		if (i != -1)
			generalData = new ShortGotthardData();
		else
			generalData = new GotthardData();
		numberofJobs = -1; //changes to imagesize has to be noted to recreate fifo structure
		if (SetupFifoStructure() == FAIL)
			return FAIL;

		for (vector<Listener*>::const_iterator it = listener.begin(); it != listener.end(); ++it)
			(*it)->SetGeneralData(generalData);
		for (vector<DataProcessor*>::const_iterator it = dataProcessor.begin(); it != dataProcessor.end(); ++it)
			(*it)->SetGeneralData(generalData);
		for (vector<DataStreamer*>::const_iterator it = dataStreamer.begin(); it != dataStreamer.end(); ++it)
			(*it)->SetGeneralData(generalData);
	}
	FILE_LOG(logINFO) << "Short Frame Enable: " << shortFrameEnable;
	return OK;
}


int UDPStandardImplementation::setFrameToGuiFrequency(const uint32_t freq) {
	if (frameToGuiFrequency != freq) {
		frameToGuiFrequency = freq;

		/*//only the ones lisening to more than 1 frame at a time needs to change fifo structure
		switch (myDetectorType) {
		case GOTTHARD:
		case PROPIX:
		if (SetupFifoStructure() == FAIL)
			return FAIL;
		break;
		default:
			break;
		}*/
	}
	FILE_LOG(logINFO) << "Frame to Gui Frequency: " << frameToGuiFrequency;
	return OK;
}


int UDPStandardImplementation::setDataStreamEnable(const bool enable) {

	if (dataStreamEnable != enable) {
		dataStreamEnable = enable;

		//data sockets have to be created again as the client ones are
		for (vector<DataStreamer*>::const_iterator it = dataStreamer.begin(); it != dataStreamer.end(); ++it)
			delete(*it);
		dataStreamer.clear();

		if (enable) {
			bool error = false;
			for ( int i = 0; i < numThreads; ++i ) {
				dataStreamer.push_back(new DataStreamer(fifo[i], &dynamicRange, &shortFrameEnable, &fileIndex));
				dataStreamer[i]->SetGeneralData(generalData);
				if (dataStreamer[i]->CreateZmqSockets(&numThreads, streamingPort) == FAIL) {
					error = true;
					break;
				}
			}
			if (DataStreamer::GetErrorMask() || error) {
				if (DataStreamer::GetErrorMask())
					cprintf(RED,"Error: Could not create data callback threads\n");
				else
					cprintf(RED,"Error: Could not create zmq sockets\n");
				for (vector<DataStreamer*>::const_iterator it = dataStreamer.begin(); it != dataStreamer.end(); ++it)
					delete(*it);
				dataStreamer.clear();
				dataStreamEnable = false;
				return FAIL;
			}
			SetThreadPriorities();
		}
	}
	FILE_LOG(logINFO) << "Data Send to Gui: " << dataStreamEnable;
	return OK;
}


int UDPStandardImplementation::setDynamicRange(const uint32_t i) {
	if (dynamicRange != i) {
		dynamicRange = i;

		//side effects
		generalData->SetDynamicRange(i,tengigaEnable);

		numberofJobs = -1; //changes to imagesize has to be noted to recreate fifo structure
		if (SetupFifoStructure() == FAIL)
			return FAIL;
	}
	FILE_LOG(logINFO) << "Dynamic Range: " << dynamicRange;
	return OK;
}


int UDPStandardImplementation::setTenGigaEnable(const bool b) {
	if (tengigaEnable != b) {
		tengigaEnable = b;
		//side effects
		generalData->SetTenGigaEnable(b,dynamicRange);

		numberofJobs = -1; //changes to imagesize has to be noted to recreate fifo structure
		if (SetupFifoStructure() == FAIL)
			return FAIL;
	}
	FILE_LOG(logINFO) << "Ten Giga: " << stringEnable(tengigaEnable);
	return OK;
}


int UDPStandardImplementation::setFifoDepth(const uint32_t i) {
	if (fifoDepth != i) {
		fifoDepth = i;

		numberofJobs = -1; //changes to imagesize has to be noted to recreate fifo structure
		if (SetupFifoStructure() == FAIL)
			return FAIL;
	}
	FILE_LOG(logINFO) << "Fifo Depth: " << i;
	return OK;
}


void UDPStandardImplementation::setSilentMode(const uint32_t i){
	silentMode = i;

	Listener::SetSilentMode(i);
	DataProcessor::SetSilentMode(i);
	DataStreamer::SetSilentMode(i);

	FILE_LOG(logINFO) << "Silent Mode: " << i;
}


int UDPStandardImplementation::setDetectorType(const detectorType d) {
	FILE_LOG(logDEBUG) << "Setting receiver type";
	DeleteMembers();
	InitializeMembers();
	myDetectorType = d;
	switch(myDetectorType) {
	case GOTTHARD:
	case PROPIX:
	case MOENCH:
	case EIGER:
	case JUNGFRAUCTB:
	case JUNGFRAU:
		FILE_LOG(logINFO) << " ***** " << getDetectorType(d) << " Receiver *****";
		break;
	default:
		FILE_LOG(logERROR) << "This is an unknown receiver type " << (int)d;
		return FAIL;
	}


	//set detector specific variables
	switch(myDetectorType) {
	case GOTTHARD:		generalData = new GotthardData();	break;
	case PROPIX:		generalData = new PropixData();		break;
	case MOENCH:		generalData = new Moench02Data();	break;
	case EIGER:			generalData = new EigerData();		break;
	case JUNGFRAUCTB:	generalData = new JCTBData();		break;
	case JUNGFRAU:		generalData = new JungfrauData();	break;
	default: break;
	}
	numThreads = generalData->threadsPerReceiver;
	fifoDepth = generalData->defaultFifoDepth;

	//local network parameters
	SetLocalNetworkParameters();

	//create fifo structure
	numberofJobs = -1;
	if (SetupFifoStructure() == FAIL) {
		FILE_LOG(logERROR) << "Could not allocate memory for fifo structure";
		return FAIL;
	}

	//create threads
	for ( int i=0; i < numThreads; ++i ) {
		listener.push_back(new Listener(myDetectorType, fifo[i], &status, &udpPortNum[i], eth, &activated, &numberOfFrames, &dynamicRange));
		dataProcessor.push_back(new DataProcessor(fifo[i], &fileFormatType,
				fileWriteEnable, &dataStreamEnable, &frameToGuiFrequency, &frameToGuiTimerinMS,
				rawDataReadyCallBack,pRawDataReady));
		if (Listener::GetErrorMask() || DataProcessor::GetErrorMask()) {
			FILE_LOG(logERROR) << "Could not create listener/dataprocessor threads (index:" << i << ")";
			for (vector<Listener*>::const_iterator it = listener.begin(); it != listener.end(); ++it)
				delete(*it);
			listener.clear();
			for (vector<DataProcessor*>::const_iterator it = dataProcessor.begin(); it != dataProcessor.end(); ++it)
				delete(*it);
			dataProcessor.clear();
			return FAIL;
		}
	}

	//set up writer and callbacks
	for (vector<Listener*>::const_iterator it = listener.begin(); it != listener.end(); ++it)
		(*it)->SetGeneralData(generalData);
	for (vector<DataProcessor*>::const_iterator it = dataProcessor.begin(); it != dataProcessor.end(); ++it)
		(*it)->SetGeneralData(generalData);

	SetThreadPriorities();

	FILE_LOG(logDEBUG) << " Detector type set to " << getDetectorType(d);
	return OK;
}




void UDPStandardImplementation::setDetectorPositionId(const int i){

	detID = i;
	FILE_LOG(logINFO) << "Detector Position Id:" << detID;
	for (unsigned int i = 0; i < dataProcessor.size(); ++i) {
		dataProcessor[i]->SetupFileWriter( fileWriteEnable, (int*)numDet, fileName, filePath, &fileIndex, &frameIndexEnable,
									&overwriteEnable, &detID, &numThreads, &numberOfFrames, &dynamicRange, &udpPortNum[i], generalData);
	}
}


void UDPStandardImplementation::resetAcquisitionCount() {
	for (vector<Listener*>::const_iterator it = listener.begin(); it != listener.end(); ++it)
		(*it)->ResetParametersforNewAcquisition();

	for (vector<DataProcessor*>::const_iterator it = dataProcessor.begin(); it != dataProcessor.end(); ++it)
		(*it)->ResetParametersforNewAcquisition();

	for (vector<DataStreamer*>::const_iterator it = dataStreamer.begin(); it != dataStreamer.end(); ++it)
		(*it)->ResetParametersforNewAcquisition();

	FILE_LOG(logINFO) << "Acquisition Count has been reset";
}



int UDPStandardImplementation::startReceiver(char *c) {
	cprintf(RESET,"\n");
	FILE_LOG(logINFO) << "Starting Receiver";

	ResetParametersforNewMeasurement();

	//listener
	if (CreateUDPSockets() == FAIL) {
		strcpy(c,"Could not create UDP Socket(s).");
		FILE_LOG(logERROR) << c;
		return FAIL;
	}

	//callbacks
	if (startAcquisitionCallBack) {
		startAcquisitionCallBack(filePath, fileName, fileIndex,
				(generalData->imageSize) * numberofJobs + (generalData->fifoBufferHeaderSize), pStartAcquisition);
		if (rawDataReadyCallBack != NULL) {
			FILE_LOG(logINFO) << "Data Write has been defined externally";
		}
	}

	//processor->writer
	if (fileWriteEnable) {
		if (SetupWriter() == FAIL) {
			strcpy(c,"Could not create file.");
			FILE_LOG(logERROR) << c;
			return FAIL;
		}
	} else
		FILE_LOG(logINFO) << "File Write Disabled";

	FILE_LOG(logINFO) << "Ready ...";

	//status
	status = RUNNING;

	//Let Threads continue to be ready for acquisition
	StartRunning();

	FILE_LOG(logINFO)  << "Receiver Started";
	FILE_LOG(logINFO)  << "Status: " << runStatusType(status);
	return OK;
}



void UDPStandardImplementation::stopReceiver(){
	FILE_LOG(logINFO)  << "Stopping Receiver";

	//set status to transmitting
	startReadout();

	//wait for the processes to be done
	while(Listener::GetRunningMask()){
		usleep(5000);
	}
	while(DataProcessor::GetRunningMask()){
		usleep(5000);
	}

	//create virtual file
	if (fileWriteEnable && fileFormatType == HDF5) {
		uint64_t maxIndexCaught = 0;
		bool anycaught = false;
		for (vector<DataProcessor*>::const_iterator it = dataProcessor.begin(); it != dataProcessor.end(); ++it) {
			maxIndexCaught = max(maxIndexCaught, (*it)->GetProcessedMeasurementIndex());
			if((*it)->GetMeasurementStartedFlag())
				anycaught = true;
		}

		//to create virtual file & set files/acquisition to 0 (only hdf5 at the moment)
		dataProcessor[0]->EndofAcquisition(anycaught, maxIndexCaught);
	}

	while(DataStreamer::GetRunningMask()){
		usleep(5000);
	}

	status = RUN_FINISHED;
	FILE_LOG(logINFO)  << "Status: " << runStatusType(status);


	{	//statistics
		uint64_t tot = 0;
		for (int i = 0; i < numThreads; i++) {
			tot += dataProcessor[i]->GetNumFramesCaught();

			uint64_t missingpackets = numberOfFrames*generalData->packetsPerFrame-listener[i]->GetPacketsCaught();
			if ((int)missingpackets > 0) {
				cprintf(RED, "\n[Port %d]\n",udpPortNum[i]);
				cprintf(RED, "Missing Packets\t\t: %lld\n",(long long int)missingpackets);
				cprintf(RED, "Complete Frames\t\t: %lld\n",(long long int)dataProcessor[i]->GetNumFramesCaught());
				cprintf(RED, "Last Frame Caught\t: %lld\n",(long long int)listener[i]->GetLastFrameIndexCaught());
			}else{
				cprintf(GREEN, "\n[Port %d]\n",udpPortNum[i]);
				cprintf(GREEN, "Missing Packets\t\t: %lld\n",(long long int)missingpackets);
				cprintf(GREEN, "Complete Frames\t\t: %lld\n",(long long int)dataProcessor[i]->GetNumFramesCaught());
				cprintf(GREEN, "Last Frame Caught\t: %lld\n",(long long int)listener[i]->GetLastFrameIndexCaught());
			}
		}
		if(!activated)
			cprintf(RED,"Note: Deactivated Receiver\n");
		//callback
		if (acquisitionFinishedCallBack)
			acquisitionFinishedCallBack((tot/numThreads), pAcquisitionFinished);
	}

	//change status
	status = IDLE;

	FILE_LOG(logINFO)  << "Receiver Stopped";
	FILE_LOG(logINFO)  << "Status: " << runStatusType(status);
}



void UDPStandardImplementation::startReadout(){
	if(status == RUNNING){

		//needs to wait for packets only if activated
		if(activated){

			//current packets caught
			volatile int totalP = 0,prev=-1;
			for (vector<Listener*>::const_iterator it = listener.begin(); it != listener.end(); ++it)
				totalP += (*it)->GetPacketsCaught();

			//wait for all packets
			if((unsigned long long int)totalP!=numberOfFrames*generalData->packetsPerFrame*listener.size()){

				//wait as long as there is change from prev totalP,
				while(prev != totalP){
#ifdef VERY_VERBOSE
					cprintf(MAGENTA,"waiting for all packets prevP:%d totalP:%d\n",
							prev,totalP);

#endif
					//usleep(1*1000*1000);usleep(1*1000*1000);usleep(1*1000*1000);usleep(1*1000*1000);
					usleep(5*1000);/* Need to find optimal time **/

					prev = totalP;
					totalP = 0;

					for (vector<Listener*>::const_iterator it = listener.begin(); it != listener.end(); ++it)
						totalP += (*it)->GetPacketsCaught();
#ifdef VERY_VERBOSE
					cprintf(MAGENTA,"\tupdated:  totalP:%d\n",totalP);
#endif
				}
			}
		}

		//set status
		status = TRANSMITTING;
		FILE_LOG(logINFO) << "Status: Transmitting";
	}
	//shut down udp sockets so as to make listeners push dummy (end) packets for processors
	shutDownUDPSockets();
}


void UDPStandardImplementation::shutDownUDPSockets() {
	for (vector<Listener*>::const_iterator it = listener.begin(); it != listener.end(); ++it)
		(*it)->ShutDownUDPSocket();
}



void UDPStandardImplementation::closeFiles() {
	uint64_t maxIndexCaught = 0;
	bool anycaught = false;
	for (vector<DataProcessor*>::const_iterator it = dataProcessor.begin(); it != dataProcessor.end(); ++it) {
		(*it)->CloseFiles();
		maxIndexCaught = max(maxIndexCaught, (*it)->GetProcessedMeasurementIndex());
		if((*it)->GetMeasurementStartedFlag())
			anycaught = true;
	}
	//to create virtual file & set files/acquisition to 0 (only hdf5 at the moment)
	dataProcessor[0]->EndofAcquisition(anycaught, maxIndexCaught);
}


int UDPStandardImplementation::restreamStop() {
	bool ret = OK;
	for (vector<DataStreamer*>::const_iterator it = dataStreamer.begin(); it != dataStreamer.end(); ++it) {
		if ((*it)->restreamStop() == FAIL)
			ret = FAIL;
	}

	// if fail, prints in datastreamer
	if (ret == OK) {
		FILE_LOG(logINFO) << "Restreaming Dummy Header via ZMQ successful";
	}

	return ret;
}


void UDPStandardImplementation::SetLocalNetworkParameters() {
	//to increase socket receiver buffer size and max length of input queue by changing kernel settings
	if (myDetectorType == EIGER)
		return;

	char command[255];

	//to increase Socket Receiver Buffer size
	sprintf(command,"echo $((%d)) > /proc/sys/net/core/rmem_max",RECEIVE_SOCKET_BUFFER_SIZE);
	if (system(command)) {
		FILE_LOG(logWARNING) << "No root privileges to change Socket Receiver Buffer size (net.core.rmem_max)";
		return;
	}
	FILE_LOG(logINFO) << "Socket Receiver Buffer size (/proc/sys/net/core/rmem_max) modified to " << RECEIVE_SOCKET_BUFFER_SIZE ;


	// to increase Max length of input packet queue
	sprintf(command,"echo %d > /proc/sys/net/core/netdev_max_backlog",MAX_SOCKET_INPUT_PACKET_QUEUE);
	if (system(command)) {
		FILE_LOG(logWARNING) << "No root privileges to change Max length of input packet queue (net.core.rmem_max)";
		return;
	}
	FILE_LOG(logINFO) << "Max length of input packet queue (/proc/sys/net/core/netdev_max_backlog) modified to " << MAX_SOCKET_INPUT_PACKET_QUEUE ;
}



void UDPStandardImplementation::SetThreadPriorities() {

	for (vector<Listener*>::const_iterator it = listener.begin(); it != listener.end(); ++it){
		if ((*it)->SetThreadPriority(LISTENER_PRIORITY) == FAIL) {
			FILE_LOG(logWARNING) << "No root privileges to prioritize listener threads";
			return;
		}
	}
	ostringstream osfn;
	osfn << "Priorities set - "
			"Listener:" << LISTENER_PRIORITY;

	FILE_LOG(logINFO) << osfn.str();
}


int UDPStandardImplementation::SetupFifoStructure() {
		numberofJobs = 1;


	for (vector<Fifo*>::const_iterator it = fifo.begin(); it != fifo.end(); ++it)
		delete(*it);
	fifo.clear();
	for ( int i = 0; i < numThreads; i++ ) {
		//create fifo structure
		bool success = true;
		fifo.push_back( new Fifo (
				(generalData->imageSize) * numberofJobs + (generalData->fifoBufferHeaderSize),
				fifoDepth, success));
		if (!success) {
			cprintf(RED,"Error: Could not allocate memory for fifo structure of index %d\n", i);
			for (vector<Fifo*>::const_iterator it = fifo.begin(); it != fifo.end(); ++it)
				delete(*it);
			fifo.clear();
			return FAIL;
		}
		//set the listener & dataprocessor threads to point to the right fifo
		if(listener.size())listener[i]->SetFifo(fifo[i]);
		if(dataProcessor.size())dataProcessor[i]->SetFifo(fifo[i]);
		if(dataStreamer.size())dataStreamer[i]->SetFifo(fifo[i]);
	}

	FILE_LOG(logINFO) << "Memory Allocated Per Fifo: " << ( ((generalData->imageSize) * numberofJobs + (generalData->fifoBufferHeaderSize)) * fifoDepth) << " bytes" ;
	FILE_LOG(logINFO) << " Fifo structure(s) reconstructed: " << numThreads;
	return OK;
}



void UDPStandardImplementation::ResetParametersforNewMeasurement() {
	Listener::ResetRunningMask();
	DataProcessor::ResetRunningMask();
	DataStreamer::ResetRunningMask();

	for (vector<Listener*>::const_iterator it = listener.begin(); it != listener.end(); ++it)
		(*it)->ResetParametersforNewMeasurement();
	for (vector<DataProcessor*>::const_iterator it = dataProcessor.begin(); it != dataProcessor.end(); ++it)
		(*it)->ResetParametersforNewMeasurement();

	if (dataStreamEnable) {
		char fnametostream[MAX_STR_LENGTH];
		snprintf(fnametostream, MAX_STR_LENGTH, "%s/%s", filePath, fileName);
		for (vector<DataStreamer*>::const_iterator it = dataStreamer.begin(); it != dataStreamer.end(); ++it)
			(*it)->ResetParametersforNewMeasurement(fnametostream);
	}
}



int UDPStandardImplementation::CreateUDPSockets() {
	bool error = false;
	for (unsigned int i = 0; i < listener.size(); ++i)
		if (listener[i]->CreateUDPSockets() == FAIL) {
			error = true;
			break;
		}
	if (error) {
		shutDownUDPSockets();
		return FAIL;
	}

	FILE_LOG(logDEBUG) << "UDP socket(s) created successfully.";
	return OK;
}


int UDPStandardImplementation::SetupWriter() {
	bool error = false;
	for (unsigned int i = 0; i < dataProcessor.size(); ++i)
		if (dataProcessor[i]->CreateNewFile(tengigaEnable,
				numberOfFrames, acquisitionTime, subExpTime, acquisitionPeriod) == FAIL) {
			error = true;
			break;
		}
	if (error) {
		shutDownUDPSockets();
		closeFiles();
		return FAIL;
	}

	return OK;
}


void UDPStandardImplementation::StartRunning() {
	//set running mask and post semaphore to start the inner loop in execution thread
	for (vector<Listener*>::const_iterator it = listener.begin(); it != listener.end(); ++it) {
		(*it)->StartRunning();
		(*it)->Continue();
	}
	for (vector<DataProcessor*>::const_iterator it = dataProcessor.begin(); it != dataProcessor.end(); ++it){
		(*it)->StartRunning();
		(*it)->Continue();
	}
	for (vector<DataStreamer*>::const_iterator it = dataStreamer.begin(); it != dataStreamer.end(); ++it){
		(*it)->StartRunning();
		(*it)->Continue();
	}
}
