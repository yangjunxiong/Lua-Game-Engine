#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"
#include "WorldState.h"

using namespace GameEngine;
using namespace std;

RTTI_DEFINITIONS(Sector);

Sector::Sector() : Attributed(Sector::TypeIdClass())
{
	assert(mDatumPointers[ENTITY_TABLE_INDEX]->first == ENTITY_TABLE_KEY);
}

const std::string& Sector::Name() const
{
	return mName;
}

void Sector::SetName(const std::string& name)
{
	mName = name;
}

void Sector::SetActive(bool active)
{
	mActive = active ? 1 : 0;
}

bool Sector::IsActive() const
{
	return mActive == 0 ? false : true;
}

Datum& Sector::Entities()
{
	return (*this)[ENTITY_TABLE_INDEX];
}

const Datum& Sector::Entities() const
{
	return (*this)[ENTITY_TABLE_INDEX];
}

Entity* Sector::CreateEntity(const std::string& className, const std::string& instanceName)
{
	Scope* product = Factory<Scope>::Create(className);
	assert(product != nullptr && product->Is("Entity"));
	Entity* entity = static_cast<Entity*>(product);
	entity->SetName(instanceName);
	entity->SetSector(*this);
	return entity;
}

void Sector::SetWorld(World& world)
{
	world.Adopt(*this, World::SECTOR_TABLE_KEY);
}

void Sector::Start(WorldState& state)
{
	Datum& entities = Entities();
	for (size_t i = 0; i < entities.Size(); ++i)
	{
		Scope* scope = &entities.AsTable(i);
		assert(scope->Is("Entity"));
		Entity* entity = static_cast<Entity*>(scope);
		entity->Start(state);
	}
}

void Sector::Update(WorldState& state)
{
	state.mSector = this;

	Datum& entities = Entities();
	for (size_t i = 0; i < entities.Size(); ++i)
	{
		Scope* scope = &entities.AsTable(i);
		assert(scope->Is("Entity"));
		Entity* entity = static_cast<Entity*>(scope);
		if (entity->IsActive())
		{
			entity->Update(state);
		}
	}
}

void Sector::Draw(const World::RenderFunction& func)
{
	Datum& entities = Entities();
	for (size_t i = 0; i < entities.Size(); ++i)
	{
		Scope* scope = &entities.AsTable(i);
		assert(scope->Is("Entity"));
		Entity* entity = static_cast<Entity*>(scope);
		if (entity->IsActive())
		{
			entity->Draw(func);
		}
	}
}

gsl::owner<Scope*> Sector::Clone() const
{
	return new Sector(*this);
}

const Vector<Attributed::Signature> Sector::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Name", Datum::DatumType::String, 1, offsetof(Sector, mName)),
		Signature("Active", Datum::DatumType::Integer, 1, offsetof(Sector, mActive)),
		Signature(ENTITY_TABLE_KEY, Datum::DatumType::Table, 0)
	});
}