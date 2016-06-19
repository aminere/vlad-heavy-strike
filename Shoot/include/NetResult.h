/* 

Amine Rehioui
Created: September 21st 2013

*/

#ifndef _NET_RESULT_H_INCLUDED_
#define _NET_RESULT_H_INCLUDED_

namespace shoot
{	
	//! NetResult
	struct NetResult
	{
		//! Status
		enum E_Status
		{
			S_Undefined,
			S_ConnectionError,			
			S_DataReceived,
		};

		NetResult()
			: m_Status(S_Undefined)
		{
		}

		E_Status m_Status;
		std::vector<u8> m_Data;
	};

	//! net result callback
	typedef void NetResultCallback(NetResult& result);
}

#endif // _NET_RESULT_H_INCLUDED_


