#pragma once
/************************************************
 * @file zmqSocket.h
 * @short functions to open/close zmq sockets
 ***********************************************/
/**
 *@short functions to open/close zmq sockets
 */

#include "ansi.h"

#include <iostream>
#include <zmq.h>
#include <errno.h>
#include <netdb.h>				//gethostbyname()
#include <arpa/inet.h>			//inet_ntoa
#include <rapidjson/document.h> //json header in zmq stream
using namespace rapidjson;


//#define ZMQ_DETAIL

class ZmqSocket {

public:

	//Socket Options for optimization
	//ZMQ_LINGER default is already -1 means no messages discarded. use this options if optimizing required
	//ZMQ_SNDHWM default is 0 means no limit. use this to optimize if optimizing required
	// eg. int value = -1;
	// if (zmq_setsockopt(socketDescriptor, ZMQ_LINGER, &value,sizeof(value))) {
	//	Close();
	// }

	/**
	 * Constructor for a client
	 * Creates socket, context and connects to server
	 * @param hostname hostname or ip of server
	 * @param portnumber port number
	 */
	ZmqSocket (const char* const hostname_or_ip, const uint32_t  portnumber):
		portno (portnumber),
		server (false),
		contextDescriptor (NULL),
		socketDescriptor (NULL)
	{
		char ip[MAX_STR_LENGTH] = "";
		memset(ip, 0, MAX_STR_LENGTH);

		// convert hostname to ip (not required, but a test that returns if failed)
		struct addrinfo *result;
		if ((ConvertHostnameToInternetAddress(hostname_or_ip, &result)) ||
		 (ConvertInternetAddresstoIpString(result, ip, MAX_STR_LENGTH)))
			return;

		// construct address
		sprintf (serverAddress, "tcp://%s:%d", ip, portno);

		// create context
		contextDescriptor = zmq_ctx_new();
		if (contextDescriptor == NULL)
			return;

		// create publisher
		socketDescriptor = zmq_socket (contextDescriptor, ZMQ_SUB);
		if (socketDescriptor == NULL) {
			PrintError ();
			Close ();
		}

		//Socket Options provided above
	    // an empty string implies receiving any messages
		if ( zmq_setsockopt(socketDescriptor, ZMQ_SUBSCRIBE, "", 0)) {
			PrintError ();
			Close();
		}
		//ZMQ_LINGER default is already -1 means no messages discarded. use this options if optimizing required
		//ZMQ_SNDHWM default is 0 means no limit. use this to optimize if optimizing required
		// eg. int value = -1;
		int value = 0;
		 if (zmq_setsockopt(socketDescriptor, ZMQ_LINGER, &value,sizeof(value))) {
			PrintError ();
			Close();
		 }
	};

	/**
	 * Constructor for a server
	 * Creates socket, context and connects to server
	 * @param hostname hostname or ip of server
	 * @param portnumber port number
	 */
	ZmqSocket (const uint32_t portnumber):
		portno (portnumber),
		server (true),
		contextDescriptor (NULL),
		socketDescriptor (NULL)
	{
		// create context
		contextDescriptor = zmq_ctx_new();
		if (contextDescriptor == NULL)
			return;
		// create publisher
		socketDescriptor = zmq_socket (contextDescriptor, ZMQ_PUB);
		if (socketDescriptor == NULL) {
			PrintError ();
			Close ();
		}

		//Socket Options provided above

		// construct address
		sprintf (serverAddress,"tcp://*:%d", portno);
		// bind address
		if (zmq_bind (socketDescriptor, serverAddress) < 0) {
			PrintError ();
			Close ();
		}

		//sleep for a few milliseconds to allow a slow-joiner
		usleep(200* 1000);
	};

	/**
	 * Destructor
	 */
	~ZmqSocket () {
		Disconnect();
		Close();
	};

	/**
	 * Returns error status
	 * @returns true if error else false
	 */
	bool IsError() { if (socketDescriptor == NULL) return true; return false; };

	/**
	 * Returns Server Address
	 * @returns Server Address
	 */
	char* GetZmqServerAddress () { return serverAddress; };

	/**
	 * Returns Port Number
	 * @returns Port Number
	 */
	uint32_t GetPortNumber () { return portno; };

	/**
	 * Returns Socket Descriptor
	 * @reutns Socket descriptor
	 */

	void* GetsocketDescriptor () { return socketDescriptor; };

	/**
	 * Connect client socket to server socket
	 * @returns 1 for fail, 0 for success
	 */
	int Connect() {
		if (zmq_connect(socketDescriptor, serverAddress) < 0) {
			PrintError ();
			Close ();
			return 1;
		}
		return 0;
	}


	/**
	 * Unbinds the Socket
	 */
	void Disconnect () {
		if (server)
			zmq_unbind (socketDescriptor, serverAddress);
		else
			zmq_disconnect (socketDescriptor, serverAddress);
	};

	/**
	 * Close Socket and destroy Context
	 */
	void Close () {
		if (socketDescriptor != NULL) {
			zmq_close (socketDescriptor);
			socketDescriptor = NULL;
		}

		if (contextDescriptor != NULL) {
			zmq_ctx_destroy (contextDescriptor);
			contextDescriptor = NULL;
		}
	};


 	/**
 	 * Convert Hostname to Internet address info structure
 	 * One must use freeaddrinfo(res) after using it
 	 * @param hostname hostname
 	 * @param res address of pointer to address info structure
 	 * @return 1 for fail, 0 for success
 	 */
     // Do not make this static (for multi threading environment)
     int ConvertHostnameToInternetAddress (const char* const hostname, struct addrinfo **res) {
    	// criteria in selecting socket address structures returned by res
 		struct addrinfo hints;
 		memset (&hints, 0, sizeof (hints));
 		 hints.ai_family = AF_INET;
 		 hints.ai_socktype = SOCK_STREAM;
 		 // get host info into res
 		 int errcode = getaddrinfo (hostname, NULL, &hints, res);
 		 if (errcode != 0) {
 			 cprintf (RED,"Error: Could not convert %s hostname to internet address (zmq):"
 					 "%s\n", hostname, gai_strerror(errcode));
 		 } else {
 			 if (*res == NULL) {
 				 cprintf (RED,"Error: Could not convert %s hostname to internet address (zmq): "
 						 "gettaddrinfo returned null\n", hostname);
 			 } else{
 				 return 0;
 			 }
 		 }
 		 cprintf(RED, "Error: Could not convert hostname to internet address\n");
 		 return 1;
 	};

     /**
      * Convert Internet Address structure pointer to ip string (char*)
      * Clears the internet address structure as well
      * @param res pointer to internet address structure
      * @param ip pointer to char array to store result in
      * @param ipsize size available in ip buffer
      * @return 1 for fail, 0 for success
      */
     // Do not make this static (for multi threading environment)
     int ConvertInternetAddresstoIpString (struct addrinfo *res, char* ip, const int ipsize) {
    	 if (inet_ntop (res->ai_family, &((struct sockaddr_in *) res->ai_addr)->sin_addr, ip, ipsize) != NULL) {
    		 freeaddrinfo(res);
    		 return 0;
    	 }
    	 cprintf(RED, "Error: Could not convert internet address to ip string\n");
    	 return 1;
     }



	/**
	 * Send Message Header
	 * @param buf message
	 * @param length length of message
	 * @param dummy true if end of acquistion else false
	 * @returns 0 if error, else 1
	 */
	int SendHeaderData ( int index, bool dummy, uint32_t jsonversion, uint32_t dynamicrange = 0, uint64_t fileIndex = 0,
			uint32_t npixelsx = 0, uint32_t npixelsy = 0,
			uint64_t acqIndex = 0, uint64_t fIndex = 0, char* fname = NULL,
			uint64_t frameNumber = 0, uint32_t expLength = 0, uint32_t packetNumber = 0,
			uint64_t bunchId = 0, uint64_t timestamp = 0,
			uint16_t modId = 0, uint16_t xCoord = 0, uint16_t yCoord = 0, uint16_t zCoord = 0,
			uint32_t debug = 0, uint16_t roundRNumber = 0,
			uint8_t detType = 0, uint8_t version = 0) {


		char buf[MAX_STR_LENGTH] = "";
		/** Json Header Format */
		const char* jsonHeaderFormat =
				"{"
				"\"jsonversion\":%u, "
				"\"bitmode\":%u, "
				"\"fileIndex\":%llu, "
				"\"shape\":[%u, %u], "
				"\"acqIndex\":%llu, "
				"\"fIndex\":%llu, "
				"\"fname\":\"%s\", "
				"\"data\": %d, "

				"\"frameNumber\":%llu, "
				"\"expLength\":%u, "
				"\"packetNumber\":%u, "
				"\"bunchId\":%llu, "
				"\"timestamp\":%llu, "
				"\"modId\":%u, "
				"\"xCoord\":%u, "
				"\"yCoord\":%u, "
				"\"zCoord\":%u, "
				"\"debug\":%u, "
				"\"roundRNumber\":%u, "
				"\"detType\":%u, "
				"\"version\":%u"
				"}\n\0";
		int length = sprintf(buf, jsonHeaderFormat,
				jsonversion, dynamicrange, fileIndex, npixelsx, npixelsy,
				acqIndex, fIndex, (fname == NULL)? "":fname, dummy?0:1,
						frameNumber, expLength, packetNumber, bunchId, timestamp,
						modId, xCoord, yCoord, zCoord, debug, roundRNumber,
						detType, version);
#ifdef VERBOSE
		//if(!index)
			FILE_LOG(logINFO) << index << ": Streamer: buf:" << buf;
#endif

		if(zmq_send (socketDescriptor, buf, length, dummy?0:ZMQ_SNDMORE) < 0) {
			PrintError ();
			return 0;
		}
#ifdef VERBOSE
		cprintf(GREEN,"[%u] send header data\n",portno);
#endif
		return 1;
	};

	/**
	 * Send Message Body
	 * @param buf message
	 * @param length length of message
	 * @returns 0 if error, else 1
	 */
	int SendData (char* buf, int length) {
		if(zmq_send (socketDescriptor, buf, length, 0) < 0) {
			PrintError ();
			return 0;
		}
#ifdef VERBOSE
		cprintf(GREEN,"[%u] send data\n",portno);
#endif
		return 1;
	};


	/**
	 * Receive Message
	 * @param index self index for debugging
	 * @param message message
	 * @returns length of message, -1 if error
	 */
	int ReceiveMessage(const int index, zmq_msg_t& message) {
		int length = zmq_msg_recv (&message, socketDescriptor, 0);
		if (length == -1) {
			PrintError ();
			cprintf (BG_RED,"Error: Could not read header for socket %d\n",index);
		}
#ifdef VERBOSE
		else
			cprintf( RED,"Message %d Length: %d Header:%s \n", index, length, (char*) zmq_msg_data (&message) );
#endif
		return length;
	};


	/**
	 * Receive Header
	 * @param index self index for debugging
	 * @param acqIndex address of acquisition index
	 * @param frameIndex address of frame index
	 * @param subframeIndex address of subframe index
	 * @param filename address of file name
	 * @returns 0 if error or end of acquisition, else 1
	 */
	int ReceiveHeader(const int index, uint64_t &acqIndex,
			uint64_t &frameIndex, uint32_t &subframeIndex, string &filename)
	{
		zmq_msg_t message;
		zmq_msg_init (&message);
		int len = ReceiveMessage(index, message);
		if ( len > 0 ) {
			bool dummy = false;
#ifdef ZMQ_DETAIL
				cprintf( BLUE,"Header %d [%d] Length: %d Header:%s \n", index, portno, len, (char*) zmq_msg_data (&message) );
#endif
			if ( ParseHeader (index, len, message, acqIndex, frameIndex, subframeIndex, filename, dummy)) {
#ifdef ZMQ_DETAIL
				cprintf( RED,"Parsed Header %d [%d] Length: %d Header:%s \n", index, portno, len, (char*) zmq_msg_data (&message) );
#endif
				zmq_msg_close (&message);
				if (dummy) {
#ifdef ZMQ_DETAIL
					cprintf(RED,"%d [%d] Received end of acquisition\n", index, portno );
#endif
					return 0;
				}
#ifdef ZMQ_DETAIL
				cprintf(GREEN,"%d [%d] data\n",index, portno );
#endif
				return 1;
			}
		}
		zmq_msg_close(&message);
		return 0;
	};

	/**
	 * Receive Data
	 * @param index self index for debugging
	 * @param buf buffer to copy image data to
	 * @param size size of image
	 * @returns length of data received
	 */
	int ReceiveData(const int index, int* buf, const int size)
	{
		zmq_msg_t message;
		zmq_msg_init (&message);
		int length = ReceiveMessage(index, message);

		//actual data
		if (length == size) {
#ifdef VERBOSE
			cprintf(BLUE,"%d actual data\n", index);
#endif
			memcpy((char*)buf, (char*)zmq_msg_data(&message), size);
		}

		//incorrect size
		else {
			cprintf(RED,"Error: Received weird packet size %d for socket %d\n", length, index);
			memset((char*)buf,0xFF,size);
		}

		zmq_msg_close(&message);
		return length;
	};


	/**
	 * Parse Header
	 * @param index self index for debugging
	 * @param length length of message
	 * @param message message
	 * @param acqIndex address of acquisition index
	 * @param frameIndex address of frame index
	 * @param subframeIndex address of subframe index
	 * @param filename address of file name
	 * @param dummy true if end of acquisition else false
	 * @returns true if successfull else false
	 */
	int ParseHeader(const int index, int length, zmq_msg_t& message, uint64_t &acqIndex,
			uint64_t &frameIndex, uint32_t &subframeIndex, string &filename, bool& dummy)
	{

		acqIndex = -1;
		frameIndex = -1;
		subframeIndex = -1;
		dummy = true;

		Document d;
		if ( d.Parse( (char*) zmq_msg_data (&message), zmq_msg_size (&message)).HasParseError() ) {
			cprintf( RED,"%d Could not parse. len:%d: Message:%s \n", index, length, (char*) zmq_msg_data (&message) );
			fflush ( stdout );
			char* buf =  (char*) zmq_msg_data (&message);
			for ( int i= 0; i < length; ++i ) {
				cprintf(RED,"%02x ",buf[i]);
			}
			printf("\n");
			fflush( stdout );
			return 0;
		}

		int temp = d["data"].GetUint();
		dummy = temp ? false : true;
		if (!dummy) {
			acqIndex 		= d["acqIndex"].GetUint64();
			frameIndex 		= d["fIndex"].GetUint64();
			if(d["bitmode"].GetUint()==32 && d["detType"].GetUint() == slsReceiverDefs::EIGER) {
				subframeIndex 	= d["expLength"].GetUint();
			}
			filename 		= d["fname"].GetString();
		}
#ifdef VERYVERBOSE
		cprintf(BLUE,"%d Dummy:%d\n"
				"\tAcqIndex:%lu\n"
				"\tFrameIndex:%lu\n"
				"\tSubIndex:%u\n"
				"\tBitMode:%u\n"
				"\tDetType:%u\n",
				index, (int)dummy, acqIndex, frameIndex, subframeIndex,
				d["bitmode"].GetUint(),d["detType"].GetUint());
#endif
		return 1;
	};


	/**
	 * Print error
	 */
	void PrintError () {
		switch (errno) {
		case EINVAL:
			cprintf(RED, "Error: The socket type/option or value/endpoint supplied is invalid (zmq)\n");
			break;
		case EAGAIN:
			cprintf(RED, "Error: Non-blocking mode was requested and the message cannot be sent/available at the moment (zmq)\n");
			break;
		case ENOTSUP:
			cprintf(RED, "Error: The zmq_send()/zmq_msg_recv() operation is not supported by this socket type (zmq)\n");
			break;
		case EFSM:
			cprintf(RED, "Error: The zmq_send()/zmq_msg_recv() unavailable now as socket in inappropriate state (eg. ZMQ_REP). Look up messaging patterns (zmq)\n");
			break;
		case EFAULT:
			cprintf(RED, "Error: The provided context/message is invalid (zmq)\n");
			break;
		case EMFILE:
			cprintf(RED, "Error: The limit on the total number of open ØMQ sockets has been reached (zmq)\n");
			break;
		case EPROTONOSUPPORT:
			cprintf(RED, "Error: The requested transport protocol is not supported (zmq)\n");
			break;
		case ENOCOMPATPROTO:
			cprintf(RED, "Error: The requested transport protocol is not compatible with the socket type (zmq)\n");
			break;
		case EADDRINUSE:
			cprintf(RED, "Error: The requested address is already in use (zmq)\n");
			break;
		case EADDRNOTAVAIL:
			cprintf(RED, "Error: The requested address was not local (zmq)\n");
			break;
		case ENODEV:
			cprintf(RED, "Error: The requested address specifies a nonexistent interface (zmq)\n");
			break;
		case ETERM:
			cprintf(RED, "Error: The ØMQ context associated with the specified socket was terminated (zmq)\n");
			break;
		case ENOTSOCK:
			cprintf(RED, "Error: The provided socket was invalid (zmq)\n");
			break;
		case EINTR:
			cprintf(RED, "Error: The operation was interrupted by delivery of a signal (zmq)\n");
			break;
		case EMTHREAD:
			cprintf(RED, "Error: No I/O thread is available to accomplish the task (zmq)\n");
			break;
		default:
			cprintf(RED, "Error: Unknown socket error (zmq)\n");
			break;
		}
	};


private:
	/** Port Number */
	uint32_t portno;

	/** true if server, else false */
	bool server;

	/** Context Descriptor */
	void* contextDescriptor;

	/** Socket Descriptor */
	void* socketDescriptor;

	/** Server Address */
	char serverAddress[1000];

};
