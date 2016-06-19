// Modified by Amine Rehioui, August 2010 
//- All file operations are now done through the shoot::File class

#include "Shoot.h"

#pragma warning(disable : 4786)
#include "MS3DFile.h"
#include <set>

#include "File.h"

#define MAKEDWORD(a, b)      ((unsigned int)(((word)(a)) | ((word)((word)(b))) << 16))

class CMS3DFileI
{
public:
	std::vector<ms3d_vertex_t> arrVertices;
	std::vector<ms3d_triangle_t> arrTriangles;
	std::vector<ms3d_edge_t> arrEdges;
	std::vector<ms3d_group_t> arrGroups;
	std::vector<ms3d_material_t> arrMaterials;
	float fAnimationFPS;
	float fCurrentTime;
	int iTotalFrames;
	std::vector<ms3d_joint_t> arrJoints;

public:
	CMS3DFileI()
	:	fAnimationFPS(24.0f),
		fCurrentTime(0.0f),
		iTotalFrames(0)
	{
	}

	// AmineR: fix memory leaks
	~CMS3DFileI()
	{
		for(unsigned int i=0; i<arrGroups.size(); ++i) delete[] arrGroups[i].triangleIndices;
		for(unsigned int i=0; i<arrJoints.size(); ++i) delete[] arrJoints[i].keyFramesRot;
		for(unsigned int i=0; i<arrJoints.size(); ++i) delete[] arrJoints[i].keyFramesTrans;
	}
};

CMS3DFile::CMS3DFile()
{
	_i = snew CMS3DFileI();
}

CMS3DFile::~CMS3DFile()
{
	delete _i;
}

bool CMS3DFile::LoadFromFile(const char* lpszFileName)
{
	shoot::File* pFile = shoot::File::Create(lpszFileName, shoot::File::M_ReadBinary);	
	if (!pFile->Open())
	{
		delete pFile;
		return false;
	}

	size_t i;
	ms3d_header_t header;
	pFile->Read(&header, sizeof(ms3d_header_t));	

	if (strncmp(header.id, "MS3D000000", 10) != 0)
		return false;

	if (header.version != 4)
		return false;

	// vertices
	word nNumVertices;	
	pFile->Read(&nNumVertices, sizeof(word));	
	_i->arrVertices.resize(nNumVertices);	
	pFile->Read(&_i->arrVertices[0], nNumVertices*sizeof(ms3d_vertex_t));	

	// triangles
	word nNumTriangles;	
	pFile->Read(&nNumTriangles, sizeof(word));	
	_i->arrTriangles.resize(nNumTriangles);	
	pFile->Read(&_i->arrTriangles[0], nNumTriangles*sizeof(ms3d_triangle_t));	

	// edges
	std::set<unsigned int> setEdgePair;
	for (i = 0; i < _i->arrTriangles.size(); i++)
	{
		word a, b;
		a = _i->arrTriangles[i].vertexIndices[0];
		b = _i->arrTriangles[i].vertexIndices[1];
		if (a > b)
			std::swap(a, b);
		if (setEdgePair.find(MAKEDWORD(a, b)) == setEdgePair.end())
			setEdgePair.insert(MAKEDWORD(a, b));

		a = _i->arrTriangles[i].vertexIndices[1];
		b = _i->arrTriangles[i].vertexIndices[2];
		if (a > b)
			std::swap(a, b);
		if (setEdgePair.find(MAKEDWORD(a, b)) == setEdgePair.end())
			setEdgePair.insert(MAKEDWORD(a, b));

		a = _i->arrTriangles[i].vertexIndices[2];
		b = _i->arrTriangles[i].vertexIndices[0];
		if (a > b)
			std::swap(a, b);
		if (setEdgePair.find(MAKEDWORD(a, b)) == setEdgePair.end())
			setEdgePair.insert(MAKEDWORD(a, b));
	}

	for(std::set<unsigned int>::iterator it = setEdgePair.begin(); it != setEdgePair.end(); ++it)
	{
		unsigned int EdgePair = *it;
		ms3d_edge_t Edge;
		Edge.edgeIndices[0] = (word) EdgePair;
		Edge.edgeIndices[1] = (word) ((EdgePair >> 16) & 0xFFFF);
		_i->arrEdges.push_back(Edge);
	}

	// groups
	word nNumGroups;	
	pFile->Read(&nNumGroups, sizeof(word));	

	_i->arrGroups.resize(nNumGroups);
	for (i = 0; i < nNumGroups; i++)
	{		
		pFile->Read(&_i->arrGroups[i].flags, sizeof(byte));			
		pFile->Read(&_i->arrGroups[i].name, 32*sizeof(char));		
		pFile->Read(&_i->arrGroups[i].numtriangles, sizeof(word));	
		_i->arrGroups[i].triangleIndices = snew word[_i->arrGroups[i].numtriangles];		
		pFile->Read(_i->arrGroups[i].triangleIndices, _i->arrGroups[i].numtriangles*sizeof(word));			
		pFile->Read(&_i->arrGroups[i].materialIndex, sizeof(char));	
	}

	// materials
	word nNumMaterials;	
	pFile->Read(&nNumMaterials, sizeof(word));	
	_i->arrMaterials.resize(nNumMaterials);	
	pFile->Read(&_i->arrMaterials[0], nNumMaterials*sizeof(ms3d_material_t));	

	pFile->Read(&_i->fAnimationFPS, sizeof(float));		
	pFile->Read(&_i->fCurrentTime, sizeof(float));	
	pFile->Read(&_i->iTotalFrames, sizeof(int));

	// joints
	word nNumJoints;	
	pFile->Read(&nNumJoints, sizeof(word));
	_i->arrJoints.resize(nNumJoints);
	for (i = 0; i < nNumJoints; i++)
	{
		pFile->Read(&_i->arrJoints[i].flags, sizeof(byte));
		pFile->Read(&_i->arrJoints[i].name, 32*sizeof(char));
		pFile->Read(&_i->arrJoints[i].parentName, 32*sizeof(char));
		pFile->Read(&_i->arrJoints[i].rotation, 3*sizeof(float));
		pFile->Read(&_i->arrJoints[i].position, 3*sizeof(float));
		pFile->Read(&_i->arrJoints[i].numKeyFramesRot, sizeof(word));
		pFile->Read(&_i->arrJoints[i].numKeyFramesTrans, sizeof(word));
		_i->arrJoints[i].keyFramesRot = snew ms3d_keyframe_rot_t[_i->arrJoints[i].numKeyFramesRot];
		_i->arrJoints[i].keyFramesTrans = snew ms3d_keyframe_pos_t[_i->arrJoints[i].numKeyFramesTrans];
		pFile->Read(_i->arrJoints[i].keyFramesRot, _i->arrJoints[i].numKeyFramesRot*sizeof(ms3d_keyframe_rot_t));
		pFile->Read(_i->arrJoints[i].keyFramesTrans, _i->arrJoints[i].numKeyFramesTrans*sizeof(ms3d_keyframe_pos_t));
	}

	pFile->Close();
	delete pFile;

	return true;
}

void CMS3DFile::Clear()
{
	_i->arrVertices.clear();
	_i->arrTriangles.clear();
	_i->arrEdges.clear();
	_i->arrGroups.clear();
	_i->arrMaterials.clear();
	_i->arrJoints.clear();
}

int CMS3DFile::GetNumVertices()
{
	return (int) _i->arrVertices.size();
}

void CMS3DFile::GetVertexAt(int nIndex, ms3d_vertex_t **ppVertex)
{
	if (nIndex >= 0 && nIndex < (int) _i->arrVertices.size())
		*ppVertex = &_i->arrVertices[nIndex];
}

int CMS3DFile::GetNumTriangles()
{
	return (int) _i->arrTriangles.size();
}

void CMS3DFile::GetTriangleAt(int nIndex, ms3d_triangle_t **ppTriangle)
{
	if (nIndex >= 0 && nIndex < (int) _i->arrTriangles.size())
		*ppTriangle = &_i->arrTriangles[nIndex];
}

int CMS3DFile::GetNumEdges()
{
	return (int) _i->arrEdges.size();
}

void CMS3DFile::GetEdgeAt(int nIndex, ms3d_edge_t **ppEdge)
{
	if (nIndex >= 0 && nIndex < (int) _i->arrEdges.size())
		*ppEdge = &_i->arrEdges[nIndex];
}

int CMS3DFile::GetNumGroups()
{
	return (int) _i->arrGroups.size();
}

void CMS3DFile::GetGroupAt(int nIndex, ms3d_group_t **ppGroup)
{
	if (nIndex >= 0 && nIndex < (int) _i->arrGroups.size())
		*ppGroup = &_i->arrGroups[nIndex];
}

int CMS3DFile::GetNumMaterials()
{
	return (int) _i->arrMaterials.size();
}

void CMS3DFile::GetMaterialAt(int nIndex, ms3d_material_t **ppMaterial)
{
	if (nIndex >= 0 && nIndex < (int) _i->arrMaterials.size())
		*ppMaterial = &_i->arrMaterials[nIndex];
}

int CMS3DFile::GetNumJoints()
{
	return (int) _i->arrJoints.size();
}

void CMS3DFile::GetJointAt(int nIndex, ms3d_joint_t **ppJoint)
{
	if (nIndex >= 0 && nIndex < (int) _i->arrJoints.size())
		*ppJoint = &_i->arrJoints[nIndex];
}

int CMS3DFile::FindJointByName(const char* lpszName)
{
	for (size_t i = 0; i < _i->arrJoints.size(); i++)
	{
		if (!strcmp(_i->arrJoints[i].name, lpszName))
			return i;
	}

	return -1;
}

float CMS3DFile::GetAnimationFPS()
{
	return _i->fAnimationFPS;
}

float CMS3DFile::GetCurrentTime()
{
	return _i->fCurrentTime;
}

int CMS3DFile::GetTotalFrames()
{
	return _i->iTotalFrames;
}
