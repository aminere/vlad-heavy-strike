/* 

Amine Rehioui
Created: May 3rd 2012

*/

#include "Shoot.h"

#include "Utils.h"

#include <string.h>

#include <ctype.h> // ::tolower

namespace shoot
{
	//! formats a resource path
	std::string Utils::FormatResourcePath(const std::string& path)
	{
		std::vector<std::string> tokens;
		Tokenize(path.c_str(), "/\\", tokens);
        while(std::find(tokens.begin(), tokens.end(), "..") != tokens.end())
        {
            std::vector<std::string> tokensOut;
            for(std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end();)
            {
                std::vector<std::string>::iterator nextToken = it + 1;
                if(nextToken == tokens.end() || (*nextToken) != "..")
                {
                    tokensOut.push_back((*it));
                    ++it;
                }
                else
                {
                    it += 2;
                }
            }           
            tokens = tokensOut;
        }
 
        std::string pathOut = tokens[0];
        for(u32 i=1; i<tokens.size(); ++i)
        {
            pathOut = pathOut + "/" + tokens[i];
        }
		return pathOut;
	}

	//! tokenizes a string
	void Utils::Tokenize(const char* pStr, const char* pDelim, std::vector<std::string>& tokens)
	{
		char aStr[1024];		
		strcpy(aStr, pStr);
		char* token = strtok(aStr, pDelim);
		while(token)
		{
			tokens.push_back(std::string(token));
			token = strtok(NULL, pDelim);
		}
	}

	//! returns an file extension from a file path
	std::string Utils::GetFileExtension(const char* strPath)
	{
		std::string path(strPath);
		s32 extIndex = path.rfind(".");
		SHOOT_ASSERT(extIndex >= 0, "Resource::GetFileExtension: invalid path");
		std::string extension = path.substr(extIndex+1);
		return extension;
	}

	//! string comparison
	bool Utils::Equals(const char* str1, const char* str2, bool bCaseSensitive /*= false*/)
	{
		std::string s1(str1), s2(str2);
		if(!bCaseSensitive)
		{
			std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
			std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
		}
		return s1 == s2;
	}
}
