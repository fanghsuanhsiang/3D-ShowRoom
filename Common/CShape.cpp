#include "CShape.h"

CShape::CShape()
{
	m_bTRSUpdated = m_bViewUpdated = m_bProjUpdated = false;

	// ambient �w�]�� 0, diffuse, specular ���C�ⳣ�O�Ǧ� 0.5
	// Ka = 0 �Y�� , kd = 0.8 , ks = 0.2
	m_Material.ambient = vec4(vec3(0));
	m_Material.diffuse = vec4(vec3(0.5f));
	m_Material.specular = vec4(vec3(0.5f));
	m_Material.ka = 0; m_Material.kd = 0.8f; m_Material.ks = 0.2f;
	m_Material.shininess = 2.0f;

	m_iMode = FLAT_SHADING;
	// �w�]�� RGBA �� (0.5,0.5,0.5,1.0) , �ѳo�ӦǶ��C��ӥN�������C��
	m_fColor[0] = 0.5f; m_fColor[1] = 0.5f; m_fColor[2] = 0.5f; m_fColor[3] = 1.0f;

	m_iLighting = 1;		// �w�]�����O�����ө�
	m_iTexLayer = 1;		// �w�]���@�i Diffuse �K��

	m_pPoints = nullptr; 	m_pNormals = nullptr; 	m_pColors = nullptr; 	m_pTex1 = nullptr;
#if MULTITEXTURE >= LIGHT_MAP
	m_pTex2 = nullptr;	
#endif
#if MULTITEXTURE >= NORMAL_MAP
	m_pTex3 = nullptr;
	m_pTangentV = nullptr;
#endif
	 
}

CShape::~CShape()
{
	if( m_pPoints  != NULL ) delete [] m_pPoints;  
	if( m_pNormals != NULL ) delete	[] m_pNormals;
	if( m_pColors  != NULL ) delete	[] m_pColors;
	if (m_pTex1 != NULL) delete m_pTex1;
#if MULTITEXTURE >= LIGHT_MAP
	if (m_pTex2 != NULL) delete m_pTex2;		// Example 3
#endif
#if MULTITEXTURE  >= NORMAL_MAP
	if (m_pTex3 != NULL) delete m_pTex3;		// Example 4
	if (m_pTangentV != NULL) delete[] m_pTangentV;
#endif

	if( m_pVXshader != NULL ) delete [] m_pVXshader;
	if( m_pFSshader != NULL ) delete [] m_pFSshader;
}

void CShape::CreateBufferObject()
{
    glGenVertexArrays( 1, &m_uiVao );
    glBindVertexArray( m_uiVao );

    // Create and initialize a buffer object
    glGenBuffers( 1, &m_uiBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );

#if MULTITEXTURE < DIFFUSE_MAP
	glBufferData(GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4))*m_iNumVtx, NULL, GL_STATIC_DRAW);
#elif MULTITEXTURE <= DIFFUSE_MAP
    glBufferData( GL_ARRAY_BUFFER, (sizeof(vec4) + sizeof(vec3) + sizeof(vec4) + sizeof(vec2))*m_iNumVtx, NULL, GL_STATIC_DRAW );
	// (sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx + sizeof(vec4)*m_iNumVtx <- vertices, normal and color
	// (sizeof(vec4) + sizeof(vec3) + sizeof(vec4) + sizeof(vec2))*m_iNumVtx <- vertices, normal, color and texture coordinates
#elif MULTITEXTURE <= (LIGHT_MAP | DIFFUSE_MAP)
	glBufferData( GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4)+sizeof(vec2)*2)*m_iNumVtx, NULL, GL_STATIC_DRAW); // �]�t��i�K��
#elif MULTITEXTURE <= (LIGHT_MAP|DIFFUSE_MAP|NORMAL_MAP)
	glBufferData(GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4)+sizeof(vec2)* 3 + sizeof(vec3))*m_iNumVtx, NULL, GL_STATIC_DRAW); // �]�t�T�i�K�ϻP tangent
#endif

	// vertices
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec4)*m_iNumVtx, m_pPoints );  
	// vertices' normal
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx, sizeof(vec3)*m_iNumVtx, m_pNormals );
	// vertcies' Color
	glBufferSubData( GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3))*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors ); 
#if  MULTITEXTURE >= DIFFUSE_MAP
	// Diffuse Map's Texture Coordinates
	glBufferSubData( GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4))*m_iNumVtx, sizeof(vec2)*m_iNumVtx, m_pTex1 );  //�Ĥ@�i�K��
#endif
#if MULTITEXTURE >= LIGHT_MAP
	// Light Map's Texture Coordinates
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4)+sizeof(vec2))*m_iNumVtx, sizeof(vec2)*m_iNumVtx, m_pTex2); // �ĤG�i�K��
#endif
#if MULTITEXTURE >= NORMAL_MAP
	// Normal Map's Texture Coordinates
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4)+sizeof(vec2)*2)*m_iNumVtx, sizeof(vec2)*m_iNumVtx, m_pTex3); // �ĤT�i�K��
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4)+sizeof(vec2)*3)*m_iNumVtx, sizeof(vec3)*m_iNumVtx, m_pTangentV); // �ĤT�i�K��
	// Vertex's Tangent
#endif
}

void CShape::CreateBufferObject_2D_Cut() {
	//For UI
	glGenVertexArrays(1, &m_uiVao);
	glBindVertexArray(m_uiVao);

	// Create and initialize a buffer object
	glGenBuffers(1, &m_uiBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4)*m_iNumVtx, m_pPoints);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx, sizeof(vec3)*m_iNumVtx, m_pNormals);
}


void CShape::SetShader(GLuint uiShaderHandle)
{
	// ���m�o��, ��K�C�@�Ӫ��󪺳]�w
	CreateBufferObject();

    // Load shaders and use the resulting shader program
	if( uiShaderHandle == MAX_UNSIGNED_INT) m_uiProgram = InitShader( m_pVXshader, m_pFSshader );
	else m_uiProgram = uiShaderHandle;

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( m_uiProgram, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    GLuint vNormal = glGetAttribLocation( m_uiProgram, "vNormal" ); 
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*m_iNumVtx) );

    GLuint vColorVtx = glGetAttribLocation( m_uiProgram, "vVtxColor" );  // vertices' color 
    glEnableVertexAttribArray( vColorVtx );
    glVertexAttribPointer( vColorVtx, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(vec4)+sizeof(vec3))*m_iNumVtx));

	GLuint vDifMapCoord = glGetAttribLocation( m_uiProgram, "vDiffuseMapCoord" );  // vertices' texture coordinates
	glEnableVertexAttribArray( vDifMapCoord );
	glVertexAttribPointer( vDifMapCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(vec4)+sizeof(vec3)+sizeof(vec4))*m_iNumVtx));
	glUniform1i(glGetUniformLocation(m_uiProgram, "diffuMap"), 0);

#if MULTITEXTURE >= LIGHT_MAP
	// For Light Map
	GLuint vLightMapCoord = glGetAttribLocation(m_uiProgram, "vLightMapCoord");  // Light maps' texture coordinates�A �����s�W�� Shader ��
	glEnableVertexAttribArray(vLightMapCoord);
	glVertexAttribPointer(vLightMapCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(vec4)+sizeof(vec3)+sizeof(vec4)+sizeof(vec2))*m_iNumVtx));
	glUniform1i(glGetUniformLocation(m_uiProgram, "lightMap"), 1);
#endif
#if MULTITEXTURE >= NORMAL_MAP
	// For Normal Map
	GLuint vNormalMapCoord = glGetAttribLocation(m_uiProgram, "vNormalMapCoord");  // Light maps' texture coordinates�A �����s�W�� Shader ��
	glEnableVertexAttribArray(vNormalMapCoord);
	glVertexAttribPointer(vNormalMapCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(vec4)+sizeof(vec3)+sizeof(vec4)+sizeof(vec2)*2)*m_iNumVtx));
	glUniform1i(glGetUniformLocation(m_uiProgram, "normalMap"), 2);

	GLuint vTangentVec = glGetAttribLocation(m_uiProgram, "vTangentV");  // vertices' color 
	glEnableVertexAttribArray(vTangentVec);
	glVertexAttribPointer(vTangentVec, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(vec4)+sizeof(vec3)+sizeof(vec4)+sizeof(vec2)* 3)*m_iNumVtx));

#endif
	m_uiModelView = glGetUniformLocation( m_uiProgram, "ModelView" );
	glUniformMatrix4fv( m_uiModelView, 1, GL_TRUE, m_mxMVFinal );

    m_uiProjection = glGetUniformLocation( m_uiProgram, "Projection" );
	glUniformMatrix4fv( m_uiProjection, 1, GL_TRUE, m_mxProjection );

    m_uiColor = glGetUniformLocation( m_uiProgram, "vObjectColor" );
	glUniform4fv(m_uiColor, 1, m_fColor ); 

	m_uiLightInView = glGetUniformLocation( m_uiProgram, "LightInView" );
	glUniform4fv(m_uiLightInView, 1, m_vLightInView); 

	m_uiAmbient = glGetUniformLocation( m_uiProgram, "AmbientProduct" );
	glUniform4fv(m_uiAmbient, 1, m_AmbientProduct); 

	m_uiDiffuse = glGetUniformLocation( m_uiProgram, "DiffuseProduct" );
	glUniform4fv(m_uiDiffuse, 1, m_DiffuseProduct); 

	m_uiSpecular = glGetUniformLocation( m_uiProgram, "SpecularProduct" );
	glUniform4fv(m_uiSpecular, 1, m_SpecularProduct); 

	m_uiShininess = glGetUniformLocation( m_uiProgram, "fShininess" );
	glUniform1f(m_uiShininess, m_Material.shininess); 

	m_uiLighting = glGetUniformLocation( m_uiProgram, "iLighting" );
	glUniform1i(m_uiLighting, m_iLighting); // �������Ӽ�

	m_uiTexLayer = glGetUniformLocation(m_uiProgram, "iTexLayer");
	glUniform1i(m_uiTexLayer, m_iTexLayer);	// �K�Ϫ��ӼơA�w�]�� 1�A�����ǤJ pixel shader

#ifdef CUBIC_MAP
	// For Cube Map
	m_uiCubeMap = glGetUniformLocation(m_uiProgram, "cubeMap");
	glUniform1i(m_uiCubeMap, m_uiCubeMapTexName);

	m_uiViewPos = glGetUniformLocation(m_uiProgram, "ViewPos");
	glUniform4fv(m_uiViewPos, 1, m_v4Eye);

	m_uiTRS = glGetUniformLocation(m_uiProgram, "TRSMatrix");
	glUniformMatrix4fv(m_uiTRS, 1, GL_TRUE, m_mxTRS);
#endif

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void CShape::SetShader_2D_Cut(GLuint uiShaderHandle) {

	CreateBufferObject_2D_Cut();
	// Load shaders and use the resulting shader program
	if (uiShaderHandle == MAX_UNSIGNED_INT) m_uiProgram = InitShader(m_pVXshader, m_pFSshader);
	else m_uiProgram = uiShaderHandle;

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(m_uiProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(m_uiProgram, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*m_iNumVtx));

	m_uiModelView = glGetUniformLocation(m_uiProgram, "ModelView");
	//	m_mxMVFinal , m_mxView �ŧi�ɴN�O���x�}
	glUniformMatrix4fv(m_uiModelView, 1, GL_TRUE, m_mxView);

	m_uiProjection = glGetUniformLocation(m_uiProgram, "Projection");
	// m_mxProjection �ŧi�ɴN�O���x�}
	glUniformMatrix4fv(m_uiProjection, 1, GL_TRUE, m_mxProjection);

	m_uiColor = glGetUniformLocation(m_uiProgram, "vColor");
	glUniform4fv(m_uiColor, 1, m_fColor);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CShape::DrawingSetShader()
{
	glUseProgram( m_uiProgram );
	glBindVertexArray( m_uiVao );
	glUniformMatrix4fv( m_uiModelView, 1, GL_TRUE, m_mxMVFinal );

	if( m_bProjUpdated ) {
		glUniformMatrix4fv( m_uiProjection, 1, GL_TRUE, m_mxProjection );
		m_bProjUpdated = false;
	}

	// �N��T��s�� Shader ��
	glUniform4fv(m_uiColor, 1, m_fColor ); 
	glUniform4fv(m_uiLightInView, 1, m_vLightInView); 
	glUniform4fv(m_uiAmbient, 1, m_AmbientProduct); 
	glUniform4fv(m_uiDiffuse, 1, m_DiffuseProduct); 
	glUniform4fv(m_uiSpecular, 1, m_SpecularProduct); 
	glUniform1f(m_uiShininess, m_Material.shininess); 
	glUniform1i(m_uiLighting, m_iLighting);
	glUniform1i(m_uiTexLayer, m_iTexLayer);

#ifdef CUBIC_MAP
	glUniform4fv(m_uiViewPos, 1, m_v4Eye);
	glUniformMatrix4fv(m_uiTRS, 1, GL_TRUE, m_mxTRS);
#endif
}

void CShape::DrawingSetShader_2D_Cut() {
	glUseProgram(m_uiProgram);
	glBindVertexArray(m_uiVao);

	if (m_bViewUpdated || m_bTRSUpdated) { // Model View �������x�}���e�����
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	glUniformMatrix4fv(m_uiModelView, 1, GL_TRUE, m_mxMVFinal);

	if (m_bProjUpdated) {
		glUniformMatrix4fv(m_uiProjection, 1, GL_TRUE, m_mxProjection);
		m_bProjUpdated = false;
	}
	glUniform4fv(m_uiColor, 1, m_fColor);
}

// ���B�w�]�ϥΫe�@�Ӵyø����ҨϥΪ� Shader
void CShape::DrawingWithoutSetShader()
{
	glBindVertexArray( m_uiVao );
	glUniformMatrix4fv( m_uiModelView, 1, GL_TRUE, m_mxMVFinal );

	if( m_bProjUpdated ) {
		glUniformMatrix4fv( m_uiProjection, 1, GL_TRUE, m_mxProjection );
		m_bProjUpdated = false;
	}

	glUniform4fv(m_uiColor, 1, m_fColor ); 
	glUniform4fv(m_uiLightInView, 1, m_vLightInView); 
	glUniform4fv(m_uiAmbient,  1, m_AmbientProduct); 
	glUniform4fv(m_uiDiffuse,  1, m_DiffuseProduct); 
	glUniform4fv(m_uiSpecular, 1, m_SpecularProduct); 
	glUniform1f(m_uiShininess, m_Material.shininess); 
	glUniform1i(m_uiLighting, m_iLighting); 
	glUniform1i(m_uiTexLayer, m_iTexLayer);

#ifdef CUBIC_MAP
	glUniform4fv(m_uiViewPos, 1, m_v4Eye);
	glUniformMatrix4fv(m_uiTRS, 1, GL_TRUE, m_mxTRS);
#endif
}

void CShape::SetShaderName(const char vxShader[], const char fsShader[])
{
	int len;
	len = strlen(vxShader);
	m_pVXshader = new char[len+1];
	memcpy(m_pVXshader, vxShader, len+1);

	len = strlen(fsShader);
	m_pFSshader = new char[len+1];
	memcpy(m_pFSshader, fsShader, len+1);
}

void CShape::SetViewMatrix(mat4 &mat)
{
	m_mxView = mat;
	m_bViewUpdated = true;
}

void CShape::SetTRSMatrix(mat4 &mat)
{
	m_mxTRS = mat;
	m_bTRSUpdated = true;
}

void CShape::SetProjectionMatrix(mat4 &mat)
{
	m_mxProjection = mat;
	m_bProjUpdated = true;
}

void CShape::SetColor(vec4 vColor)
{
	m_fColor[0] = vColor.x;
	m_fColor[1] = vColor.y;
	m_fColor[2] = vColor.z;
	m_fColor[3] = vColor.w;
//	glUniform4fv(m_uiColor, 1, m_fColor); 
}

void CShape::SetTextureLayer(int texlayer)
{
	m_iTexLayer = texlayer;
}

void CShape::SetMaterials(color4 ambient, color4 diffuse, color4 specular)
{
	m_Material.ambient =  ambient;
	m_Material.diffuse =  diffuse;
	m_Material.specular = specular;
}

void CShape::SetKaKdKsShini(float ka, float kd, float ks, float shininess) // ka kd ks shininess
{
	m_Material.ka = ka;
	m_Material.kd = kd;
	m_Material.ks = ks;
	m_Material.shininess = shininess;
}

// Phong Reflection Model
vec4 CShape::PhongReflectionModel(vec4 vPoint, vec3 vNormal, const LightSource &lights)
{
	vec3 vN;		// �� vec3 �ӫŧi�O���F�`�٭p��, �p�G�n���{���g�_�ӧ��K�A�i��� vec4 �ӫŧi
	vec4 vColor;	// �ӳ��I���C��
	vec4 vPosInView, vLightInView;

	// 1. �N�I��������Y�y�Шt�ΡA�]�����I�|�ھ� TRS Matrix ���ʡA�ҥH�����H m_mxMVFinal �p��b�@�ɮy�Ъ������m
	vPosInView = m_mxMVFinal * vPoint;

	// 2. �N���� Normal ������Y�y�Шt�ΡA���ন���V�q
	//    �u������ rotation �|���� Normal ����V�A���W���� Final ModelView Matrix�A�P���ഫ�����Y�y��
	//    m_mxMV3X3Final �b Update �B����  m_mxMVFinal �����W�� 3X3 ����, ���ݭn�첾�q
#ifdef GENERAL_CASE
	//	  m_mxITMV �w�g�b�I�s SetModelViewMatrix(mat4 &mat) �P SetTRSMatrix(mat4 &mat) �N�P�ɭp��
	//    ���B�N���ΦA���s�p��
	vN = m_mxITMV * vNormal;
#else
	vN = m_mxMV3X3Final * vNormal;
#endif

	vN = normalize(vN);

	// 3. �p�� Ambient color :  Ia = Ka * Material.ambient * La
	vColor = m_Material.ka * m_Material.ambient * lights.diffuse;

	// 4. �p�� Light �b���Y�y�Шt�Ϊ���m
	//    ���B���] Light ����m�w�g�b�@�ɮy�Ъ������m
	vLightInView = m_mxView * lights.position;

	vec3 vLight; // �� vec3 �ӫŧi�O���F�`�٭p��, �p�G�n���{���g�_�ӧ��K�A�i��� vec4 �ӫŧi
	vLight.x = vLightInView.x - vPosInView.x;
	vLight.y = vLightInView.y - vPosInView.y;
	vLight.z = vLightInView.z - vPosInView.z;
	vLight = normalize(vLight); // normalize light vector

	// 5. �p�� L dot N
	GLfloat fLdotN = vLight.x*vN.x + vLight.y*vN.y + vLight.z*vN.z;
	if (fLdotN > 0) { // ���I�Q�����Ө�~�ݭn�p��
		// Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
		vColor += m_Material.kd * m_Material.diffuse * lights.diffuse * fLdotN;

		// Specular color
		// Method 1: Phone Model
		//   �p�� View Vector
		vec3 vView;
		vView.x = 0 - vPosInView.x;  // �ثe�w�g�H���Y�y�Ь���¦, �ҥH View ����m�N�O (0,0,0)
		vView.y = 0 - vPosInView.y;
		vView.z = 0 - vPosInView.z;
		vView = normalize(vView);

		//	 �p�� Light �� �Ϯg�� vRefL
		vec3 vRefL = 2.0f * (fLdotN)* vN - vLight;

		vRefL = normalize(vRefL);
		//   �p��  vReflectedL dot View
		GLfloat RdotV = vRefL.x*vView.x + vRefL.y*vView.y + vRefL.z*vView.z;

		// Specular Color : Is = Ks * Ls * (R dot V)^Shininess;
		if (RdotV > 0) vColor += m_Material.ks * m_Material.specular * lights.diffuse * powf(RdotV, m_Material.shininess);
	}

	vColor.w = 1; // Alpha ��]�w�� 1�A�w�]���O���z������
	// Method 2: Modified Phone Model 

	// �N�C���x�s��  m_Colors ���A�]�� Quad �O��Ӧ@�������T�����Һc��, �ҥH�]�w��ӤT���������ۦP���C��
	// �]�N�O�]�w�Ҧ������I���O�o���C��
	return vColor;
}

// Phong Reflection Model
vec4 CShape::PhongReflectionModel(vec4 vPoint, vec3 vNormal, vec4 vLightPos, color4 vLightI)
{
	vec3 vN;		// �� vec3 �ӫŧi�O���F�`�٭p��, �p�G�n���{���g�_�ӧ��K�A�i��� vec4 �ӫŧi
	vec4 vColor;	// �ӳ��I���C��
	vec4 vPosInView, vLightInView;

	// 1. �N�I��������Y�y�Шt�ΡA�]�����I�|�ھ� TRS Matrix ���ʡA�ҥH�����H m_mxMVFinal �p��b�@�ɮy�Ъ������m
	vPosInView = m_mxMVFinal * vPoint;

	// 2. �N���� Normal ������Y�y�Шt�ΡA���ন���V�q
	//    �u������ rotation �|���� Normal ����V�A���W���� Final ModelView Matrix�A�P���ഫ�����Y�y��
	//    m_mxMV3X3Final �b Update �B���� m_mxMVFinal �����W�� 3X3 ����, ���ݭn�첾�q
#ifdef GENERAL_CASE
	//	  m_mxITMV �w�g�b�I�s SetModelViewMatrix(mat4 &mat) �P SetTRSMatrix(mat4 &mat) �N�P�ɭp��
	//    ���B�N���ΦA���s�p��
	vN = m_mxITMV * vNormal;
#else
	vN = m_mxMV3X3Final * vNormal;
#endif

	vN = normalize(vN);

	// 3. �p�� Ambient color :  Ia = Ka * Material.ambient * La
	vColor = m_Material.ka * m_Material.ambient * vLightI;

	// 4. �p�� Light �b���Y�y�Шt�Ϊ���m
	//    ���B���] Light ����m�w�g�b�@�ɮy�Ъ������m
	vLightInView = m_mxView * vLightPos;

	vec3 vLight; // �� vec3 �ӫŧi�O���F�`�٭p��, �p�G�n���{���g�_�ӧ��K�A�i��� vec4 �ӫŧi
	vLight.x = vLightInView.x - vPosInView.x;
	vLight.y = vLightInView.y - vPosInView.y;
	vLight.z = vLightInView.z - vPosInView.z;
	vLight = normalize(vLight); // normalize light vector

								// 5. �p�� L dot N
	GLfloat fLdotN = vLight.x*vN.x + vLight.y*vN.y + vLight.z*vN.z;
	if (fLdotN > 0) { // ���I�Q�����Ө�~�ݭn�p��
					  // Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
		vColor += m_Material.kd * m_Material.diffuse * vLightI * fLdotN;

		// Specular color
		// Method 1: Phone Model
		//   �p�� View Vector
		vec3 vView;
		vView.x = 0 - vPosInView.x;  // �ثe�w�g�H���Y�y�Ь���¦, �ҥH View ����m�N�O (0,0,0)
		vView.y = 0 - vPosInView.y;
		vView.z = 0 - vPosInView.z;
		vView = normalize(vView);

		//	 �p�� Light �� �Ϯg�� vRefL
		vec3 vRefL = 2.0f * (fLdotN)* vN - vLight;

		vRefL = normalize(vRefL);
		//   �p��  vReflectedL dot View
		GLfloat RdotV = vRefL.x*vView.x + vRefL.y*vView.y + vRefL.z*vView.z;

		// Specular Color : Is = Ks * Ls * (R dot V)^Shininess;
		if (RdotV > 0) vColor += m_Material.ks * m_Material.specular * vLightI * powf(RdotV, m_Material.shininess);
	}

	vColor.w = 1; // Alpha ��]�w�� 1�A�w�]���O���z������
				  // Method 2: Modified Phone Model 

				  // �N�C���x�s��  m_Colors ���A�]�� Quad �O��Ӧ@�������T�����Һc��, �ҥH�]�w��ӤT���������ۦP���C��
				  // �]�N�O�]�w�Ҧ������I���O�o���C��
	return vColor;
}


// For controlling texture mapping ----------------
void CShape::SetMirror(bool uAxis, bool vAxis) // U�b �P V�b �O�_�n��g
{
	if( uAxis )  {
		if( vAxis ) { // U V �b�P����g
			for(int i = 0 ; i < m_iNumVtx ; i++ ) { // �N�C�@�� vertex ���K�Ϯy�Х� 1 �h��
				m_pTex1[i].x -= 1.0f; m_pTex1[i].y -= 1.0f;
			}
		}
		else { // �u�� U �b��g
			for(int i = 0 ; i < m_iNumVtx ; i++ ) { // �N�C�@�� vertex ���K�Ϯy�Х� 1 �h��
				m_pTex1[i].x -= 1.0f; // x �N�O U �b
			}
		}
	}
	else if( vAxis ) { // �u�� V �b��g
		for(int i = 0 ; i < m_iNumVtx ; i++ ) { // �N�C�@�� vertex ���K�Ϯy�Х� 1 �h��
			m_pTex1[i].y -= 1.0f; // y �� V �b
		}
	}
	else ;
	glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );
	glBufferSubData( GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4))*m_iNumVtx, sizeof(vec2)*m_iNumVtx, m_pTex1 ); // vertcies' Color

}

void CShape::SetTiling(float uTiling, float vTiling)  // �� U�b �P V�b �i����K���Y��
{
	for(int i = 0 ; i < m_iNumVtx ; i++ ) { // �N�C�@�� vertex ���K�Ϯy�Х� 1 �h��
		m_pTex1[i].x *= uTiling; m_pTex1[i].y *= vTiling;
	}
	glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );
	glBufferSubData( GL_ARRAY_BUFFER, (sizeof(vec4)+sizeof(vec3)+sizeof(vec4))*m_iNumVtx, sizeof(vec2)*m_iNumVtx, m_pTex1 ); // vertcies' Color
}
//-----------------------------------------------------------------------------------------------------------------------------