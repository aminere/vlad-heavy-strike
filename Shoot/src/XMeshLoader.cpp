/* 

Amine Rehioui
Created: December 12th 2010

*/

#include "Shoot.h"

#include "XMeshLoader.h"

#include "File.h"

#include "Utils.h"

// X Header definition
#define XOFFILE_FORMAT_MAGIC ((long)'x' + ((long)'o' << 8) + ((long)'f' << 16) + ((long)' ' << 24))
#define XOFFILE_FORMAT_VERSION ((long)'0' + ((long)'3' << 8))
#define XOFFILE_FORMAT_TEXT ((long)'t' + ((long)'x' << 8) + ((long)'t' << 16) + ((long)' ' << 24))

namespace shoot
{
	// constants
	const char* XMeshLoader::sFrame = "Frame";
	const char* XMeshLoader::sFrameTransformMatrix = "FrameTransformMatrix";	
	const char* XMeshLoader::sMesh = "Mesh";
	const char* XMeshLoader::sMeshNormals = "MeshNormals";
	const char* XMeshLoader::sMeshVertexColors = "MeshVertexColors";
	const char* XMeshLoader::sMeshTextureCoords = "MeshTextureCoords";
	const char* XMeshLoader::sMeshMaterialList = "MeshMaterialList";	
	const char* XMeshLoader::sMaterial = "Material";
	const char* XMeshLoader::sTextureFileName = "TextureFileName";
	const char* XMeshLoader::sTextureFilename = "TextureFilename";
	const char* XMeshLoader::sVertexDuplicationIndices = "VertexDuplicationIndices";
	const char* XMeshLoader::sSkinMeshHeader = "SkinMeshHeader";
	const char* XMeshLoader::sXSkinMeshHeader = "XSkinMeshHeader";
	const char* XMeshLoader::sSkinWeights = "SkinWeights";
	const char* XMeshLoader::sAnimationSet = "AnimationSet";
	const char* XMeshLoader::sAnimation = "Animation";
	const char* XMeshLoader::sAnimationKey = "AnimationKey";	

	//! constructor
	XMeshLoader::XMeshLoader(MeshResource* pMesh)
		: m_pMesh(pMesh)
		, m_CurrentTransformation(Matrix44::Identity)
	{
	}

	//! loads a mesh from a file
	void XMeshLoader::Load(const char* strPath)
	{
		if (!File::GetUseDataFolder())
		{
			m_pMesh->LoadFS(strPath);
			return;
		}
		
		File* pFile = File::Create(strPath, File::M_ReadBinary);
		pFile->Open();

		// read header
		XFileHeader header;
		pFile->Read(&header, 16);
		SHOOT_ASSERT(header.Magic == XOFFILE_FORMAT_MAGIC, "XMeshLoader::Load: invalid X file");
		SHOOT_ASSERT(header.Major_Version == XOFFILE_FORMAT_VERSION, "XMeshLoader::Load: major version unsupported");
		SHOOT_ASSERT(header.Minor_Version == XOFFILE_FORMAT_VERSION, "XMeshLoader::Load: minor version unsupported");
		SHOOT_ASSERT(header.Format == XOFFILE_FORMAT_TEXT, "XMeshLoader::Load: file is not in text format");
		
		Parse(pFile);
	
		pFile->Close();
		delete pFile;
	}

	//! parses the file
	void XMeshLoader::Parse(shoot::File* pFile)
	{		
		std::string token;
		while(!pFile->EOFReached() && (token = GetNextToken(pFile)).length() > 0)
		{
			if(token == sFrame)
			{
				std::vector<MeshResource::SubMesh>& aSubMeshes = m_pMesh->GetSubMeshes();
				m_aUnusedSubMeshes = aSubMeshes;

				ParseFrame(pFile);

				// Remove unused VertexBuffers
				for(; !m_aUnusedSubMeshes.empty(); m_aUnusedSubMeshes.erase(m_aUnusedSubMeshes.begin()))
				{
					std::vector<MeshResource::SubMesh>::iterator it = std::find(aSubMeshes.begin(), aSubMeshes.end(), m_aUnusedSubMeshes[0]);			
					aSubMeshes.erase(it);
				}
			}
		}	
	}

	//! parses a frame
	void XMeshLoader::ParseFrame(shoot::File* pFile)
	{
		std::string name = GetNextToken(pFile);

		std::string token;
		while(!pFile->EOFReached() && (token = GetNextToken(pFile)) != "")
		{
			if(token == sFrame)
			{
				ParseFrame(pFile);
			}
			else if(token == sMesh)
			{
				ParseMesh(pFile, name);
			}
			else if(token == sFrameTransformMatrix)
			{
				for(u32 i=0; i<16; ++i)
				{
					m_CurrentTransformation[i] = ParseFloat(pFile);
				}
			}
		}
	}

	//! parses a mesh
	void XMeshLoader::ParseMesh(shoot::File* pFile, const std::string& strName)
	{
		pFile->Ignore(1024, '{');
		s32 numVertices = ParseInt(pFile);	
		Vertex3D* pVertices = snew Vertex3D[numVertices];

		// read the vertices
		for(s32 i=0; i<numVertices; ++i)
		{
			Vector3 pos = ParseVec3D(pFile);
			pVertices[i].Pos = pos;
		}
				
		std::vector<MeshResource::SubMesh>& aSubMeshes = m_pMesh->GetSubMeshes();

		// Look for an existing sub mesh
		s32 subMeshIndex = -1;
		for(u32 i=0; i<aSubMeshes.size(); ++i)
		{
			if(aSubMeshes[i].m_VertexBuffer->GetName() == strName)
			{
				subMeshIndex = i;
				m_aUnusedSubMeshes.erase(std::find(m_aUnusedSubMeshes.begin(), m_aUnusedSubMeshes.end(), aSubMeshes[i]));
				break;
			}
		}

		if(subMeshIndex == -1)
		{
			// Create a new sub mesh
			MeshResource::SubMesh subMesh = { Reference<VertexBuffer>(GraphicsDriver::Instance()->CreateVertexBuffer()), m_CurrentTransformation.GetTranslation() };
			m_pMesh->AddSubMesh(subMesh);
			subMeshIndex = aSubMeshes.size()-1;
		}
		aSubMeshes[subMeshIndex].m_VertexBuffer->SetVertices(pVertices, numVertices);
		aSubMeshes[subMeshIndex].m_VertexBuffer->SetVertexFlag(Vertex3D::VF_Pos);
		aSubMeshes[subMeshIndex].m_VertexBuffer->SetName(strName);
		aSubMeshes[subMeshIndex].m_vPosition = m_CurrentTransformation.GetTranslation();

		// read the faces - NOTE: only triangulated meshes supported
		s32 numTriangles = ParseInt(pFile);
		s32 numIndices = numTriangles*3;
		u16* pIndices = snew u16[numIndices];
		
		for(s32 i=0; i<numTriangles; ++i)
		{
			s32 numIndicesPerFace = ParseInt(pFile);
			SHOOT_ASSERT(numIndicesPerFace == 3, "XMeshLoader::ParseMesh: only triangulated meshes are supported");
			pIndices[(i*3) + 0] = ParseInt(pFile);
			pIndices[(i*3) + 1] = ParseInt(pFile);
			pIndices[(i*3) + 2] = ParseInt(pFile);
		}

		aSubMeshes[subMeshIndex].m_VertexBuffer->SetIndices(pIndices, numIndices);		

		// parse mesh sub templates
		while(!pFile->EOFReached())
		{
			std::string token = GetNextToken(pFile);
			if(token == sMeshNormals)
			{
				ParseMeshNormals(pFile, aSubMeshes[subMeshIndex].m_VertexBuffer);
			}
			else if(token == sMeshVertexColors)
			{
				ParseMeshVertexColors(pFile, aSubMeshes[subMeshIndex].m_VertexBuffer);
			}
			else if(token == sMeshTextureCoords)
			{
				ParseMeshTextureCoords(pFile, aSubMeshes[subMeshIndex].m_VertexBuffer);
			}
			else if(token == sMeshMaterialList)
			{
				ParseMeshMaterialList(pFile);
			}
			else if(token == sSkinMeshHeader
				 || token == sXSkinMeshHeader)
			{
				ParseSkinMeshHeader(pFile);
			}
			else if(token == sSkinWeights)
			{
				ParseSkinWeights(pFile);
			}
			else if(token == sVertexDuplicationIndices)
			{
				ParseVertexDuplicationIndices(pFile);
			}
			else
			{
				if(token.length() > 0)
				{
					CancelTokenRead(token, pFile);
				}
				break; // done parsing the mesh
			}
		}		
	}

	//! parses mesh normals
	void XMeshLoader::ParseMeshNormals(shoot::File* pFile, VertexBuffer* pVertexBuffer)
	{
		s32 numVertices = ParseInt(pFile);
		SHOOT_ASSERT(numVertices == pVertexBuffer->GetNumVertices(), "XMeshLoader::ParseMeshNormals: numVertices changed");

		// read the normals
		for(s32 i=0; i<numVertices; ++i)
		{
			Vector3 normal = ParseVec3D(pFile);
			pVertexBuffer->GetVertices()[i].Normal = normal;
		}

		// read face normals: TODO do something with them
		s32 numFaceNormals = ParseInt(pFile);
		for(s32 i=0; i<numFaceNormals; ++i)
		{
			s32 numIndicesPerFace = ParseInt(pFile);
			for(s32 j=0; j<numIndicesPerFace; j++)
			{
				ParseInt(pFile);
			}
		}

		pVertexBuffer->SetVertexFlag(Vertex3D::VF_Normal);
	}

	//! parses mesh vertex colors
	void XMeshLoader::ParseMeshVertexColors(shoot::File* pFile, VertexBuffer* pVertexBuffer)
	{
		s32 numVertices = ParseInt(pFile);
		SHOOT_ASSERT(numVertices == pVertexBuffer->GetNumVertices(), "XMeshLoader::ParseMeshVertexColors: numVertices changed");

		// read the colors
		for(s32 i=0; i<numVertices; ++i)
		{
			s32 vertex = ParseInt(pFile);
			Color color = ParseColorRGBA(pFile);
			pVertexBuffer->GetVertices()[vertex].color = color;			
		}

		pVertexBuffer->SetVertexFlag(Vertex3D::VF_Color);
	}

	//! parses mesh texture coords
	void XMeshLoader::ParseMeshTextureCoords(shoot::File* pFile, VertexBuffer* pVertexBuffer)
	{
		s32 numVertices = ParseInt(pFile);
		SHOOT_ASSERT(numVertices == pVertexBuffer->GetNumVertices(), "XMeshLoader::ParseMeshNormals: numVertices changed");

		// read the UVs
		for(s32 i=0; i<numVertices; ++i)
		{
			Vector2 UV = ParseVec2D(pFile);
			pVertexBuffer->GetVertices()[i].UV = UV;
		}

		pVertexBuffer->SetVertexFlag(Vertex3D::VF_UV);
	}

	//! parses mesh material lists
	void XMeshLoader::ParseMeshMaterialList(shoot::File* pFile)
	{
		s32 numMaterials = ParseInt(pFile);
		SHOOT_WARNING(numMaterials <= 1, "Only one mesh material supported for now");
		s32 numFaces = ParseInt(pFile);

		// TODO: support multiple materials
		for(s32 i=0; i<numFaces; ++i)
		{
			s32 materialIndex = ParseInt(pFile);
		}

		for(s32 i=0; i<numMaterials; ++i)
		{
			std::string token = GetNextToken(pFile);
			SHOOT_ASSERT(token == sMaterial, "XMeshLoader::ParseMeshMaterialLists: could not find Material");
			pFile->Ignore(1024, '{');

			Color color = ParseColorRGBA(pFile);
			f32 power = ParseFloat(pFile);
			Color specular = ParseColorRGB(pFile);
			Color emissive = ParseColorRGB(pFile);			

			token = GetNextToken(pFile);
			std::string texturePath;
			if(token == sTextureFileName
			|| token == sTextureFilename)
			{
				texturePath = ParseTextureFileName(pFile);
			}
			else
			{
				CancelTokenRead(token, pFile);
			}
			
			Material::CreationInfo materialInfo;
			materialInfo.m_Color = color;

			if(texturePath.length() > 0)
			{
				// determine texture path
				std::string meshPath = pFile->GetPath();
				size_t dirIndex = meshPath.rfind("/");
				std::string basePath = meshPath.substr(0, dirIndex+1);
				texturePath = Utils::FormatResourcePath(basePath + texturePath);				
						
				Material::CreationInfo::TextureInfo* pInfo = snew Material::CreationInfo::TextureInfo();
				pInfo->m_Path = texturePath;
				materialInfo.m_TextureInfos.Add(pInfo);
			}

			m_pMesh->AddMaterial(materialInfo);	
		}
	}

	//! parses a texture file name
	std::string XMeshLoader::ParseTextureFileName(shoot::File* pFile)
	{
		std::string filename = GetNextToken(pFile);
		char strBuf[512];
		strcpy(strBuf, filename.c_str());
		std::string result = strtok(strBuf, ";,\"");
		return result;
	}

	//! parses vertex duplicatioin indices
	void  XMeshLoader::ParseVertexDuplicationIndices(shoot::File* pFile)
	{
		// TODO: support
		s32 numIndices = ParseInt(pFile);
		s32 numOroginalVerices = ParseInt(pFile);
		for(s32 i=0; i<numIndices; ++i)
		{
			s32 index = ParseInt(pFile);;
		}
	}

	//! parses skin mesh header
	void XMeshLoader::ParseSkinMeshHeader(shoot::File* pFile)
	{
		// TODO: support
		s32 maxSkinWeightsPerVertex = ParseInt(pFile);		
		s32 maxSkinWeightsPerFace = ParseInt(pFile); 
		s32 numBones = ParseInt(pFile);
	}

	//! parses skin weights
	void XMeshLoader::ParseSkinWeights(shoot::File* pFile)
	{
		SHOOT_ASSERT(0, "XMeshLoader: skin weights not supported");
	}

	//! returns the next token in the file
	std::string XMeshLoader::GetNextToken(shoot::File* pFile)
	{
		std::string token = "";		
		if(SkipToNextToken(pFile))
		{	
			pFile->Read(token);
		}
		return token;
	}

	//! cancels a token read
	void XMeshLoader::CancelTokenRead(std::string token, shoot::File* pFile)
	{
		s32 length = token.length();
		pFile->SetOffset(-length);
	}

	//! returns true if a character is an empty space
	bool XMeshLoader::IsEmptySpace(char c)
	{
		bool bEmptySpace = (IsEndOfLine(c) || c == ' ' || c == '\t');
		return bEmptySpace;
	}

	//! returns true if a character is an operator
	bool XMeshLoader::IsOperator(char c)
	{
		bool bIsOperator = (c == ',' || c == ';' || c == '{' || c == '}' || c == '\"');
		return bIsOperator;
	}

	//! returns true if a character is an end of line marker
	bool XMeshLoader::IsEndOfLine(char c)
	{
		bool bEndOfLine = (c == '\r' || c == '\n');
		return bEndOfLine;
	}

	//! skips to next token
	/** returns true if a token was reached */
	bool XMeshLoader::SkipToNextToken(shoot::File* pFile)
	{
		// skip all white spaces
		char c = ' ';
		s32 bytesRead = 0;
		do
		{
			bytesRead = pFile->Read(&c, 1);

			// check for comment
			if(c == '/')
			{
				bytesRead = pFile->Read(&c, 1);
				SHOOT_ASSERT(bytesRead && c == '/', "XMeshLoader::SkipToNextToken: imcomplete comment marker");
				do
				{
					bytesRead = pFile->Read(&c, 1);
				}
				while(bytesRead > 0 && !IsEndOfLine(c));
			}
		}
		while(bytesRead > 0 && (IsEmptySpace(c) || IsOperator(c)) && !pFile->EOFReached());

		if(bytesRead > 0 && !pFile->EOFReached())
		{
			pFile->SetOffset(-1);
			return true;
		}
		else
		{
			return false; // reached EOF without finding a token
		}
	}

	//! parses an integer
	s32 XMeshLoader::ParseInt(shoot::File* pFile)
	{
		s32 result = s32(ParseFloat(pFile));
		return result;
	}

	//! parses a float
	f32 XMeshLoader::ParseFloat(shoot::File* pFile)
	{
		bool bTokenReached = SkipToNextToken(pFile);
		SHOOT_ASSERT(bTokenReached, "XMeshLoader::ParseFloat: could reach token");

		std::string str;
		pFile->Read(str, ';', ',');
		SHOOT_ASSERT(str.length() > 0, "XMeshLoader::ParseInt: error");
		char strBuf[512];
		char* strToken;
		strcpy(strBuf, str.c_str());
		strToken = strtok(strBuf, ";,");
		f32 result = f32(strtod(strToken, NULL));
		return result;
	}

	//! parses a 3D vector
	Vector3 XMeshLoader::ParseVec3D(shoot::File* pFile)
	{
		Vector3 result;
		result.X = ParseFloat(pFile);
		result.Y = ParseFloat(pFile);
		result.Z = ParseFloat(pFile);
		return result;
	}

	//! parses a 2D vector
	Vector2 XMeshLoader::ParseVec2D(shoot::File* pFile)
	{
		Vector2 result;
		result.X = ParseFloat(pFile);
		result.Y = ParseFloat(pFile);
		return result;
	}

	//! parses an RGBA color
	Color XMeshLoader::ParseColorRGBA(shoot::File* pFile)
	{
		Color result = ParseColorRGB(pFile);		
		result.A = ParseFloat(pFile);
		return result;
	}

	//! parses an RGB color
	Color XMeshLoader::ParseColorRGB(shoot::File* pFile)
	{
		Color result;
		result.R = ParseFloat(pFile);
		result.G = ParseFloat(pFile);
		result.B = ParseFloat(pFile);
		result.A = 1.0f;
		return result;
	}
}

