/* 

Amine Rehioui
Created: September 21st 2013

*/

#ifndef _SCOREOID_UTILS_H_INCLUDED_
#define _SCOREOID_UTILS_H_INCLUDED_

#include "NetResult.h"

// forwards
namespace tinyxml2
{
	class XMLDocument;
}

namespace shoot
{
	//! ScoreoidUtils
	class ScoreoidUtils
	{
	public:

		typedef std::map<std::string, std::string> Params;

		//! score info
		struct ScoreInfo
		{
			std::string m_Rank;
			std::string m_Name;
			std::string m_Score;
			std::string m_Platform;
		};

		//! Sends a scoreoid request
		static void Request(const std::string& request, Params& params, NetResultCallback* pResultCallback);

		//! Checks the player info
		static bool CheckPlayerInfo(const std::string& info, bool& bPlayerExists, bool& bPasswordMatch);
		
		//! Gets the score infos
		static bool GetScoreInfos(const std::string& result, std::list<ScoreInfo>& infos);

		//! Gets the player count
		static bool GetPlayerCount(const std::string& result, s32& playerCount);

		//! Gets the player rank
		static bool GetPlayerRank(const std::string& result, s32& playerRank);

		//! returns the result status
		static bool IsResultOK(const std::string& result);

		//! returns the platform string
		static const char* GetPlatformString();

	private:

		//! parses scoreoid XML
		static bool ParseXML(const std::string& xml, tinyxml2::XMLDocument& document);
	};
}

#endif // _SCOREOID_UTILS_H_INCLUDED_

