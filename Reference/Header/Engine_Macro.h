#pragma once
#ifndef __ENGINE_MACRO_H__
#define __ENGINE_MACRO_H__

#if true
/* Base Macro for exception message box macros
   예외처리 메세지박스 매크로 생성을 위한 기초 매크로들 */

#define MSG_BOX(CONTENT_TEXT)		\
MessageBox(NULL,TEXT(CONTENT_TEXT),TEXT("System Message"),MB_OK)

#endif

#endif // !__ENGINE_MACRO_H__