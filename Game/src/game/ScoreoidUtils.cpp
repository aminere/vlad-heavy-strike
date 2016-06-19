/* 

Amine Rehioui
Created: September 21st 2013

*/

#include "ShootTest.h"

#include "ScoreoidUtils.h"

#include "NetworkManager.h"

#include "Utils.h"

#include "tinyxml/tinyxml2.h"

namespace shoot
{
	//! Sends a scoreoid request
	void ScoreoidUtils::Request(const std::string& request, Params& params, NetResultCallback* pResultCallback)
	{
		std::string _params = "api_key=9ff88c86e775b2725ffdc40f034a40f0e677400a";
		_params += "&game_id=17d30ed391";
		_params += "&response=XML";

		for(Params::iterator it = params.begin(); it != params.end(); ++it)
		{
			std::string key = (*it).first;
			std::string value = (*it).second;
			_params = _params + "&" + key + "=" + value;
		}

		std::string httpRequest = "POST /v1/" + request + " HTTP/1.1\r\n";
		httpRequest += "Host: api.scoreoid.com\r\n";
		httpRequest += "Content-Type: application/x-www-form-urlencoded\r\n";
		httpRequest += "Content-Length: " + Utils::ToString(_params.size()) + "\r\n";
		httpRequest += "Connection: close\r\n\r\n";
		httpRequest += _params;

		NetworkManager::Instance()->SendData("api.scoreoid.com", 80, (u8*)httpRequest.c_str(), httpRequest.size(), pResultCallback);
	}

	//! Checks the player info
	bool ScoreoidUtils::CheckPlayerInfo(const std::string& info, bool& bPlayerExists, bool& bPasswordMatch)
	{
		tinyxml2::XMLDocument document;
		if(!ParseXML(info, document))
		{
			return false;
		}

		tinyxml2::XMLElement* pRoot = document.FirstChildElement();
		std::string rootTag  = pRoot->Value();
		Log.Print("CheckPlayer rootTag: %s\n", rootTag.c_str());
		if(rootTag == "error")
		{
			std::string error = pRoot->GetText();
			Log.Print("CheckPlayer error: %s\n", error.c_str());
			if(Utils::Equals(error.c_str(), "Invalid player password"))
			{
				bPlayerExists = true;
				bPasswordMatch = false;
			}
			else
			{
				bPlayerExists = false;
				bPasswordMatch = false;
			}			
		}
		else
		{
			bPlayerExists = true;
			bPasswordMatch = true;
		}

		return true;
	}

	//! Gets the score infos
	bool ScoreoidUtils::GetScoreInfos(const std::string& result, std::list<ScoreInfo>& infos)
	{
		tinyxml2::XMLDocument document;
		infos.clear();

		if(!ParseXML(result, document))
		{
			return false;
		}

		tinyxml2::XMLElement* pRoot = document.FirstChildElement();
		std::string rootTag  = pRoot->Value();
		Log.Print("GetScoreInfos rootTag: %s\n", rootTag.c_str());
		if(rootTag != "scores")
		{
			SHOOT_WARNING(false, "GetScoreInfos: Invalid XML data");
			return false;
		}

		tinyxml2::XMLElement* pPlayer = pRoot->FirstChildElement();
		s32 playerCount = 1;
		while(pPlayer)
		{
			ScoreInfo info;
			info.m_Name = pPlayer->Attribute("username");
			info.m_Rank = Utils::ToString(playerCount);
			
			tinyxml2::XMLElement* pScore = pPlayer->FirstChildElement();
			const char* strPlatform = pScore ? pScore->Attribute("platform") : NULL;
			const char* strScore = pScore ? pScore->Attribute("score") : NULL;
			if(!pScore || !strPlatform || !strScore)
			{
				SHOOT_WARNING(false, "GetScoreInfos: Invalid <score> tag (%x, %x, %x)", pScore, strPlatform, strScore);
				return false;
			}
			info.m_Platform = strPlatform;
			info.m_Score = strScore;
			infos.push_back(info);
			pPlayer = pPlayer->NextSiblingElement();
			++playerCount;
		}
		return true;
	}

	//! Gets the player count
	bool ScoreoidUtils::GetPlayerCount(const std::string& result, s32& playerCount)
	{		
		tinyxml2::XMLDocument document;
		playerCount = -1;

		if(!ParseXML(result, document))
		{
			return false;
		}

		tinyxml2::XMLElement* pRoot = document.FirstChildElement();
		std::string rootTag  = pRoot->Value();
		Log.Print("GetPlayerCount rootTag: %s\n", rootTag.c_str());
		if(rootTag == "players")
		{
			playerCount = Utils::FromString<s32>(pRoot->GetText());
			return true;
		}

		return false;
	}

	//! Gets the player rank
	bool ScoreoidUtils::GetPlayerRank(const std::string& result, s32& playerRank)
	{
		tinyxml2::XMLDocument document;
		playerRank = -1;

		if(!ParseXML(result, document))
		{
			return false;
		}

		tinyxml2::XMLElement* pRoot = document.FirstChildElement();
		std::string rootTag  = pRoot->Value();
		Log.Print("GetPlayerRank rootTag: %s\n", rootTag.c_str());
		if(rootTag == "rank")
		{
			playerRank = Utils::FromString<s32>(pRoot->GetText())-1;
			if(playerRank == 0)
			{
				playerRank = -1;
			}
			return true;
		}

		return false;
	}

	//! returns as result status
	bool ScoreoidUtils::IsResultOK(const std::string& result)
	{
		tinyxml2::XMLDocument document;
		if(!ParseXML(result, document))
		{
			return false;
		}

		tinyxml2::XMLElement* pRoot = document.FirstChildElement();
		std::string rootTag  = pRoot->Value();
		Log.Print("IsResultOK rootTag: %s\n", rootTag.c_str());
		return rootTag != "error";
	}

	//! parses scoreoid XML
	bool ScoreoidUtils::ParseXML(const std::string& xml, tinyxml2::XMLDocument& document)
	{
		std::size_t xmlStart = xml.find("<?xml");
		std::size_t xmlEnd = xml.rfind("\r\n");
		std::string xmlData = xml.substr(xmlStart, xmlEnd-xmlStart);
		if(document.Parse(xmlData.c_str()) != tinyxml2::XML_SUCCESS)
		{
			SHOOT_WARNING(false, "IsResultOK: Invalid XML data");
			Log << xmlData.c_str() << "\n";
			return false;
		}
		return true;
	}

	//! returns the platform string
	const char* ScoreoidUtils::GetPlatformString()
	{
#if SHOOT_PLATFORM == SHOOT_PLATFORM_IOS
		#ifdef UI_USER_INTERFACE_IDIOM
		if(UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) 
		{
			return "iPad";
		}
		#endif
		return "iPhone";
#elif SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
		return "Android";
#elif SHOOT_PLATFORM == SHOOT_PLATFORM_WP8
		return "WP8";
#else
		return "PC";
#endif
	}
}

