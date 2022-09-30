#pragma once

typedef struct tagFrame
{
	float fFrame;
	float fCount;
	float fMax;

	tagFrame() {};

	tagFrame(float _fFrame, float _fCount, float _fMax)
		: fFrame(_fFrame), fCount(_fCount), fMax(_fMax) {}
}FRAME;

typedef struct RecordData
{
	D3DXVECTOR3		pos;
	BYTE			keyState;

	RecordData() {};

	RecordData(D3DXVECTOR3 _pos, BYTE _keyState)
		: pos(_pos), keyState(_keyState) {}
} RECORD_DATA;

/*
	1이면 누르고있는중 0이면 안누르고 있는중
	key == 0x01 / 0000 0001 (상)
	key == 0x02 / 0000 0010 (하)
	key == 0x04 / 0000 0100 (좌)
	key == 0x08 / 0000 1000 (우)
*/

typedef struct LineData
{
	D3DXVECTOR3		startPos;
	D3DXVECTOR3		endPos;

	LineData() {};

	LineData(D3DXVECTOR3 _start, D3DXVECTOR3 _end)
		: startPos(_start), endPos(_end) {}
} LINE_DATA;