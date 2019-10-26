#include "StdAfx.h"
#include ".\skin.h"



Skin::Skin(char *strSkinDll)
{
	char sCurrentFileName[255];
	GetCurrentDirectory(255,sCurrentFileName);
	strcat("\\",sCurrentFileName);
	strcat(strSkinDll,sCurrentFileName);
	strcat(".ski",sCurrentFileName);
	m_Skin = LoadLibrary(sCurrentFileName);
}

Skin::~Skin(void)
{
}
