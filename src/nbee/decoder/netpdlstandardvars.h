/*****************************************************************************/
/*                                                                           */
/* Copyright notice: please read file license.txt in the NetBee root folder. */
/*                                                                           */
/*****************************************************************************/


/*!
	\file netpdlstandardvars.h

	This file defines the class devoted to the management of the standard variables of the NetPDL engine.
*/


#pragma once

#include <nbee_packetdecoderutils.h>


//! Maximum number of variables handled by the current NetPDL implementation
#define NETPDL_MAX_NVARS 40
//! Maximum number of char allowed when defining a variable name
#define NETPDL_MAX_VARNAME 30




/*!
	\brief This class is devoted to the management of the standard variables of the NetPDL engine

	The NetPDL specification defines a set of run-time variables. Moreover, the user can make use
	of its own variables for other purposes.
	This class keeps trace of all the run-time variables of the NetPDL engine.
*/
class CNetPDLStandardVars : public nbPacketDecoderVars
{
public:
	CNetPDLStandardVars(char* ErrBuf, int ErrBufSize);
	virtual ~CNetPDLStandardVars();

	//! Structure that contains the internal ID of the default variables.
	//! This is not the most efficient way to do so, but it works nicely.
	struct _defaultVarsList
	{
		long long LinkType;
		long long FrameLength;
		long long PacketLength;
		long long TimestampSec;
		long long TimestampUSec;
		long long CurrentOffset;
		long long CurrentProtoOffset;
		long long PacketBuffer;
		long long PrevProto;
		long long NextProto;
		long long ShowNetworkNames;
		long long ProtoVerifyResult;
		long long TokenBeginTokenLen;
		long long TokenFieldLen;
		long long TokenEndTokenLen;
	} m_defaultVarList;

	// All these functions are documented in base class
	int CreateVariable(struct _nbNetPDLElementVariable* Variable);
	int GetVariableID(const char* Name, long long* VariableID);

	void SetVariableNumber(long long VariableID, unsigned long long Value);
	void SetVariableBuffer(long long VariableID, unsigned char* Value, long long StartingOffset, long long Size);
	void SetVariableRefBuffer(long long VariableID, unsigned char* PtrValue, long long StartingOffset, long long Size);

	void GetVariableNumber(long long VariableID, unsigned long long* ReturnValue);
	void GetVariableBuffer(long long VariableID, unsigned char** ReturnBufferPtr, unsigned long long* ReturnBufferSize);
	int GetVariableBuffer(long long VariableID, long long StartAt, long long Size, unsigned char** ReturnBufferPtr, unsigned long long* ReturnBufferSize);

	long long SetVariableNumber(char* Name, unsigned long long Value);
	int SetVariableBuffer(char* Name, unsigned char* Value, unsigned long long StartingOffset, unsigned long long Size);
	int SetVariableRefBuffer(char* Name, unsigned char* Value, unsigned long long StartingOffset, unsigned long long Size);

	int GetVariableNumber(char* Name, unsigned long long* ReturnValue);
	int GetVariableBuffer(char* Name, unsigned char** ReturnBufferPtr, unsigned long long* ReturnBufferSize);

	void DoGarbageCollection(int TimestampSec);

	char *GetLastError() { return m_errbuf; };

private:
	//! Keeps the numeric run-time variables declared in the NetPDL engine.
	struct _variableList
	{
		//! Type of the variable. It can assume values listed in the same element of struct _nbNetPDLElementVariable (in the 'nbprotodb' module).
		int Type;
		//! Name of the variable.
		char Name[NETPDL_MAX_VARNAME];
		//! A code that defines the validity of the packet. It can assume the values allowed in the same element of struct _nbNetPDLElementVariable (in the 'nbprotodb' module).
		int Validity;
		//! Numeric value associated to this variable. It is meaningful only for numeric variables.
		long ValueNumber;
		//! Buffer associated to this variable. It is meaningful only for 'buffer' and 'refbuffer' variables.
		//! In case of 'buffer' variable, this buffer is allocated of size 'SizeBuffer'; otherwise, it is simply a pointer.
		unsigned char *ValueBuffer;
		//! Size of the data contained in previous buffer. It is meaningful only for 'buffer' and 'refbuffer' variables.
		long long Size;
		//! Size of the previous buffer. It is meaningful only for 'buffer' variables.
		//! This value is different from 'Size' because this is the entire size of the buffer, while 'Size' contains
		//! only the number of data actually in it.
		long long SizeBuffer;

		//! Initialization value (if any). It is valid only in case of a nbNETPDL_VARIABLE_TYPE_NUMBER.
		int InitValueNumber;
		//! Initialization value (if any). It is valid only in case of a nbNETPDL_VARIABLE_TYPE_BUFFER.
		unsigned char *InitValueString;
		//! Size of previous buffer (if any). It is valid only in case of a nbNETPDL_VARIABLE_TYPE_BUFFER.
		int InitValueStringSize;
	} m_variableList [NETPDL_MAX_NVARS];

	//! Keeps the number of variables actually stored in m_variableList.
	int m_currNumVariables;

	//! Pointer to the buffer that will keep the error message (if any); this buffer belongs to the class that creates this one.
	char* m_errbuf;

	//! Size of the buffer that will keep the error message (if any); this buffer belongs to the class that creates this one.
	int m_errbufSize;
};

