#pragma once
#ifndef __ENGINE_TYPE_DEFINE_H__
#define __ENGINE_TYPE_DEFINE_H__

namespace Engine {
	
	typedef char							My_Byte;
	typedef unsigned char					My_uByte;

	typedef short							My_Short;
	typedef unsigned short					My_uShort;

	typedef int								My_Int;
	typedef unsigned int					My_uInt;

	typedef long							My_Long;
	typedef unsigned long					My_uLong;



	typedef float							My_Float;
	typedef double							My_Double;

	typedef bool							My_Bool;
	typedef wchar_t							My_TChar;
	
	

	typedef D3DXVECTOR2						My_Vec2;
	typedef D3DXVECTOR3						My_Vec3;
	typedef D3DXVECTOR4						My_Vec4;

	typedef D3DXMATRIX						My_Matrix;

}

#endif // !__ENGINE_TYPE_DEFINE_H__
