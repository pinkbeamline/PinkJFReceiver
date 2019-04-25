#include "slsDetectorUsers.h"
#include "detectorData.h"
#include "multiSlsDetector.h"
#include "multiSlsDetectorCommand.h"


slsDetectorUsers::slsDetectorUsers(int id) : myDetector(NULL){

  myDetector=new multiSlsDetector(id);
  myCmd=new multiSlsDetectorCommand(myDetector);
};


slsDetectorUsers::~slsDetectorUsers() {
  if (myDetector)
    delete myDetector;
};



string slsDetectorUsers::getDetectorDeveloper(){
  return myDetector->getDetectorDeveloper();
}



int slsDetectorUsers::setOnline(int const online){
  return myDetector->setOnline(online);
}

int slsDetectorUsers::setReceiverOnline(int const online){
  return myDetector->setReceiverOnline(online);
}

void slsDetectorUsers::startMeasurement(){
  myDetector->acquire(0); 
}

int slsDetectorUsers::stopMeasurement(){
  return myDetector->stopAcquisition();
}
 

int slsDetectorUsers::getDetectorStatus(){
  return (int)myDetector->getRunStatus();
}

string slsDetectorUsers::getFilePath(){
  return myDetector->getFilePath();
}


string slsDetectorUsers::setFilePath(string s){
  return myDetector->setFilePath(s);
}  


string slsDetectorUsers::getFileName(){
  return myDetector->getFileName();
}  

string slsDetectorUsers::setFileName(string s){
  return myDetector->setFileName(s);
}  
  
int slsDetectorUsers::getFileIndex(){
  return (int)myDetector->getFileIndex();
}
  
int slsDetectorUsers::setFileIndex(int i){
  return (int)myDetector->setFileIndex(i);
}

string slsDetectorUsers::getFlatFieldCorrectionDir(){
  return myDetector->getFlatFieldCorrectionDir();
} 

string slsDetectorUsers::setFlatFieldCorrectionDir(string dir){
  return myDetector->setFlatFieldCorrectionDir(dir);
}
 
string slsDetectorUsers::getFlatFieldCorrectionFile(){
  return myDetector->getFlatFieldCorrectionFile();
};
  
int slsDetectorUsers::setFlatFieldCorrectionFile(string fname){
  return myDetector->setFlatFieldCorrectionFile(fname);
} 

int slsDetectorUsers::enableFlatFieldCorrection(int i){
  return myDetector->enableFlatFieldCorrection(i);
}

int slsDetectorUsers::enableCountRateCorrection(int i){  
  return myDetector->enableCountRateCorrection(i);
}

int slsDetectorUsers::enablePixelMaskCorrection(int i){
  return myDetector->enablePixelMaskCorrection(i);
}
int slsDetectorUsers::enableAngularConversion(int i){
  return myDetector->enableAngularConversion(i);
}
int slsDetectorUsers::enableWriteToFile(int i){
  return myDetector->enableWriteToFile(i);
}

int slsDetectorUsers::setPositions(int nPos, double *pos){
  return myDetector->setPositions(nPos, pos);
}

int slsDetectorUsers::getPositions(double *pos){
  return myDetector->getPositions(pos);
}

int slsDetectorUsers::setDetectorSize(int x0, int y0, int nx, int ny){
  if(myDetector->getTotalNumberOfChannels(slsDetectorDefs::Y)>1)
	return 1;
  int nmod=nx/(myDetector->getChansPerMod(0));
  cout << myDetector->getChansPerMod(0) << " " << nx << " " << nmod << endl;
  return myDetector->setNumberOfModules(nmod)*myDetector->getChansPerMod(0);}

int slsDetectorUsers::getDetectorSize(int &x0, int &y0, int &nx, int &ny){ 
  y0=0; 
  x0=0; 
  nx=myDetector->getTotalNumberOfChannels(slsDetectorDefs::X);
  ny=myDetector->getTotalNumberOfChannels(slsDetectorDefs::Y);
  return nx*ny;
}

int slsDetectorUsers::getMaximumDetectorSize(int &nx, int &ny){
  nx=myDetector->getMaxNumberOfChannelsPerDetector(slsDetectorDefs::X);
  ny=myDetector->getMaxNumberOfChannelsPerDetector(slsDetectorDefs::Y);
  return nx*ny;
}

int slsDetectorUsers::setBitDepth(int i){
  return myDetector->setDynamicRange(i);
}


int slsDetectorUsers::setSettings(int isettings){
  return myDetector->slsDetectorBase::setSettings(isettings);
}

int slsDetectorUsers::getThresholdEnergy(){
  return myDetector->getThresholdEnergy(-1);
}  

int slsDetectorUsers::setThresholdEnergy(int e_eV){
  return myDetector->setThresholdEnergy(e_eV);
}

double slsDetectorUsers::setExposureTime(double t, bool inseconds, int imod){
  if(!inseconds)
    return myDetector->setExposureTime((int64_t)t,imod);
  else {
	  // + 0.5 to round for precision lost from converting double to int64_t
	  int64_t tms = (int64_t)(t * (1E+9) + 0.5);
	  if (t < 0) tms = -1;
	  tms = myDetector->setExposureTime(tms,imod);
	  if (tms < 0)
		  return -1;
	  return  ((1E-9) * (double)tms);
  }
}

double slsDetectorUsers::setExposurePeriod(double t, bool inseconds, int imod){
  if(!inseconds)
    return myDetector->setExposurePeriod((int64_t)t,imod);
  else {
	  // + 0.5 to round for precision lost from converting double to int64_t
	  int64_t tms = (int64_t)(t * (1E+9) + 0.5);
	  if (t < 0) tms = -1;
	  tms = myDetector->setExposurePeriod(tms,imod);
	  if (tms < 0)
		  return -1;
	  return  ((1E-9) * (double)tms);
  }
}

double slsDetectorUsers::setDelayAfterTrigger(double t, bool inseconds, int imod){
  if(!inseconds)
    return myDetector->setDelayAfterTrigger((int64_t)t,imod);
  else {
	  // + 0.5 to round for precision lost from converting double to int64_t
	  int64_t tms = (int64_t)(t * (1E+9) + 0.5);
	  if (t < 0) tms = -1;
	  tms = myDetector->setDelayAfterTrigger(tms,imod);
	  if (tms < 0)
		  return -1;
	  return  ((1E-9) * (double)tms);
  }
}

int64_t slsDetectorUsers::setNumberOfGates(int64_t t, int imod){
  return myDetector->setNumberOfGates(t,imod);
} 

int64_t slsDetectorUsers::setNumberOfFrames(int64_t t, int imod){
  return myDetector->setNumberOfFrames(t,imod);
}

int64_t slsDetectorUsers::setNumberOfCycles(int64_t t, int imod){
  return myDetector->setNumberOfCycles(t,imod);
}
  
int slsDetectorUsers::setTimingMode(int pol){
  return myDetector->setTimingMode(pol);
}

int slsDetectorUsers::readConfigurationFile(string const fname){
  return myDetector->readConfigurationFile(fname);
}  

int slsDetectorUsers::dumpDetectorSetup(string const fname){
  return myDetector->dumpDetectorSetup(fname);
} 

int slsDetectorUsers::retrieveDetectorSetup(string const fname){
  return myDetector->retrieveDetectorSetup(fname);
}

string slsDetectorUsers::getDetectorType(){
  return myDetector->sgetDetectorsType();
}

void slsDetectorUsers::initDataset(int refresh){
  myDetector->initDataset(refresh);
}

void slsDetectorUsers::addFrame(double *data, double pos, double i0, double t, string fname, double var){
  myDetector->addFrame(data,pos,i0,t,fname,var);
}


void slsDetectorUsers::finalizeDataset(double *a, double *v, double *e, int &np){
  myDetector->finalizeDataset(a, v, e, np);
}

int slsDetectorUsers::setReceiverMode(int n){
	return myDetector->setReadReceiverFrequency(n);
}

int slsDetectorUsers::enableDataStreamingFromReceiver(int i){
	return myDetector->enableDataStreamingFromReceiver(i);
}

int slsDetectorUsers::enableDataStreamingToClient(int i){
	return myDetector->enableDataStreamingToClient(i);
}

int slsDetectorUsers::setReceiverDataStreamingOutPort(int i){
	return myDetector->setReceiverDataStreamingOutPort(i);
}

int slsDetectorUsers::setClientDataStreamingInPort(int i){
	return myDetector->setClientDataStreamingInPort(i);
}

int64_t slsDetectorUsers::getModuleFirmwareVersion(){
	return myDetector->getModuleFirmwareVersion();
}

int64_t slsDetectorUsers::getModuleSerialNumber(int imod){
	return myDetector->getModuleSerialNumber(imod);
}

int64_t slsDetectorUsers::getDetectorFirmwareVersion(){
	return myDetector->getDetectorFirmwareVersion();
}

int64_t slsDetectorUsers::getDetectorSerialNumber(){
	return myDetector->getDetectorSerialNumber();
}

int64_t slsDetectorUsers::getDetectorSoftwareVersion(){
	return myDetector->getDetectorSoftwareVersion();
}

int64_t slsDetectorUsers::getThisSoftwareVersion(){
	return myDetector->getThisSoftwareVersion();
}




void slsDetectorUsers::registerDataCallback(int( *userCallback)(detectorData*, int, int, void*), void *pArg){
  myDetector->registerDataCallback(userCallback,pArg);
}

void slsDetectorUsers::registerRawDataCallback(int( *userCallback)(double*, int, void*), void *pArg){
  myDetector->registerRawDataCallback(userCallback,pArg);
}

void slsDetectorUsers::registerAcquisitionFinishedCallback(int( *func)(double,int, void*), void *pArg){
  myDetector->registerAcquisitionFinishedCallback(func,pArg);
}

void slsDetectorUsers::registerGetPositionCallback( double (*func)(void*),void *arg){
  myDetector->registerGetPositionCallback(func,arg);
}

void slsDetectorUsers::registerConnectChannelsCallback( int (*func)(void*),void *arg){
  myDetector->registerConnectChannelsCallback(func,arg);
}

void slsDetectorUsers::registerDisconnectChannelsCallback( int (*func)(void*),void *arg){
  myDetector->registerDisconnectChannelsCallback(func,arg);
}  

void slsDetectorUsers::registerGoToPositionCallback( int (*func)(double,void*),void *arg){
  myDetector->registerGoToPositionCallback(func,arg);
}

void slsDetectorUsers::registerGoToPositionNoWaitCallback( int (*func)(double,void*),void *arg){
  myDetector->registerGoToPositionNoWaitCallback(func,arg);
}

void slsDetectorUsers::registerGetI0Callback( double (*func)(int,void*),void *arg){
  myDetector->registerGetI0Callback(func,arg);
}




string slsDetectorUsers::putCommand(int narg, char *args[], int pos){
	if(narg < 2)
		return string("Error: Insufficient Parameters");
	return myCmd->putCommand(narg, args, pos);
}

string slsDetectorUsers::getCommand(int narg, char *args[], int pos){
	if(narg < 1)
		return string("Error: Insufficient Parameters");
	return myCmd->getCommand(narg, args, pos);
}




int slsDetectorUsers::setClockDivider(int value) {
	return myDetector->setClockDivider(value);
}

int slsDetectorUsers::setParallelMode(int value) {
	if(value >= 0)
		myDetector->setParallelMode(value);
	return myDetector->getParallelMode();
}

int slsDetectorUsers::setAllTrimbits(int val, int id) {
	return myDetector->setAllTrimbits(val, id);
}

int slsDetectorUsers::setDAC(string dac, int val, int id) {
	int dacindex = myDetector->getDACIndex(dac);
	if(dacindex == -1) return -9999;
	return myDetector->setDACValue(val, dacindex, id);
}

int slsDetectorUsers::getADC(string adc, int id) {
	int adcindex = myDetector->getADCIndex(adc);
	if(adcindex == -1) return -9999;
	return myDetector->getADCValue(adcindex, id);
}

int slsDetectorUsers::startReceiver() {
	return myDetector->startReceiver();
}

int slsDetectorUsers::stopReceiver() {
	return myDetector->stopReceiver();
}

int slsDetectorUsers::startAcquisition() {
	return myDetector->startAcquisition();
}

int slsDetectorUsers::stopAcquisition() {
	return myDetector->stopAcquisition();
}


int slsDetectorUsers::setReceiverSilentMode(int i) {
	return myDetector->setReceiverSilentMode(i);
}

int slsDetectorUsers::setHighVoltage(int i) {
	return myDetector->setHighVoltage(i);
}
