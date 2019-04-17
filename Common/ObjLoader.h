#ifndef CObjNew_H
#define CObjNew_H
#include "../header/Angel.h"
#include "CShape.h"

#define QUAD_NUM 6		// 2 faces, 2 triangles/face 

class ObjLoader : public CShape
{
private:

	//vec4 m_Points[QUAD_NUM];
	//vec3 m_Normal[QUAD_NUM];	// �� vec3 �ӫŧi�O���F�`�٭p��, �p�G�n���{���g�_�ӧ��K�A�i��� vec4 �ӫŧi
	//vec4 m_Colors[QUAD_NUM];

public:
	ObjLoader(char *);

	void Update(float dt, const LightSource &lights);
	void Update(float dt); // ���p��������ө�

	GLuint GetShaderHandle() { return m_uiProgram; }
	void RenderWithFlatShading(const LightSource &lights);//  vLightI: Light Intensity
	void RenderWithGouraudShading(const LightSource &lights);//  vLightI: Light Intensity
	void SetVtxColors(vec4 vLFColor, vec4 vLRColor, vec4 vTRColor, vec4 vTLColor); // four Vertices' Color
	void SetVtxColors(vec4 vFColor, vec4 vSColor);	// three Vertices' Color with idx as the first 

	void Draw();
	void DrawW();

	//	Ū��
	char lineHeader[1000];//�@��Ū�ɼȦs�Ŷ�
	int indices[10];
	float vertex[3];
	FILE* objfile;
	char* adrFile;
};




#endif