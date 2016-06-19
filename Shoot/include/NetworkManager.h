/* 

Amine Rehioui
Created: September 21st 2013

*/

#ifndef _NETWORK_MANAGER_H_INCLUDED_
#define _NETWORK_MANAGER_H_INCLUDED_

#include "NetResult.h"

namespace shoot
{	
	//! NetworkManager
	class NetworkManager : public Singleton<NetworkManager>
	{
		DECLARE_SINGLETON(NetworkManager);

	public:

		//! initializes the network manager
		void Init();

		static const u32 ReadBufferSize = 65536;

		//! sends data to a host
		void SendData(const char* strHostName, u32 port, u8* pData, u32 size, NetResultCallback* pResultCallback);
			
	private:

		Thread* m_pNetworkThread;
		bool m_bActive;
		bool m_bNetworkSupported;

		//! state
		enum E_State
		{
			S_Idle,
			S_Connecting,
			S_SendingData,
			S_ReceivingData
		};

		//! operation
		struct Operation
		{
			//! constructor
			Operation();
			
			//! releases operation resources
			void Release();

			s32 m_SocketID;
			std::string m_HostName;
			u32 m_Port;
			u8* m_pData;
			u32 m_DataSize;
			NetResultCallback* m_pResultCallback;
		};

		E_State m_eState;
		std::list<Operation> m_lOperations;
		Mutex* m_pMutex;
		u8 m_ReadBuffer[ReadBufferSize+1];

		//! remove HTTP markers
		void RemoveHTTPMarkers(const std::vector<u8>& dataIn, std::vector<u8>& dataOut);

		//! returns hexadecimal status
		bool IsHexadecimal(char c) const;

		//! called on a networkd error
		void OnError(const char* strError);

		//! Network thread
		static void* NetworkThread(void* pParam);
	};
}

#endif // _NETWORK_MANAGER_H_INCLUDED_


