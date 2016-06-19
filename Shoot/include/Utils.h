/* 

Amine Rehioui
Created: May 3rd 2012

*/

#ifndef _UTILS_H_INCLUDED_
#define _UTILS_H_INCLUDED_

namespace shoot
{
	// utils
	class Utils
	{
	public:

		//! formats a resource path
		static std::string FormatResourcePath(const std::string& path);

		//! tokenizes a string
		static void Tokenize(const char* pStr, const char* pDelim, std::vector<std::string>& tokens);

		//! returns an file extension from a file path
		static std::string GetFileExtension(const char* strPath);

		//! string comparison
		static bool Equals(const char* str1, const char* str2, bool bCaseSensitive = false);

		//! converts a native type to a string
		template <class T>
		static std::string ToString(T value)
		{
			std::stringstream ss(std::stringstream::in | std::stringstream::out);
			ss << value;
			std::string result = ss.str();
			return result;
		}

		//! converts a string to a native type
		template <class T>
		static T FromString(const std::string& str)
		{
			std::stringstream ss(std::stringstream::in | std::stringstream::out);
			ss << str;
			T t;
			ss >> t;
			return t;
		}
	};
}

#endif // _UTILS_H_INCLUDED_

