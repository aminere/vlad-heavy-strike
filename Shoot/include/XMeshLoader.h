/* 

Amine Rehioui
Created: December 12th 2010

*/

#ifndef _X_MESH_LOADER_H_INCLUDED_
#define _X_MESH_LOADER_H_INCLUDED_

#include "MeshResource.h"
#include "Vertex3D.h"

namespace shoot
{
	// forwards
	class File;
	class VertexBuffer;	

	//! X mesh loader
	class XMeshLoader
	{
	public:

		//! constructor
		XMeshLoader(MeshResource* pMesh);

		//! loads a mesh from a file
		void Load(const char* strPath);

	private:

		//! X file header structure
		struct XFileHeader
		{
			u32 Magic;
			u16 Major_Version;
			u16 Minor_Version;
			u32 Format;
			u32 Float_Size;
		};

		//! parses the file
		void Parse(shoot::File* pFile);

		//! parses a frame
		void ParseFrame(shoot::File* pFile);

		//! parses a mesh
		void ParseMesh(shoot::File* pFile, const std::string& strName);

		//! parses mesh normals
		void ParseMeshNormals(shoot::File* pFile, VertexBuffer* pVertexBuffer);

		//! parses mesh vertex colors
		void ParseMeshVertexColors(shoot::File* pFile, VertexBuffer* pVertexBuffer);

		//! parses mesh texture coords
		void ParseMeshTextureCoords(shoot::File* pFile, VertexBuffer* pVertexBuffer);

		//! parses mesh material lists
		void ParseMeshMaterialList(shoot::File* pFile);

		//! parses a texture file name
		std::string ParseTextureFileName(shoot::File* pFile);

		//! parses vertex duplicatioin indices
		void ParseVertexDuplicationIndices(shoot::File* pFile);

		//! parses skin mesh header
		void ParseSkinMeshHeader(shoot::File* pFile);

		//! parses skin weights
		void ParseSkinWeights(shoot::File* pFile);

		//! returns the next token in the file
		std::string GetNextToken(shoot::File* pFile);

		//! cancels a token read
		void CancelTokenRead(std::string token, shoot::File* pFile);

		//! returns true if a character is an empty space
		inline bool IsEmptySpace(char c);

		//! returns true if a character is an operator
		inline bool IsOperator(char c);

		//! returns true if a character is an end of line marker
		inline bool IsEndOfLine(char c);

		//! skips to next token
		/** returns true if a token was reached */
		bool SkipToNextToken(shoot::File* pFile);

		//! parses an integer
		s32 ParseInt(shoot::File* pFile);

		//! parses a float
		f32 ParseFloat(shoot::File* pFile);

		//! parses a 3D vector
		Vector3 ParseVec3D(shoot::File* pFile);

		//! parses a 2D vector
		Vector2 ParseVec2D(shoot::File* pFile);

		//! parses an RGBA color
		Color ParseColorRGBA(shoot::File* pFile);

		//! parses an RGB color
		Color ParseColorRGB(shoot::File* pFile);

		MeshResource* m_pMesh;
		std::vector<MeshResource::SubMesh> m_aUnusedSubMeshes;
		Matrix44 m_CurrentTransformation;

		// constants
		static const char* sFrame;
		static const char* sFrameTransformMatrix;	
		static const char* sMesh;
		static const char* sMeshNormals;
		static const char* sMeshVertexColors;
		static const char* sMeshTextureCoords;
		static const char* sMeshMaterialList;
		static const char* sMaterial;
		static const char* sTextureFileName;
		static const char* sTextureFilename;
		static const char* sVertexDuplicationIndices;
		static const char* sSkinMeshHeader;
		static const char* sXSkinMeshHeader;
		static const char* sSkinWeights;
		static const char* sAnimationSet;
		static const char* sAnimation;
		static const char* sAnimationKey;		
	};
}

#endif // _X_MESH_LOADER_H_INCLUDED_



