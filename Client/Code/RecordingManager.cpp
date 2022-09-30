#include "stdafx.h"
#include "..\Header\RecordingManager.h"

IMPLEMENT_SINGLETON(CRecordingManager)

CRecordingManager::CRecordingManager()
{
}

void CRecordingManager::Save()
{
	HANDLE hFile = CreateFile(L"../Bin/Data/Record0.dat", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Record Save Failed!!", L"파일 저장 오류", MB_OK);
		return;
	}

	DWORD writeByte = 0;

	for (auto record : mRecordList)
		WriteFile(hFile, &record, sizeof(RECORD_DATA), &writeByte, nullptr);

	CloseHandle(hFile); 
}

My_uLong CRecordingManager::ReleaseMemory(void)
{
	mRecordList.clear();
	return My_uLong();
}
