/* 

Amine Rehioui
Created: August 18th 2013

*/

#include "ShootTest.h"

#include "Bomber.h"

namespace shoot
{
	DEFINE_OBJECT(Bomber);
	DEFINE_OBJECT(BomberSettings);
	
	//! constructor
	BomberSettings::BomberSettings()
	{
	}

	//! serializes the entity to/from a PropertyStream
	void BomberSettings::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
	}

	//! constructor
	Bomber::Bomber()
	{
	}

	//! serializes the entity to/from a PropertyStream
	void Bomber::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
	}

	//! called during the initialization of the entity
	void Bomber::Init()
	{
		super::Init();

		if(BomberSettings* pSettings = static_cast<BomberSettings*>(m_Settings.Get()))
		{
			// TODO
		}

		if(Player* pPlayer = Player::Instance())
		{
			Vector3 vPosition = pPlayer->GetPosition() + GetSpawningPoint();
			SetAbsolutePosition(vPosition);
		}
	}

	//! called during the update of the entity	
	void Bomber::Update()
	{
		super::Update();

		if(m_HitPoints < 0)
		{
			return;
		}

		BomberSettings* pSettings = static_cast<BomberSettings*>(m_Settings.Get());

		// TODO
	}
}


