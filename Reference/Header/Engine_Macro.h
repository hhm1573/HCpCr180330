#pragma once
#ifndef __ENGINE_MACRO_H__
#define __ENGINE_MACRO_H__

#if true
/* Base Macro for exception message box macros
   ����ó�� �޼����ڽ� ��ũ�� ������ ���� ���� ��ũ�ε� */

#define MSG_BOX(CONTENT_TEXT)		\
MessageBox(NULL,TEXT(CONTENT_TEXT),TEXT("System Message"),MB_OK)

#endif

#endif // !__ENGINE_MACRO_H__