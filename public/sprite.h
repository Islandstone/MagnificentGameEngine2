#ifndef _SPRITE_HEADER
#define _SPRITE_HEADER

#include "base_object.h"
#include "IL\il.h"
//#include "qt_gl_interface.h"


class CSprite : public CBaseObject
{
public:
	CSprite();

	void Spawn();
	void Precache();

	inline void SetPos(float x, float y) { m_flPosX = x; m_flPosY = y; }
	int GetWidth() { return m_iWidth; }
	int GetHeight() { return m_iHeight; }

	bool LoadTexture(const wchar_t* texture_filename);

	void Think();
	void Render();

	bool ShouldRender() { return true; }
	bool ShouldThink() { return true; }

protected:

private:
	// TODO: Vector class
	float m_flPosX;
	float m_flPosY;
	float m_flRotation;
	float m_flScale;

	float m_flLastThinkTime;

	int m_iWidth;
	int m_iHeight;

	ILuint m_iTexture;
};


#endif