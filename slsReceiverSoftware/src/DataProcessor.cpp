/************************************************
 * @file DataProcessor.cpp
 * @short creates data processor thread that
 * pulls pointers to memory addresses from fifos
 * and processes data stored in them & writes them to file
 ***********************************************/


#include "DataProcessor.h"
#include "GeneralData.h"
#include "Fifo.h"
#include "BinaryFile.h"
#ifdef HDF5C
#include "HDF5File.h"
#endif
#include "DataStreamer.h"

#include <iostream>
#include <errno.h>
#include <cstring>
using namespace std;

const string DataProcessor::TypeName = "DataProcessor";

int DataProcessor::NumberofDataProcessors(0);

uint64_t DataProcessor::ErrorMask(0x0);

uint64_t DataProcessor::RunningMask(0x0);

pthread_mutex_t DataProcessor::Mutex = PTHREAD_MUTEX_INITIALIZER;

bool DataProcessor::SilentMode(false);


DataProcessor::DataProcessor(Fifo*& f, fileFormat* ftype, bool fwenable, bool* dsEnable,
		uint32_t* freq, uint32_t* timer,
		void (*dataReadycb)(uint64_t, uint32_t, uint32_t, uint64_t, uint64_t, uint16_t, uint16_t, uint16_t, uint16_t, uint32_t, uint16_t, uint8_t, uint8_t,
				char*, uint32_t, void*),
		void *pDataReadycb) :

		ThreadObject(NumberofDataProcessors),
		generalData(0),
		fifo(f),
		file(0),
		dataStreamEnable(dsEnable),
		fileFormatType(ftype),
		fileWriteEnable(fwenable),
		streamingFrequency(freq),
		streamingTimerInMs(timer),
		currentFreqCount(0),
		xcoord(0),
		acquisitionStartedFlag(false),
		measurementStartedFlag(false),
		firstAcquisitionIndex(0),
		firstMeasurementIndex(0),
		numTotalFramesCaught(0),
		numFramesCaught(0),
		currentFrameIndex(0),
		rawDataReadyCallBack(dataReadycb),
		pRawDataReady(pDataReadycb)
{
	if(ThreadObject::CreateThread()){
		pthread_mutex_lock(&Mutex);
		ErrorMask ^= (1<<index);
		pthread_mutex_unlock(&Mutex);
	}

	NumberofDataProcessors++;
	FILE_LOG(logDEBUG) << "Number of DataProcessors: " << NumberofDataProcessors;

	memset((void*)&timerBegin, 0, sizeof(timespec));
}


DataProcessor::~DataProcessor() {
	if (file) delete file;
	ThreadObject::DestroyThread();
	NumberofDataProcessors--;
}

/** static functions */

uint64_t DataProcessor::GetErrorMask() {
	return ErrorMask;
}

uint64_t DataProcessor::GetRunningMask() {
	return RunningMask;
}

void DataProcessor::ResetRunningMask() {
	RunningMask = 0x0;
}

void DataProcessor::SetSilentMode(bool mode) {
	SilentMode = mode;
}

/** non static functions */
/** getters */
string DataProcessor::GetType(){
	return TypeName;
}

bool DataProcessor::IsRunning() {
	return ((1 << index) & RunningMask);
}

bool DataProcessor::GetAcquisitionStartedFlag(){
	return acquisitionStartedFlag;
}

bool DataProcessor::GetMeasurementStartedFlag(){
	return measurementStartedFlag;
}

uint64_t DataProcessor::GetNumTotalFramesCaught() {
	return numTotalFramesCaught;
}

uint64_t DataProcessor::GetNumFramesCaught() {
	return numFramesCaught;
}

uint64_t DataProcessor::GetActualProcessedAcquisitionIndex() {
	return currentFrameIndex;
}

uint64_t DataProcessor::GetProcessedAcquisitionIndex() {
	return currentFrameIndex - firstAcquisitionIndex;
}

uint64_t DataProcessor::GetProcessedMeasurementIndex() {
	return currentFrameIndex - firstMeasurementIndex;
}



/** setters */
void DataProcessor::StartRunning() {
	pthread_mutex_lock(&Mutex);
	RunningMask |= (1<<index);
	pthread_mutex_unlock(&Mutex);
}


void DataProcessor::StopRunning() {
	pthread_mutex_lock(&Mutex);
	RunningMask ^= (1<<index);
	pthread_mutex_unlock(&Mutex);
}

void DataProcessor::SetFifo(Fifo*& f) {
	fifo = f;
}

void DataProcessor::ResetParametersforNewAcquisition() {
	numTotalFramesCaught = 0;
	firstAcquisitionIndex = 0;
	currentFrameIndex = 0;
	acquisitionStartedFlag = false;
}

void DataProcessor::ResetParametersforNewMeasurement(){
	numFramesCaught = 0;
	firstMeasurementIndex = 0;
	measurementStartedFlag = false;
}


void DataProcessor::RecordFirstIndices(uint64_t fnum) {
	//listen to this fnum, later +1
	currentFrameIndex = fnum;

	measurementStartedFlag = true;
	firstMeasurementIndex = fnum;

	//start of entire acquisition
	if (!acquisitionStartedFlag) {
		acquisitionStartedFlag = true;
		firstAcquisitionIndex = fnum;
	}

#ifdef VERBOSE
	cprintf(BLUE,"%d First Acquisition Index:%lld\tFirst Measurement Index:%lld\n",
			index, (long long int)firstAcquisitionIndex, (long long int)firstMeasurementIndex);
#endif
}


void DataProcessor::SetGeneralData(GeneralData* g) {
	generalData = g;
#ifdef VERY_VERBOSE
	generalData->Print();
#endif
	if (file) {
		file->SetPacketsPerFrame(&generalData->packetsPerFrame);
		file->SetMaxFramesPerFile(generalData->maxFramesPerFile);
		if (file->GetFileType() == HDF5) {
			file->SetNumberofPixels(generalData->nPixelsX, generalData->nPixelsY);
		}
	}
}


int DataProcessor::SetThreadPriority(int priority) {
	struct sched_param param;
	param.sched_priority = priority;
	if (pthread_setschedparam(thread, SCHED_FIFO, &param) == EPERM)
		return FAIL;
	FILE_LOG(logINFO) << "Processor Thread Priority set to " << priority;
	return OK;
}


void DataProcessor::SetFileFormat(const fileFormat f) {
	if (file && file->GetFileType() != f) {
		//remember the pointer values before they are destroyed
		int nd[MAX_DIMENSIONS];nd[0] = 0; nd[1] = 0;
		char* fname=0; char* fpath=0; uint64_t* findex=0; bool* frindexenable=0;
		bool* owenable=0; int* dindex=0; int* nunits=0; uint64_t* nf = 0; uint32_t* dr = 0; uint32_t* port = 0;
		file->GetMemberPointerValues(nd, fname, fpath, findex, frindexenable, owenable, dindex, nunits, nf, dr, port);
		//create file writer with same pointers
		SetupFileWriter(fileWriteEnable, nd, fname, fpath, findex, frindexenable, owenable, dindex, nunits, nf, dr, port);
	}
}


void DataProcessor::SetupFileWriter(bool fwe, int* nd, char* fname, char* fpath, uint64_t* findex,
		bool* frindexenable, bool* owenable, int* dindex, int* nunits, uint64_t* nf, uint32_t* dr, uint32_t* portno,
		GeneralData* g)
{
	fileWriteEnable = fwe;
	if (g)
		generalData = g;
	// fix xcoord as detector is not providing it right now
	xcoord = ((NumberofDataProcessors > (*nunits)) ? index : ((*dindex) * (*nunits)) + index);


	if (file) {
		delete file; file = 0;
	}

	if (fileWriteEnable) {
		switch(*fileFormatType){
#ifdef HDF5C
	case HDF5:
		file = new HDF5File(index, generalData->maxFramesPerFile, &generalData->packetsPerFrame,
				nd, fname, fpath, findex,
				frindexenable, owenable,
				dindex, nunits, nf, dr, portno,
				generalData->nPixelsX, generalData->nPixelsY, &SilentMode);
		break;
#endif
	default:
		file = new BinaryFile(index, generalData->maxFramesPerFile, &generalData->packetsPerFrame,
				nd, fname, fpath, findex,
				frindexenable, owenable,
				dindex, nunits, nf, dr, portno, &SilentMode);
		break;
		}
	}
}

// only the first file
int DataProcessor::CreateNewFile(bool en, uint64_t nf, uint64_t at, uint64_t st, uint64_t ap) {
	if (file == NULL)
		return FAIL;
	file->CloseAllFiles();
	if (file->CreateMasterFile(en,	generalData->imageSize, generalData->nPixelsX, generalData->nPixelsY,
			at, st, ap) == FAIL)
		return FAIL;
	if (file->CreateFile(currentFrameIndex) == FAIL)
		return FAIL;
	return OK;
}


void DataProcessor::CloseFiles() {
	if (file)
		file->CloseAllFiles();
}

void DataProcessor::EndofAcquisition(bool anyPacketsCaught, uint64_t numf) {
	if (file && file->GetFileType() == HDF5) {
		file->EndofAcquisition(anyPacketsCaught, numf);
	}
}


void DataProcessor::ThreadExecution() {
	char* buffer=0;
	fifo->PopAddress(buffer);
#ifdef FIFODEBUG
	if (!index) cprintf(BLUE,"DataProcessor %d, pop 0x%p buffer:%s\n", index,(void*)(buffer),buffer);
#endif

	//check dummy
	uint32_t numBytes = (uint32_t)(*((uint32_t*)buffer));
#ifdef VERBOSE
	if (!index) cprintf(BLUE,"DataProcessor %d, Numbytes:%u\n", index,numBytes);
#endif
	if (numBytes == DUMMY_PACKET_VALUE) {
		StopProcessing(buffer);
		return;
	}

	ProcessAnImage(buffer + FIFO_HEADER_NUMBYTES);

	//stream (if time/freq to stream) or free
	if (*dataStreamEnable && SendToStreamer())
		fifo->PushAddressToStream(buffer);
	else
		fifo->FreeAddress(buffer);
}


void DataProcessor::StopProcessing(char* buf) {
#ifdef VERBOSE
	if (!index)
		cprintf(RED,"DataProcessing %d: Dummy\n", index);
#endif
	//stream or free
	if (*dataStreamEnable)
		fifo->PushAddressToStream(buf);
	else
		fifo->FreeAddress(buf);

	if (file)
		file->CloseCurrentFile();
	StopRunning();
#ifdef VERBOSE
	FILE_LOG(logINFO) << index << ": Processing Completed";
#endif
}

/** buf includes only the standard header */
void DataProcessor::ProcessAnImage(char* buf) {

	sls_detector_header* header = (sls_detector_header*) (buf);
	uint64_t fnum = header->frameNumber;
	currentFrameIndex = fnum;
	uint32_t nump = header->packetNumber;
	if (nump == generalData->packetsPerFrame) {
		numFramesCaught++;
		numTotalFramesCaught++;
	}


#ifdef VERBOSE
	if (!index)
		cprintf(BLUE,"DataProcessing %d: fnum:%lu\n", index, fnum);
#endif

	if (!measurementStartedFlag) {
#ifdef VERBOSE
		if (!index) cprintf(BLUE,"DataProcessing %d: fnum:%lu\n", index, fnum);
#endif
		RecordFirstIndices(fnum);

		if (*dataStreamEnable) {
			//restart timer
			clock_gettime(CLOCK_REALTIME, &timerBegin);
			timerBegin.tv_sec -= (*streamingTimerInMs) / 1000;
			timerBegin.tv_nsec -= ((*streamingTimerInMs) % 1000) * 1000000;

			//to send first image
			currentFreqCount = *streamingFrequency;
		}
	}


	// fix x coord that is currently not provided by detector
	header->xCoord = xcoord;

	if (file)
		file->WriteToFile(buf, generalData->imageSize + sizeof(sls_detector_header), fnum-firstMeasurementIndex, nump);

	if (rawDataReadyCallBack) {
		rawDataReadyCallBack(
				header->frameNumber,
				header->expLength,
				header->packetNumber,
				header->bunchId,
				header->timestamp,
				header->modId,
				header->xCoord,
				header->yCoord,
				header->zCoord,
				header->debug,
				header->roundRNumber,
				header->detType,
				header->version,
				buf + sizeof(sls_detector_header),
				generalData->imageSize,
				pRawDataReady);
	}

}



bool DataProcessor::SendToStreamer() {
	//skip
	if (!(*streamingFrequency)) {
		if (!CheckTimer())
			return false;
	} else {
		if (!CheckCount())
			return false;
	}
	return true;
}


bool DataProcessor::CheckTimer() {
	struct timespec end;
	clock_gettime(CLOCK_REALTIME, &end);
#ifdef VERBOSE
	cprintf(BLUE,"%d Timer elapsed time:%f seconds\n", index, ( end.tv_sec - timerBegin.tv_sec ) + ( end.tv_nsec - timerBegin.tv_nsec ) / 1000000000.0);
#endif
	//still less than streaming timer, keep waiting
	if((( end.tv_sec - timerBegin.tv_sec )	+ ( end.tv_nsec - timerBegin.tv_nsec ) / 1000000000.0) < ((double)*streamingTimerInMs/1000.00))
		return false;

	//restart timer
	clock_gettime(CLOCK_REALTIME, &timerBegin);
	return true;
}


bool DataProcessor::CheckCount() {
	if (currentFreqCount == *streamingFrequency ) {
		currentFreqCount = 1;
		return true;
	}
	currentFreqCount++;
	return false;
}

