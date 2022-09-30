#pragma once

#include "Include.h"
#include "Base.h"

class CRecordingManager
	: public CBase
{
	DECLARE_SINGLETON(CRecordingManager)

private:
	explicit CRecordingManager();
	virtual ~CRecordingManager() = default;

public:
	void Save();

public:
	void SetRecordData(const RECORD_DATA& data) { mRecordList.push_back(data); }

private:
	list<RECORD_DATA>	mRecordList;

protected:
	virtual My_uLong	ReleaseMemory(void);
};

