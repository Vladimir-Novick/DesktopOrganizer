#pragma once

class Skin
{
public:
	Skin(char *strSkinDll);
	~Skin(void);
private:
	HMODULE m_Skin;
};
