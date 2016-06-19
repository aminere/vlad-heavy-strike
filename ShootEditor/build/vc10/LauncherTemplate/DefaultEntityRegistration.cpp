/* 

Amine Rehioui
Created: April 4th 2010

*/

#include "Types.h"
#include "EntityFactory.h"

// Core
#include "Context.h"

// 2D
#include "Entity2D.h"
#include "Image.h"
#include "Sprite.h"
#include "Text.h"
#include "Camera2D.h"

// 3D
#include "Entity3D.h"
#include "Camera3D.h"
#include "MeshEntity.h"
#include "ParticleGenerator.h"
#include "BillBoardEntity.h"
#include "SkyBoxEntity.h"

//! Entity registration function
void RegisterEntities()
{
	// Core
	shoot::EntityFactory::Instance()->Register<shoot::Entity>();
	shoot::EntityFactory::Instance()->Register<shoot::Context>();

	// 2D
	shoot::EntityFactory::Instance()->Register<shoot::Entity2D>();
	shoot::EntityFactory::Instance()->Register<shoot::Image>();
	shoot::EntityFactory::Instance()->Register<shoot::Sprite>();
	shoot::EntityFactory::Instance()->Register<shoot::Text>();
	shoot::EntityFactory::Instance()->Register<shoot::Camera2D>();

	// 3D
	shoot::EntityFactory::Instance()->Register<shoot::Entity3D>();
	shoot::EntityFactory::Instance()->Register<shoot::Camera3D>();
	shoot::EntityFactory::Instance()->Register<shoot::MeshEntity>();
	shoot::EntityFactory::Instance()->Register<shoot::ParticleGenerator>();
	shoot::EntityFactory::Instance()->Register<shoot::BillBoardEntity>();
	shoot::EntityFactory::Instance()->Register<shoot::SkyBoxEntity>();
}
