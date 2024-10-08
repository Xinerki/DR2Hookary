#include <StdInc.h>
#include "cDebugRender.h"

DWORD& FontRoot = *reinterpret_cast<DWORD*>(0xE5FEEC);

void RenderText(float x, float y, tColor color, char* text, float persist_time, bool render_always, bool use_render_index, float scale)
{
	injector::thiscall<void(DWORD, float, float, tColor, char*, float, bool, bool, float)>::call(0xA71900, FontRoot, x, y, color, text, persist_time, render_always, use_render_index, scale);
}