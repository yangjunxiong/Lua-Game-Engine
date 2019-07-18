#include "pch.h"
#include "World.h"
#include "Sector.h"
#include "WorldState.h"
#include "Action.h"
#include "Entity.h"
#include "LuaWrapper.h"

using namespace GameEngine;
using namespace std;

RTTI_DEFINITIONS(World);

World::World(const GameTime& time) :
	Attributed(World::TypeIdClass()),
	mState(time),
	mEventQueue(time)
{
	Append("Name").first = "";
	Append(SECTOR_TABLE_KEY);
	Append(ACTION_TABLE_KEY);
	Append(ENTITY_TABLE_KEY);
	//assert(mDatumPointers[SECTOR_TABLE_INDEX]->first == SECTOR_TABLE_KEY);
	//assert(mDatumPointers[ACTION_TABLE_INDEX]->first == ACTION_TABLE_KEY);
	//assert(mDatumPointers[ENTITY_TABLE_INDEX]->first == ENTITY_TABLE_KEY);
}

const std::string& World::Name() const
{
	return mName;
}

void World::SetName(const std::string& name)
{
	mName = name;
}

Datum& World::Sectors()
{
	return (*this)[SECTOR_TABLE_INDEX];
}

const Datum& World::Sectors() const
{
	return (*this)[SECTOR_TABLE_INDEX];
}

Sector* World::CreateSector(const std::string& name)
{
	Sector* sector = new Sector();
	sector->SetName(name);
	sector->SetWorld(*this);
	return sector;
}

void World::Destroy(Attributed& object)
{
	// Same object exists already
	if (mDestroyQueue.Find(&object) != mDestroyQueue.end())
	{
		return;
	}

	// Check for duplication or same ancestor tree
	for (auto& queueObject : mDestroyQueue)
	{
		if (queueObject->IsDescendentOf(object))
		{
			queueObject = nullptr;  // If new object is ancestor of existing one, don't need to destroy existing one anymore
		}
		else if (object.IsDescendentOf(*queueObject))
		{
			return;                 // If new object is descendent of existing one, just ignore this call
		}
	}

	mDestroyQueue.PushBack(&object);
}

void World::Start()
{
	Datum& sectors = Sectors();
	for (size_t i = 0; i < sectors.Size(); ++i)
	{
		Scope* scope = &sectors.AsTable(i);
		assert(scope->Is("Sector"));
		Sector* sector = static_cast<Sector*>(scope);
		sector->Start(mState);
	}

	Datum& entities = Entities();
	for (size_t i = 0; i < entities.Size(); ++i)
	{
		Scope* scope = &entities.AsTable(i);
		assert(scope->Is("Entity"));
		Entity* entity = static_cast<Entity*>(scope);
		entity->Start(mState);
	}
}

void World::Update()
{
	Update(mState);
}

void World::Update(WorldState& state)
{
	state.mWorld = this;

	// Update all sectors
	Datum& sectors = Sectors();
	for (size_t i = 0; i < sectors.Size(); ++i)
	{
		Scope* scope = &sectors.AsTable(i);
		assert(scope->Is("Sector"));
		Sector* sector = static_cast<Sector*>(scope);
		if (sector->IsActive())
		{
			sector->Update(state);
		}
	}

	// Update direct entities
	Datum& entities = Entities();
	for (size_t i = 0; i < entities.Size(); ++i)
	{
		assert(entities.AsTable(i).Is(Entity::TypeIdClass()));
		Entity& entity = static_cast<Entity&>(entities.AsTable(i));
		entity.Update(state);
	}

	// Update actions
	Datum& actions = Actions();
	for (size_t i = 0; i < actions.Size(); ++i)
	{
		assert(actions.AsTable(i).Is(Action::TypeIdClass()));
		Action& action = static_cast<Action&>(actions.AsTable(i));
		action.Update(state);
	}

	// Dispatch events
	mEventQueue.Update();

	// Destroy objects
	for (const auto& object : mDestroyQueue)
	{
		if (object != nullptr)
		{
			Scope* parent = object->GetParent();
			if (parent != nullptr)
			{
				parent->Orphan(*object);
			}
			delete object;
		}
	}
	mDestroyQueue.Clear();
}

void World::Draw()
{
	Datum& sectors = Sectors();
	for (size_t i = 0; i < sectors.Size(); ++i)
	{
		Scope* scope = &sectors.AsTable(i);
		assert(scope->Is("Sector"));
		Sector* sector = static_cast<Sector*>(scope);
		if (sector->IsActive())
		{
			sector->Draw();
		}
	}
}

void World::EnqueueEvent(const std::shared_ptr<BaseEvent>& event, const std::chrono::milliseconds& delay)
{
	mEventQueue.Enqueue(event, delay);
}

void World::DequeueEvent(const std::shared_ptr<BaseEvent>& event)
{
	mEventQueue.Dequeue(event);
}

const EventQueue& World::GetEventQueue() const
{
	return mEventQueue;
}

Datum& World::Actions()
{
	return (*this)[ACTION_TABLE_INDEX];
}

const Datum& World::Actions() const
{
	return (*this)[ACTION_TABLE_INDEX];
}

gsl::owner<Scope*> World::Clone() const
{
	return new World(*this);
}

Datum& World::Entities()
{
	return (*this)[ENTITY_TABLE_INDEX];
}

const Datum& World::Entities() const
{
	return (*this)[ENTITY_TABLE_INDEX];
}

const Vector<Attributed::Signature> World::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Name", Datum::DatumType::String, 1, offsetof(World, mName)),
		Signature(SECTOR_TABLE_KEY, Datum::DatumType::Table, 0),
		Signature(ACTION_TABLE_KEY, Datum::DatumType::Table, 0),
		Signature(ENTITY_TABLE_KEY, Datum::DatumType::Table, 0)
	});
}