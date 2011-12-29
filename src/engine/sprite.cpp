#include "base.h"
#include "sprite.h"
#include <IL\ilut.h>
#include <QString>
#include "qt_gl_interface.h"
#include "input.h"
#include "timer.h"

#ifdef _WIN32
#include <gl/GL.h>
#include <gl/GLU.h>
#endif 

#define GL_TEXTURE_RECTANGLE_NV 0x84F5
#define GL_TEXTURE_RECTANGLE_EXT GL_TEXTURE_RECTANGLE_NV

CSprite::CSprite() : CBaseObject()
{
	m_flPosX = 0.0f;
	m_flPosY = 0.0f;
	m_flRotation = 0.0f;
	m_flScale = 1.0f;

	m_iWidth = 0;
	m_iHeight = 0;

	m_flLastThinkTime = 0.0f;
}

void CSprite::Spawn()
{
	SetPos(0, 0);
}

void CSprite::Precache()
{
	LoadTexture(L"logo.bmp");
}

bool CSprite::LoadTexture(const wchar_t* filename)
{
	// TODO: Move this into a separate class

	if ( !glIsEnabled( GL_TEXTURE_RECTANGLE_NV ) ) 
	{
		m_iTexture = 0;//ilutGLLoadImage( const_cast<wchar_t*>( filename.c_str() ) );
	}
	else
	{
		ILuint texid;

		ilGenImages(1, &texid);
		ilBindImage(texid);
		ilLoadImage( filename );

		if (ilGetError() != IL_NO_ERROR)
			return false;

		m_iWidth = ilGetInteger( IL_IMAGE_WIDTH );
		m_iHeight = ilGetInteger( IL_IMAGE_HEIGHT );

		ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		glGenTextures(1, &m_iTexture);
		glBindTexture(GL_TEXTURE_RECTANGLE_NV, m_iTexture);
		glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, ilGetInteger(IL_IMAGE_BPP), m_iWidth,
		 m_iHeight, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		 ilGetData());
	}

	return true;
}

void CSprite::Think()
{
	float curtime = Timer()->CurrentTime();

	float dt = curtime - m_flLastThinkTime;

	if (dt < 0.01f)
		return;
	
	if ( Input()->KeyHeld( VK_Right ) || Input()->KeyHeld( VK_L ) )
		m_flPosX += 200.0f * dt;
	if ( Input()->KeyHeld( VK_Left ) || Input()->KeyHeld( VK_H ) )
		m_flPosX -= 200.0f * dt;

	if ( Input()->KeyHeld( VK_Up ) || Input()->KeyHeld( VK_K ) )
		m_flPosY -= 200.0f * dt;
	if ( Input()->KeyHeld( VK_Down ) || Input()->KeyHeld( VK_J ) )
		m_flPosY += 200.0f * dt;


	if ( Input()->KeyHeld( VK_PageUp ) ) 
	{
		m_flScale += 1.5f * dt;
	}
	if ( Input()->KeyHeld( VK_PageDown ) ) 
	{
		m_flScale -= 1.5f * dt;
	}

	if ( Input()->KeyHeld( VK_Q ) )
    {
        m_flRotation -= 72.0f * dt;
    }
    if ( Input()->KeyHeld( VK_W ) )
    {
        m_flRotation += 72.0f * dt;
    }

    if ( m_flRotation >= 360.0f )
    {
        m_flRotation -= 360.0f;
    }
    else if ( m_flRotation < 0.0f )
    {
        m_flRotation += 360.0f;
    }

	if ( m_flScale < 0.1f )
		m_flScale = 0.1f;
	else if ( m_flScale > 3.0f )
		m_flScale = 3.0f;

	m_flLastThinkTime = Timer()->CurrentTime();
}

#define SCALE (30.0f/512.0f)

void CSprite::Render()
{
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(m_flPosX, m_flPosY, 0);

	glTranslatef( 0.5f*(float)(m_iWidth)*SCALE , 0.5f*(float)(m_iHeight)*SCALE, 0);
	glScalef(m_flScale, m_flScale, 0.0f);
	glRotatef(m_flRotation, 0.0f, 0.0f, 1.0f);
	glTranslatef( -0.5f*(float)(m_iWidth)*SCALE , -0.5f*(float)(m_iHeight)*SCALE, 0);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	if ( !glIsEnabled( GL_TEXTURE_RECTANGLE_NV ) ) 
	{
		glBindTexture(GL_TEXTURE_2D, m_iTexture);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(0.0, m_iHeight, 1.0);

			glTexCoord2f(1.0, 0.0);
			glVertex3f(m_iWidth, m_iHeight, 1.0);

			glTexCoord2f(1.0, 1.0);
			glVertex3f(m_iWidth, 0.0, 1.0);

			glTexCoord2f(0.0, 1.0);
			glVertex3f(0.0, 0.0, 1.0);
		glEnd();
	}
	else
	{
		glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_iTexture );

		glBegin( GL_QUADS );
			glTexCoord2i(0, m_iHeight);                           
			glVertex3i(0, 0, 1);
			glTexCoord2i(m_iWidth, m_iHeight);     
			glVertex3i(30, 0, 1);
			glTexCoord2i(m_iWidth, 0 );    
			glVertex3i(30, 30, 1);
			glTexCoord2i( 0, 0 );          
			glVertex3i(0, 30, 0);
		glEnd();
	}

	glPopMatrix();
}
