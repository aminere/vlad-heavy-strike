/* 

Amine Rehioui
Created: October 15th 2011

*/

#include "ShootEditorCommon.h"

#include "ShootTestEditor.h"

#include "ShootTestEditorRenderer.h"

namespace shoot
{
	//! constructor
	ShootTestEditor::ShootTestEditor()
	{
		EditorRenderer::OverrideInstance(new ShootTestEditorRenderer());
	}

	//! destructor
	ShootTestEditor::~ShootTestEditor()
	{
	}

	//! returns a menu category for a class type
	std::string ShootTestEditor::GetMenuCategory(const char* classType)
	{
		std::string category = ShootEditor::GetMenuCategory(classType);

		if(!category.empty())
		{
			return category;
		}
		else		
		{
			std::map<const char*, const char*> aCategoryMap;

			// Game
			const char* strGame = "Game";
			aCategoryMap["Actor"] = strGame;
			aCategoryMap["GameManager"] = strGame;
			aCategoryMap["AudioManager"] = strGame;
			aCategoryMap["Starfield"] = strGame;	
			aCategoryMap["Player"] = strGame;
			aCategoryMap["PlayerCamera"] = strGame;
			aCategoryMap["PlayerCollisionComponent"] = strGame;
			aCategoryMap["ConfigPoint"] = strGame;
			aCategoryMap["ItemManager"] = strGame;
			aCategoryMap["ConversationManager"] = strGame;
			aCategoryMap["Item"] = strGame;
			aCategoryMap["SFXManager"] = strGame;

			// UI
			const char* strUI = "UI";
			aCategoryMap["UIContext"] = strUI;
			aCategoryMap["PlayerList"] = strUI;
			aCategoryMap["VKButton"] = strUI;
			aCategoryMap["VKeyboard"] = strUI;

			// AI
			const char* strAI = "AI";
			aCategoryMap["AreaSpawner"] = strAI;
			aCategoryMap["TriggerSpawner"] = strAI;
			aCategoryMap["BasicEnemy"] = strAI;
			aCategoryMap["BasicShooter"] = strAI;
			aCategoryMap["Asteroid"] = strAI;
			aCategoryMap["FallingAsteroid"] = strAI;
			aCategoryMap["BasicTower"] = strAI;
			aCategoryMap["KamikazeTower"] = strAI;
			aCategoryMap["TightPassage"] = strAI;
			aCategoryMap["SpinningLasers"] = strAI;
			aCategoryMap["BasicFighter"] = strAI;
			aCategoryMap["AIManager"] = strAI;
			aCategoryMap["Boss1"] = strAI;
			aCategoryMap["Boss2"] = strAI;
			aCategoryMap["Boss3"] = strAI;
			aCategoryMap["Boss4"] = strAI;
			aCategoryMap["SpinShooter"] = strAI;
			aCategoryMap["RadialShooter"] = strAI;
			aCategoryMap["RadialTower"] = strAI;
			aCategoryMap["Kamikaze"] = strAI;
			aCategoryMap["Squad"] = strAI;
			aCategoryMap["RadialTank"] = strAI;
			aCategoryMap["LaserSwiper"] = strAI;
			aCategoryMap["Bomber"] = strAI;

			// Weapon
			const char* strWeapon = "Weapon";
			aCategoryMap["PlayerCannons"] = strWeapon;	
			aCategoryMap["PulseCannon"] = strWeapon;	
			aCategoryMap["BulletManager"] = strWeapon;
			aCategoryMap["PulseManager"] = strWeapon;
			aCategoryMap["PelletManager"] = strWeapon;			
			aCategoryMap["BulletManagerCollisionComponent"] = strWeapon;

			for(std::map<const char*, const char*>::iterator it = aCategoryMap.begin(); it != aCategoryMap.end(); ++it)
			{
				if(std::string((*it).first) == classType)
				{
					return (*it).second;
				}
			}

			return "";
		}
	}

	//! Selects an entity
	Entity* ShootTestEditor::SelectEntity(Entity* pEntity, const Vector3& vRayStart, const Vector3& vRayDir)
	{
		// setup Pin
		if(!pEntity && m_b3DControl)
		{
			Plane plane(Vector3::Create(0.0f, 0.0f, 1.0f), 0.0f);
			if(plane.IntersectWithRay(vRayStart, vRayDir, &m_vPinPosition))
			{
				SetHasPin(true);
			}
		}

		return super::SelectEntity(pEntity);
	}
}

