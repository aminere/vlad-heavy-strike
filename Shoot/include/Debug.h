/* 

Amine Rehioui
Created: March 18th 2010

*/

#ifndef _DEBUG_H_INCLUDED_
#define _DEBUG_H_INCLUDED_

#ifndef _RETAIL_
//! Assert with a formatted message macro
/** Takes a mandatory message argument */
#define SHOOT_ASSERT(condition, ...) shoot::Debug::Assert((condition) ? true : false, __FILE__, __LINE__, __VA_ARGS__);

//! Assert with a formatted message macro
/** Takes a mandatory message argument */
#define SHOOT_VERIFY(condition, ...) SHOOT_ASSERT(condition, __VA_ARGS__)

//! Warning with a formatted message macro
/** Takes a mandatory message argument */
#define SHOOT_WARNING(condition, ...) shoot::Debug::Warning((condition) ? true : false, __FILE__, __LINE__, __VA_ARGS__);

//! Log Warning with a formatted message macro
/** Takes a mandatory message argument */
#define SHOOT_LOG_WARNING(condition, ...) shoot::Debug::LogWarning((condition) ? true : false, __FILE__, __LINE__, __VA_ARGS__);
#else
#define SHOOT_ASSERT(condition, ...)
#define SHOOT_VERIFY(condition, ...) condition
#define SHOOT_WARNING(condition, ...) shoot::Debug::Warning((condition) ? true : false, __FILE__, __LINE__, __VA_ARGS__);
#define SHOOT_LOG_WARNING(condition, ...)
#endif // _RETAIL_

//! Debugging utilities
namespace shoot
{
	//! Debug class
	class Debug
	{
	public:

		//! Assert
		static void Assert(bool bCondition, const char* strFile, u32 line, const char* strFormat, ...);

		//! Warning
		static void Warning(bool bCondition, const char* strFile, u32 line, const char* strFormat, ...);

		//! LogWarning
		static void LogWarning(bool bCondition, const char* strFile, u32 line, const char* strFormat, ...);

		static const u32 MaxWarnings = 32;

	private:

		//! warning info
		struct WarningInfo
		{
			const char* strFile;
			u32 Line;
			bool bFree;
			bool bDisabled;

			//! constructor
			WarningInfo() : bFree(true), bDisabled(false)
			{
			}

			//! compare warning info
			bool operator == (const WarningInfo& other);
		};

		//! returns a WarningInfo index
		static s32 GetWarningIndex(const char* strFile, u32 line);

		static WarningInfo m_WarningInfos[MaxWarnings];		
	};	
}

#endif // _DEBUG_H_INCLUDED_


