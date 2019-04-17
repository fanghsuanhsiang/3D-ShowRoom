#ifndef CSHAPE_H
#define CSHAPE_H

#define  _CRT_SECURE_NO_WARNINGS 1
#include "../Header/Angel.h"
#include "../SOIL/SOIL.h"
#include "TypeDefine.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

#define FLAT_SHADING    0
#define GOURAUD_SHADING 1

// GPU ���p�⥲���ǧ�h���Ѽƶi�J Shader

//#define PERVERTEX_LIGHTING
// �]�w�W�������ѴN�O�Ұ� PERPIXEL_LIGHTING

// �H�U���}�Ҧh�h���K��
//#define MULTITEXTURE  NONE_MAP
//#define MULTITEXTURE  DIFFUSE_MAP  
//#define MULTITEXTURE  (DIFFUSE_MAP|LIGHT_MAP)  // For Example3LM
#define MULTITEXTURE  (DIFFUSE_MAP|LIGHT_MAP|NORMAL_MAP)// For Example 4 ~~

// �}�ҥH�U���w�q�N��ϥ� Environment  Mapping
// �W���}�� #define MULTITEXTURE  DIFFUSE_MAP  
// �ϥ� Environment Mapping �����]�w�ϥ�  vsCubeMapping.glsl �P fsCubeMapping.glsl
// �ثe�èS���b Cubic Mapping ���ǤJ NORMAL_MAP �P LIGHT_MAP
 #define CUBIC_MAP 1		// �o�ӥu�Φb Example 5

// ��ҫ������� non-uniform scale ���ާ@�ɡA�����z�L�p��ϯx�}�ӱo�쥿�T�� Normal ��V
// �}�ҥH�U���w�q�Y�i�A�ثe CPU �p�⪺������
// GPU �������h�O�]�w������

// #define GENERAL_CASE 1 

class CShape 
{
protected:
	vec4 *m_pPoints;
	vec3 *m_pNormals;
	vec4 *m_pColors;
	vec2 *m_pTex;
	vec2 *m_pTex1;

#if MULTITEXTURE >= LIGHT_MAP
	vec2 *m_pTex2;	// �s�W�ĤG�i�K�� for example 3
#endif
#if MULTITEXTURE >= NORMAL_MAP
	vec2 *m_pTex3;		// �s�W�ĤT�i�K�� for example 4
	vec3 *m_pTangentV;	// �s�W tangent vector for each vertex
#endif

	int  m_iNumVtx;

	GLfloat m_fColor[4]; // Object's color
	// For shaders' name
	char *m_pVXshader, *m_pFSshader;

	// For VAO
	GLuint m_uiVao;

	// For Shader
	GLuint  m_uiModelView, m_uiProjection, m_uiColor;
	GLuint  m_uiProgram;
	GLuint  m_uiBuffer;

	point4  m_vLightInView;	 // �����b�@�ɮy�Ъ���m
	GLuint  m_uiLightInView;	 // �����b shader ����m
	GLuint  m_uiAmbient;		 // light's ambient  �P Object's ambient  �P ka �����n
	GLuint  m_uiDiffuse;		 // light's diffuse  �P Object's diffuse  �P kd �����n
	GLuint  m_uiSpecular;	 // light's specular �P Object's specular �P ks �����n
	GLuint  m_uiShininess;
	GLuint  m_uiLighting;	// �������Ӽ�
	GLuint  m_uiTexLayer;	// �K�Ϫ��h���A�w�]�N�O�@�h diffuse

#ifdef CUBIC_MAP
	GLuint  m_uiTRS;			// TRSMatrix �ǤJ Pixel Shader ����m
	GLuint  m_uiViewPos;		// ViewPoint �ǤJ Pixel Shader ����m
	point4  m_v4Eye;			// Camera ����m

	GLuint  m_uiCubeMap;     // pixel shader ���� CubeMap Texture Name ����m
	GLuint  m_uiCubeMapTexName;  // �ǤJ pixel shader ���� Cube map ���K�Ͻs���A 
#endif

	LightSource m_Light1;

	color4 m_AmbientProduct;
	color4 m_DiffuseProduct;
	color4 m_SpecularProduct;
	int    m_iLighting;		// �]�w�O�_�n���O
	int    m_iTexLayer;		// �]�w�K�Ϫ��h���A0 ��ܨS���K��

	// For Matrices
	mat4    m_mxView, m_mxProjection, m_mxTRS;
	mat4    m_mxMVFinal;
	mat3    m_mxMV3X3Final, m_mxITMV;		// �ϥΦb�p�� �������᪺�s Normal
	mat3	m_mxITView;
	bool    m_bProjUpdated, m_bViewUpdated, m_bTRSUpdated;

	// For materials
	Material m_Material;

	// For Shading Mode
	int		m_iMode;	 // 0: Flat shading, 1: Gouraud shading, 0 for default

	void	CreateBufferObject();
	void	CreateBufferObject_2D_Cut();
	void	DrawingSetShader();
	void	DrawingSetShader_2D_Cut();
	void	DrawingWithoutSetShader();

public:
	CShape();
	virtual ~CShape();
	virtual void Draw() = 0;
	virtual void DrawW() = 0; // Drawing without setting shaders
	virtual void Update(float dt, const LightSource &lights) = 0;
	virtual void Update(float dt) = 0; // ���p��������ө�

	void SetShaderName(const char vxShader[], const char fsShader[]);
	void SetShader(GLuint uiShaderHandle = MAX_UNSIGNED_INT);
	void SetShader_2D_Cut(GLuint uiShaderHandle = MAX_UNSIGNED_INT);
	void SetColor(vec4 vColor);
	void SetViewMatrix(mat4 &mat);
	void SetProjectionMatrix(mat4 &mat);
	void SetTRSMatrix(mat4 &mat);
	void SetTextureLayer(int texlayer);

	// For setting materials 
	void SetMaterials(color4 ambient, color4 diffuse, color4 specular);
	void SetKaKdKsShini(float ka, float kd, float ks, float shininess); // ka kd ks shininess

	// For Lighting Calculation
	void SetShadingMode(int iMode) {m_iMode = iMode;}
	vec4 PhongReflectionModel(vec4 vPoint, vec3 vNormal, const LightSource &lights);
	vec4 PhongReflectionModel(vec4 vPoint, vec3 vNormal, vec4 vLightPos, color4 vLightI);

	// For controlling texture mapping
	void SetMirror(bool uAxis, bool vAxis); // U�b �P V�b �O�_�n��g
	void SetTiling(float uTiling, float vTiling);  // �� U�b �P V�b �i����K���Y��

	void SetLightingDisable() {m_iLighting = 0;}

	// For Cube Map
#ifdef CUBIC_MAP
	void SetCubeMapTexName(GLuint uiTexName) {
		m_uiCubeMapTexName = uiTexName;
	}
	void SetViewPosition(point4 vEye) {
			m_v4Eye = vEye;
	}
#endif
};

#endif
