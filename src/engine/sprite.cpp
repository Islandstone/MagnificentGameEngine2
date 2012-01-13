#include "base.h"
#include "sprite.h"
#include <IL\ilut.h>
#include <QString>
#include "qt_gl_interface.h"
#include "input.h"
#include "timer.h"
#include "engine.h"

#ifdef _WIN32
#include <gl/GL.h>
#include <gl/GLU.h>
#endif 

#include "script.h"

void BindClass_CSprite()
{
	Sqrat::Class<CSprite> def;

	def.Func(L"LoadTexture", &CSprite::LoadTexture);
	def.Func(L"Spawn", &CSprite::Spawn);
	def.Func(L"Think", &CSprite::Think);
	def.Func(L"Render", &CSprite::Render);
	def.Func(L"SetPos", &CSprite::SetPos );
	def.Var(L"x", &CSprite::m_flPosX );
	def.Var(L"y", &CSprite::m_flPosY );
	def.Var(L"rot", &CSprite::m_flRotation );
	def.Var(L"scale", &CSprite::m_flScale );

	Sqrat::RootTable().Bind(L"Sprite", def);
}

ClassBinder spriteBinder(&BindClass_CSprite);

/*
BEGIN_SCRIPTBIND( CSprite, Sprite )
	BIND_FUNC( LoadTexture, CSprite::LoadTexture )
	BIND_FUNC( Think, CSprite::Think )
	BIND_FUNC( SetPos, CSprite::SetPos )
END_SCRIPTBIND()
*/

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
	//LoadTexture(L"logo.bmp");
}

void CSprite::Precache()
{
	//LoadTexture(L"logo.bmp");
}

bool CSprite::LoadTexture(wchar_t* filename)
{
	Engine()->PushContext();

	// TODO: Move this into a separate class

	if ( !glIsEnabled( GL_TEXTURE_RECTANGLE_NV ) ) 
	{
		m_iTexture = ilutGLLoadImage( filename );

		if (ilGetError() != IL_NO_ERROR)
			return false;

		m_iWidth = ilGetInteger( IL_IMAGE_WIDTH );
		m_iHeight = ilGetInteger( IL_IMAGE_HEIGHT );

	}
	else
	{
		ILuint texid;

		ilGenImages(1, &texid);
		ilBindImage(texid);
		ilLoadImage( filename );

		ILenum Error = ilGetError();

		if ( Error != IL_NO_ERROR )
		{
			sqstd_printcallstack( Sqrat::DefaultVM::Get() );
			std::wstringstream st; st << L"DevIL Error: " << iluErrorString(Error) << std::endl;
			Engine()->Debug( st.str() );
			return false;
		}

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

	//Engine()->PopContext();
	return true;
}

void CSprite::Think()
{
}


// TODO: Need to fix this
#define SCALE (30.0f/512.0f)

void CSprite::Render()
{
	//Engine()->PushContext();

	glPushMatrix();
	glLoadIdentity();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glTranslatef(m_flPosX, m_flPosY, 0);

	glTranslatef( 0.5f*(float)(m_iWidth)*SCALE , 0.5f*(float)(m_iHeight)*SCALE, 0);
	glScalef(m_flScale, m_flScale, 0.0f);
	glRotatef(m_flRotation, 0.0f, 0.0f, 1.0f);
	glTranslatef( -0.5f*(float)(m_iWidth)*SCALE , -0.5f*(float)(m_iHeight)*SCALE, 0);


	if ( !glIsEnabled( GL_TEXTURE_RECTANGLE_NV ) ) 
	{
		glBindTexture(GL_TEXTURE_2D, m_iTexture);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(0.0, 0.0, 1.0);

			glTexCoord2f(1.0, 1.0);
			glVertex3f(30, 0, 1.0);

			glTexCoord2f(1.0, 0.0);
			glVertex3f(30, 30.0, 1.0);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(0.0, 30.0, 1.0);
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
