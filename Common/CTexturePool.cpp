#include "CTexturePool.h"
#include "../png_loader.h"

CTexturePool* CTexturePool::_pTexturePool = nullptr;

void CTexturePool::destroyInstance()
{
	if (_iNumTextures != 0) {
		for (int i = 0; i < _iNumTextures; i++)  delete _Pool[i].TextureName;
	}
	CC_SAFE_DELETE(_pTexturePool);
}


CTexturePool::CTexturePool() {
	_iNumTextures = 0;
	for (int i = 0; i < TEXTUREPOOL_MAX; i++) {
		_Pool[i].TextureName = NULL;
		_Pool[i].uiTextureID = -1;
		_Pool[i].iTexHeight = _Pool[i].iTexWidth = 0;
	}
}

CTexturePool::~CTexturePool()
{

}

CTexturePool* CTexturePool::create()
{
	if (_pTexturePool == nullptr) {
		_pTexturePool = new (std::nothrow) CTexturePool;
	}
	return _pTexturePool;
}

CTexturePool* CTexturePool::getInstance()
{
	if (_pTexturePool == nullptr){
		_pTexturePool = new (std::nothrow) CTexturePool;
	}
	return _pTexturePool;
}

GLuint CTexturePool::AddTexture(char *texName)
{
	// ���N texName �ƻs�@���� _pool ��
	int len = strlen(texName);
	_Pool[_iNumTextures].TextureName = new char[len+1];
	memcpy(_Pool[_iNumTextures].TextureName, texName, len);
	_Pool[_iNumTextures].TextureName[len] = '\0';
	_Pool[_iNumTextures].uiTextureID = png_load_SOIL(texName, &(_Pool[_iNumTextures].iTexWidth), &(_Pool[_iNumTextures].iTexHeight), true);	
	_iNumTextures++;
	return(_Pool[_iNumTextures-1].uiTextureID);
}