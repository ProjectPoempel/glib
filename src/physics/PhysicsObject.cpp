#include "glib/physics/PhysicsObject.h"

#include "glib/physics/component/BoxCollider.h"

using namespace glib;

float glib::PhysicsObject::Distance(PhysicsObject* other)
{
	return pos.Distance(other->pos);
}

PhysicsComponent* glib::PhysicsObject::GetFirstComponent(PhysicsComponentType type)
{
	for (PhysicsComponent* c : components)
	{
		if (c->type == type)
		{
			return c;
		}
	}
	return nullptr;
}

bool glib::PhysicsObject::HasComponent(PhysicsComponentType type)
{
	return GetFirstComponent(type) != nullptr;
}

void glib::PhysicsObject::AddComponent(PhysicsComponent* component)
{
	if (HasComponent(component->type)) return;
	components.push_back(component);
}
