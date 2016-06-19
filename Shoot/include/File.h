/* 

Amine Rehioui
Created: August 5th 2010

*/

#ifndef _FILE_H_INCLUDED_
#define _FILE_H_INCLUDED_

namespace shoot
{
	//! base file class
	class File
	{
	public:

		//! File handling mode
		enum E_Mode
		{
			M_Read,
			M_Write,
			M_ReadBinary,
			M_WriteBinary
		};

		//! creates a file based on the platform
		static File* Create(const char* strPath, E_Mode eMode);

		//! create a native file based on the platform
		static File* CreateNative(const char* strPath, E_Mode eMode);

		//! returns true if a file exists
		static bool Exists(const char* strPath);

		//! destructor
		virtual ~File()
		{
		}

		//! File offset types
		enum E_OffsetType
		{
			OT_Current = 0,
			OT_Start,
			OT_End
		};

		//! opens the file
		virtual bool Open(bool bAssertOnFailure = true) = 0;

		//! closes the file
		virtual void Close() = 0;

		//! reads data from the file
		virtual s32 Read(void* pDest, u32 bytesToRead) = 0;

		//! reads a string
		virtual s32 Read(std::string& dest, char delim1 = 0, char delim2 = 0);

		//! reads a line until a delimiter
		virtual s32 ReadLine(char* pDest, u32 numChars, char delim = '\n');

		//! writes data to the file
		virtual s32 Write(const void* pSrc, u32 bytesToWrite) = 0;

		//! peeks a character without advancing the file pointer
		virtual char Peek();

		//! ignores n characters or x characters until the delimiter is found
		virtual void Ignore(u32 numChars, char delimiter = ' ');
	
		//! changes the current read/write location in the file
		virtual void SetOffset(s32 offset, E_OffsetType eType = OT_Current) = 0;

		//! returns the current read/write location in the file
		virtual u32 GetOffset() = 0;

		//! returns true if end of file has been reached
		virtual bool EOFReached() = 0;

		// printing operators
		virtual File& operator << (const char* param) = 0;
		virtual File& operator << (s32 param) = 0;
		virtual File& operator << (f32 param) = 0;
		virtual File& operator << (char param) = 0;

		//! returns the file path
		const char* GetPath() const { return m_strPath.c_str(); }

		//! sets base path for all files
		inline static void SetBasePath(const char* strPath) { m_strBasePath = strPath; }

		//! gets the base path
		inline static std::string& GetBasePath() { return m_strBasePath; }

		//! sets the writable path
		inline static void SetWritablePath(const char* strPath) { m_strWritablePath = strPath; }

		//! sets the external writable path
		inline static void SetExternalWritablePath(const char* strPath) { m_strExternalWritablePath = strPath; }

		//! UseWritablePath
		struct UseWritablePath
		{
			UseWritablePath() {	File::ms_bUseWritablePath = true; }			
			~UseWritablePath() { File::ms_bUseWritablePath = false;	}
		};

		//! UseExternalWritablePath
		struct UseExternalWritablePath
		{
			UseExternalWritablePath() {	File::ms_bUseExternalWritablePath = true; }			
			~UseExternalWritablePath() { File::ms_bUseExternalWritablePath = false;	}
		};

	protected:

		//! constructor
		File(const char* strPath, E_Mode eMode);

		E_Mode m_eMode;
		std::string m_strPath;
		static std::string m_strBasePath;
		static std::string m_strWritablePath;
		static std::string m_strExternalWritablePath;
		static bool ms_bUseWritablePath;
		static bool ms_bUseExternalWritablePath;
	};
}

#endif // _FILE_H_INCLUDED_

