/* 

Amine Rehioui
Created: May 25th 2013

*/

#ifndef _ATLAS_MANAGER_H_INCLUDED_
#define _ATLAS_MANAGER_H_INCLUDED_

#include "AABBox2D.h"

namespace shoot
{
	//! AtlasManager
	class AtlasManager : public Singleton<AtlasManager>
	{
		DECLARE_SINGLETON(AtlasManager);

	public:

		//! TextureInfo
		struct TextureInfo
		{
			std::string m_strPath;
			AABBox2D m_Rect;
		};

		//! returns texture info
		bool GetInfo(const std::string& strTexturePath, TextureInfo& info) const;

	private:
		
	};
}

#endif // _ATLAS_MANAGER_H_INCLUDED_

