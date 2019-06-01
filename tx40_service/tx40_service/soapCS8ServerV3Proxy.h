/* soapCS8ServerV3Proxy.h
   Generated by gSOAP 2.7.13 from cs8server.h
   Copyright(C) 2000-2009, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#ifndef soapCS8ServerV3Proxy_H
#define soapCS8ServerV3Proxy_H
#include "soapH.h"

class SOAP_CMAC CS8ServerV3Proxy : public soap
{ public:
	/// Endpoint URL of service 'CS8ServerV3Proxy' (change as needed)
	const char *soap_endpoint;
	/// Constructor
	CS8ServerV3Proxy();
	/// Constructor with copy of another engine state
	CS8ServerV3Proxy(const struct soap&);
	/// Constructor with engine input+output mode control
	CS8ServerV3Proxy(soap_mode iomode);
	/// Constructor with engine input and output mode control
	CS8ServerV3Proxy(soap_mode imode, soap_mode omode);
	/// Destructor frees deserialized data
	virtual	~CS8ServerV3Proxy();
	/// Initializer used by constructor
	virtual	void CS8ServerV3Proxy_init(soap_mode imode, soap_mode omode);
	/// Disables and removes SOAP Header from message
	virtual	void soap_noheader();
	/// Put SOAP Header in message
	virtual	void soap_header(int *ns1__sessionId);
	/// Get SOAP Header structure (NULL when absent)
	virtual	const SOAP_ENV__Header *soap_header();
	/// Get SOAP Fault structure (NULL when absent)
	virtual	const SOAP_ENV__Fault *soap_fault();
	/// Get SOAP Fault string (NULL when absent)
	virtual	const char *soap_fault_string();
	/// Get SOAP Fault detail as string (NULL when absent)
	virtual	const char *soap_fault_detail();
	/// Force close connection (normally automatic, except for send_X ops)
	virtual	int soap_close_socket();
	/// Print fault
	virtual	void soap_print_fault(FILE*);
#ifndef WITH_LEAN
	/// Print fault to stream
	virtual	void soap_stream_fault(std::ostream&);
	/// Put fault into buffer
	virtual	char *soap_sprint_fault(char *buf, size_t len);
#endif

	/// Web service operation 'forwardKin' (returns error code or SOAP_OK)
	virtual	int forwardKin(_ns6__forwardKin *ns6__forwardKin, _ns6__forwardKinResponse *ns6__forwardKinResponse);

	/// Web service operation 'reverseKin' (returns error code or SOAP_OK)
	virtual	int reverseKin(_ns6__reverseKin *ns6__reverseKin, _ns6__reverseKinResponse *ns6__reverseKinResponse);

	/// Web service operation 'moveJJ' (returns error code or SOAP_OK)
	virtual	int moveJJ(_ns6__moveJJ *ns6__moveJJ, _ns6__moveResponse *ns6__moveResponse);

	/// Web service operation 'moveJC' (returns error code or SOAP_OK)
	virtual	int moveJC(_ns6__moveJC *ns6__moveJC, _ns6__moveResponse *ns6__moveResponse);

	/// Web service operation 'moveL' (returns error code or SOAP_OK)
	virtual	int moveL(_ns6__moveL *ns6__moveL, _ns6__moveResponse *ns6__moveResponse);

	/// Web service operation 'moveC' (returns error code or SOAP_OK)
	virtual	int moveC(_ns6__moveC *ns6__moveC, _ns6__moveResponse *ns6__moveResponse);

	/// Web service operation 'resetMotion' (returns error code or SOAP_OK)
	virtual	int resetMotion(_ns6__resetMotion *ns6__resetMotion, _ns6__motionResponse *ns6__motionResponse);

	/// Web service operation 'stopMotion' (returns error code or SOAP_OK)
	virtual	int stopMotion(_ns6__stopMotion *ns6__stopMotion, _ns6__motionResponse *ns6__motionResponse);

	/// Web service operation 'restartMotion' (returns error code or SOAP_OK)
	virtual	int restartMotion(_ns6__restartMotion *ns6__restartMotion, _ns6__motionResponse *ns6__motionResponse);

	/// Web service operation 'setSchedulingMode' (returns error code or SOAP_OK)
	virtual	int setSchedulingMode(_ns6__setSchedulingMode *ns6__setSchedulingMode, _ns6__setSchedulingModeResponse *ns6__setSchedulingModeResponse);

	/// Web service operation 'schedulerRefresh' (returns error code or SOAP_OK)
	virtual	int schedulerRefresh(_ns6__schedulerRefresh *ns6__schedulerRefresh, _ns6__MotionAndRobotsPos *ns6__MotionAndRobotsPos);

	/// Web service operation 'setPower' (returns error code or SOAP_OK)
	virtual	int setPower(_ns6__setPower *ns6__setPower, _ns6__setPowerResponse *ns6__setPowerResponse);
};
#endif
