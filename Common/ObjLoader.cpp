#include "ObjLoader.h"
// Example 4 �}�l
// ���P Example 3 �¤W(Y�b)
// �C�@�� Vertex �W�[ Normal �A�令�~���� CShape�A�@�ֳB�z�������]�w�ݨD


ObjLoader::ObjLoader(char *adrFile)
{
	m_iNumVtx = QUAD_NUM;
	m_pPoints = NULL; m_pNormals = NULL; m_pTex = NULL;
	//�޼ƼȦs�ܼ�
	int vindex = 0;
	int nindex = 0;
	int tindex = 0;
	int mindex = 0;

	objfile = fopen(adrFile, "r");
	if (objfile == NULL) {
		printf("fail to open the file !\n");
	}
	else {
		while (fscanf(objfile, "%s", lineHeader) != EOF) {
			if (strcmp(lineHeader, "f") == 0) { vindex++; } //�p���I�`��
		}
		fclose(objfile);
		m_iNumVtx = vindex * 3;
		m_pPoints = new vec4[m_iNumVtx + 1];
		vindex = 0; mindex = 0;//�t������I���Ȧs

		//�̷ӹ�ڳ��I�ƨM�w�}�C�j�p
		m_pPoints = new vec4[m_iNumVtx];
		m_pNormals = new vec3[m_iNumVtx];
		m_pColors = new vec4[m_iNumVtx];
		m_pTex = new vec2[m_iNumVtx];
		vec4 *temp_vertices = new vec4[m_iNumVtx];
		vec3 *temp_pNormals = new vec3[m_iNumVtx];
		vec4 *temp_pColors = new vec4[m_iNumVtx];
		vec2 *temp_pTex = new vec2[m_iNumVtx];

		objfile = fopen(adrFile, "r");

		while (fscanf(objfile, "%s", lineHeader) != EOF) {
			if (strcmp(lineHeader, "v") == 0) {
				fscanf(objfile, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
				temp_vertices[vindex] = vec4(vertex[0], vertex[1], vertex[2], 1.0f);
				vindex++;
			}
			else if (strcmp(lineHeader, "vn") == 0) {
				fscanf(objfile, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
				temp_pNormals[nindex] = vec3(vertex[0], vertex[1], vertex[2]);
				nindex++;
			}

			else if (strcmp(lineHeader, "vt") == 0) {
				fscanf(objfile, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
				temp_pTex[tindex] = vec2(vertex[0], vertex[1]);
				tindex++;
			}
			else if (strcmp(lineHeader, "f") == 0) {
				//�Ш̷Ӧۤv��OBJ�ɨM�wŪ���覡
				//"f ���I����/uv�I����/�k�u����"
				fscanf(objfile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &indices[0], &indices[1], &indices[2], &indices[3], &indices[4], &indices[5], &indices[6], &indices[7], &indices[8]);
				m_pPoints[mindex] = temp_vertices[indices[0] - 1];
				m_pPoints[mindex + 1] = temp_vertices[indices[3] - 1];
				m_pPoints[mindex + 2] = temp_vertices[indices[6] - 1];
				m_pNormals[mindex] = temp_pNormals[indices[2] - 1];
				m_pNormals[mindex + 1] = temp_pNormals[indices[5] - 1];
				m_pNormals[mindex + 2] = temp_pNormals[indices[8] - 1];
				m_pTex[mindex] = temp_pTex[indices[1] - 1];
				m_pTex[mindex + 1] = temp_pTex[indices[4] - 1];
				m_pTex[mindex + 2] = temp_pTex[indices[7] - 1];
				mindex += 3;
			}
		}
	}
	fclose(objfile);
	// �w�]�N�Ҧ��������]�w���Ǧ�
	for (int i = 0; i < m_iNumVtx; i++) m_pColors[i] = vec4(-1.0f, -1.0f, -1.0f, 1.0f);


	m_pTex1 = new vec2[m_iNumVtx];		// ���ͩһݪ��K�Ϯy��
#if MULTITEXTURE >= LIGHT_MAP
	m_pTex2 = new vec2[m_iNumVtx];  // ���� light map �һݪ��K�Ϯy��
#endif
#if MULTITEXTURE >= NORMAL_MAP
	m_pTex3 = new vec2[m_iNumVtx];	// ���� normal map �һݪ��K�Ϯy��
	// ���ͩһݭn�� Tanget vector for each vertex
	m_pTangentV = new vec3[m_iNumVtx];
#endif

	for (int i = 0; i < m_iNumVtx; i++) {
		m_pTex1[i] = m_pTex[i];
	}


#if MULTITEXTURE >= LIGHT_MAP

	for (int i = 0; i < m_iNumVtx; i++) {
		m_pTex2[i] = m_pTex[i];
	}

#endif
#if MULTITEXTURE >= NORMAL_MAP

	for (int i = 0; i < m_iNumVtx; i++) {
		m_pTex3[i] = m_pTex[i];
	}

	// �p�� tangent vector
	for (int i = 0; i < m_iNumVtx; i += 3) {
		float dU1 = m_pTex1[i + 1].x - m_pTex1[i].x;
		float dV1 = m_pTex1[i + 1].y - m_pTex1[i].y;
		float dU2 = m_pTex1[i + 2].x - m_pTex1[i].x;
		float dV2 = m_pTex1[i + 2].y - m_pTex1[i].y;
		float f = 1.0f / (dU1 * dV2 - dU2*dV1);
		vec4 E1 = m_pPoints[i + 1] - m_pPoints[i];
		vec4 E2 = m_pPoints[i + 2] - m_pPoints[i];

		vec3 tangent;
		tangent.x = f*(dV2 * E1.x + E2.x * (-dV1));
		tangent.y = f*(dV2 * E1.y + E2.y * (-dV1));
		tangent.z = f*(dV2 * E1.z + E2.z * (-dV1));

		m_pTangentV[i] += tangent;
		m_pTangentV[i + 1] += tangent;
		m_pTangentV[i + 2] = tangent;
	}
	for (int i = 0; i < 6; i++)
		m_pTangentV[i] = normalize(m_pTangentV[i]);
#endif

#ifdef PERVERTEX_LIGHTING
	SetShaderName("vsPerVtxLighting.glsl", "fsPerVtxLighting.glsl");
#else
	SetShaderName("vsPerPixelLighting.glsl", "fsPerPixelLighting.glsl");
#endif

}

void ObjLoader::RenderWithFlatShading(const LightSource &lights)
{
	// �H�C�@�ӭ����T�ӳ��I�p��䭫�ߡA�H�ӭ��ߧ@���C��p�⪺�I��
	// �ھ� Phong lighting model �p��۹������C��A�ñN�C���x�s�즹�T���I��
	// �]���C�@�ӥ��������I�� Normal ���ۦP�A�ҥH���B�èS���p�⦹�T�ӳ��I������ Normal
	vec4 vCentroidP;
	for (int i = 0; i < m_iNumVtx; i += 3) {
		// �p��T���Ϊ�����
		vCentroidP = (m_pPoints[i] + m_pPoints[i + 1] + m_pPoints[i + 2]) / 3.0f;
		m_pColors[i] = m_pColors[i + 1] = m_pColors[i + 2] = PhongReflectionModel(vCentroidP, m_pNormals[i], lights);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}


void ObjLoader::RenderWithGouraudShading(const LightSource &lights)
{
	// �H vertex �����A�ھڸ��I����m�P��k�V�q�A�Q�� Phong lighting model �p��۹������C��
	// �N���C���x�s�^�ӳ��I

	// �Ĥ����I���c������ӤT����
	// �p�� 0 1 2 5 �|�ӳ��I���C��Y�i�A0 �P 3�B2 �P 4 ���C��ۦP

	m_pColors[0] = m_pColors[3] = PhongReflectionModel(m_pPoints[0], m_pNormals[0], lights);
	m_pColors[2] = m_pColors[4] = PhongReflectionModel(m_pPoints[2], m_pNormals[2], lights);
	m_pColors[1] = PhongReflectionModel(m_pPoints[1], m_pNormals[1], lights);
	m_pColors[5] = PhongReflectionModel(m_pPoints[5], m_pNormals[5], lights);

	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}



void ObjLoader::Update(float dt, const LightSource &lights)
{
#ifdef LIGHTING_WITHCPU
	if (m_bViewUpdated || m_bTRSUpdated) { // Model View �������x�}���e�����
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxMV3X3Final = mat3(
			m_mxMVFinal._m[0].x, m_mxMVFinal._m[1].x, m_mxMVFinal._m[2].x,
			m_mxMVFinal._m[0].y, m_mxMVFinal._m[1].y, m_mxMVFinal._m[2].y,
			m_mxMVFinal._m[0].z, m_mxMVFinal._m[1].z, m_mxMVFinal._m[2].z);
#ifdef GENERAL_CASE
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal);
#endif
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	if (m_iMode == FLAT_SHADING) RenderWithFlatShading(lights);
	else RenderWithGouraudShading(lights);

#else // Lighting With GPU
	if (m_bViewUpdated || m_bTRSUpdated) {
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	m_vLightInView = m_mxView * lights.position;		// �N Light �ഫ�����Y�y�ЦA�ǤJ
														// ��X AmbientProduct DiffuseProduct �P SpecularProduct �����e
	m_AmbientProduct = m_Material.ka * m_Material.ambient  * lights.ambient;
	m_AmbientProduct.w = m_Material.ambient.w;
	m_DiffuseProduct = m_Material.kd * m_Material.diffuse  * lights.diffuse;
	m_DiffuseProduct.w = m_Material.diffuse.w;
	m_SpecularProduct = m_Material.ks * m_Material.specular * lights.specular;
	m_SpecularProduct.w = m_Material.specular.w;
#endif

}


void ObjLoader::Update(float dt)
{
	if (m_bViewUpdated || m_bTRSUpdated) { // Model View �������x�}���e�����
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal);
		m_bViewUpdated = m_bTRSUpdated = false;
	}
}

void ObjLoader::SetVtxColors(vec4 vLFColor, vec4 vLRColor, vec4 vTRColor, vec4 vTLColor)
{
	m_pColors[3] = m_pColors[0] = vLFColor;
	m_pColors[1] = vLRColor;
	m_pColors[4] = m_pColors[2] = vTRColor;
	m_pColors[5] = vTLColor;

	// �N�Ҧ� vertices �C���s�� VBO ��
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}

void ObjLoader::SetVtxColors(vec4 vFColor, vec4 vSColor) // �]�w��ӤT���Ϊ��C��
{
	m_pColors[0] = m_pColors[1] = m_pColors[2] = vFColor;
	m_pColors[3] = m_pColors[4] = m_pColors[5] = vSColor;

	// �N�Ҧ� vertices �C���s�� VBO ��
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}

void ObjLoader::Draw()
{
	DrawingSetShader();
	//	glBindTexture(GL_TEXTURE_2D, m_uiTexObject[0]); 
	glDrawArrays(GL_TRIANGLES, 0, m_iNumVtx);
	//	glBindTexture(GL_TEXTURE_2D, 0);  // ����{�����K�ϳ]�w
}

void ObjLoader::DrawW()
{
	DrawingWithoutSetShader();
	glDrawArrays(GL_TRIANGLES, 0, m_iNumVtx);
}
