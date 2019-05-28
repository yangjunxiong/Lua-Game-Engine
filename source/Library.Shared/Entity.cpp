#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "Action.h"
#include "WorldState.h"
//#include "ActionRender.h"

using namespace GameEngine;

RTTI_DEFINITIONS(Entity);

Entity::Entity(RTTI::IdType type) : Attributed(type)
{
	assert(mDatumPointers[ACTION_TABLE_INDEX]->first == ACTION_TABLE_KEY);
}

const std::string& Entity::Name() const
{
	return mName;
}

void Entity::SetName(const std::string& name)
{
	mName = name;
}

void Entity::SetSector(Sector& sector)
{
	sector.Adopt(*this, Sector::ENTITY_TABLE_KEY);
}

void Entity::Start(WorldState& state)
{
	// Because we might read transform from Json, and that doesn't trigger matrix calculation, we do it here
	mTransform.UpdateMatrix();

	Datum& actions = Actions();
	for (size_t i = 0; i < actions.Size(); ++i)
	{
		Scope& scope = actions.AsTable(i);
		assert(scope.Is(Action::TypeIdClass()));
		static_cast<Action&>(scope).Start(state);
	}
}

void Entity::Update(WorldState& state)
{
	state.mEntity = this;

	// Update actions
	Datum& actions = Actions();
	for (size_t i = 0; i < actions.Size(); ++i)
	{
		assert(actions.AsTable(i).Is(Action::TypeIdClass()));
		Action& action = static_cast<Action&>(actions.AsTable(i));
		action.Update(state);
	}
}

void Entity::Draw(const World::RenderFunction& func)
{
	Datum& actions = Actions();
	for (size_t i = 0; i < actions.Size(); ++i)
	{
		ActionRender* render = actions.AsTable(i).As<ActionRender>();
		if (render != nullptr)
		{
			func(mTransform, render->GetRenderParam());
		}
	}
}

gsl::owner<Scope*> Entity::Clone() const
{
	return new Entity(*this);
}

const Vector<Attributed::Signature> Entity::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Name", Datum::DatumType::String, 1, offsetof(Entity, mName)),
		Signature("Position", Datum::DatumType::Vector, 1, offsetof(Entity, mTransform.mPosition)),
		Signature("Rotation", Datum::DatumType::Vector, 1, offsetof(Entity, mTransform.mRotation)),
		Signature("Scale", Datum::DatumType::Vector, 1, offsetof(Entity, mTransform.mScale)),
		Signature("Active", Datum::DatumType::Integer, 1, offsetof(Entity, mActive)),
		Signature(ACTION_TABLE_KEY, Datum::DatumType::Table, 0)
	});
}

Datum& Entity::Actions()
{
	return (*this)[ACTION_TABLE_INDEX];
}

const Datum& Entity::Actions() const
{
	return (*this)[ACTION_TABLE_INDEX];
}

Transform& Entity::GetTransform()
{
	return mTransform;
}

const Transform& Entity::GetTransform() const
{
	return mTransform;
}

void Entity::SetTransform(const Transform& trans)
{
	mTransform = trans;
}

glm::vec4 Entity::GetPosition() const
{
	return mTransform.GetPosition();
}

glm::vec4 Entity::GetRotation() const
{
	return mTransform.GetRotation();
}

glm::vec4 Entity::GetScale() const
{
	return mTransform.GetScale();
}

void Entity::SetPosition(const glm::vec4& pos)
{
	mTransform.SetPosition(pos);
}

void Entity::SetRotation(const glm::vec4& rot)
{
	mTransform.SetRotation(rot);
}

void Entity::SetScale(const glm::vec4& scale)
{
	mTransform.SetScale(scale);
}

bool Entity::IsActive() const
{
	return mActive == 0 ? false : true;
}

void Entity::SetActive(bool active)
{
	mActive = active ? 1 : 0;
}

Action* Entity::CreateAction(std::string className, std::string instanceName)
{
	Scope* product = Factory<Scope>::Create(className);
	assert(product != nullptr && product->Is(Action::TypeIdClass()));
	Action* action = static_cast<Action*>(product);
	action->SetName(instanceName);
	Adopt(*action, ACTION_TABLE_KEY);
	return action;
}