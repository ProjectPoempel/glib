#include "glib/tween/TweenManager.h"

using namespace glib;

glib::TweenManager::TweenManager()
{
}

glib::TweenManager::~TweenManager()
{
}

void glib::TweenManager::Update(float delta)
{
	std::vector<int> toRemove;

	int i = 0;
	for (BaseTween* tw : m_Tweens)
	{
		tw->Update(delta);
		if (tw->IsFinished())
		{
			if (tw->m_OnComplete)
			{
				//tw->m_OnComplete();
			}
			toRemove.push_back(i);
		}

		i++;
	}

	for (int j : toRemove)
	{
		if (j >= m_Tweens.size()) continue;
		BaseTween* tw = m_Tweens.at(j);
		m_Tweens.erase(m_Tweens.begin() + j);
		delete tw;
	}

	if (m_Queue.size() > 0)
	{
		if (m_Current != nullptr)
		{
			if (m_Current->IsFinished())
			{
				m_Current = m_Queue.front();
				m_Queue.erase(m_Queue.begin());
				m_Current->Start();
			}
				
		}
		else
		{
			m_Current = m_Queue.front();
			m_Queue.erase(m_Queue.begin());
			m_Current->Start();
		}
	}

	if (m_Current != nullptr)
	{
		m_Current->Update(delta);
		if (m_Current->IsFinished())
		{
			if (m_Current->m_OnComplete)
			{
				//m_Current->m_OnComplete();
			}
			delete m_Current;
			m_Current = nullptr;
		}
	}
}

glib::Tween<float>* glib::TweenManager::QueueFloatTween(TweenData<float> data, float duration, Easing::EasingFunc easing, float delay)
{
	m_Queue.push_back(Tween<float>::Construct(data.to, data.ptr, duration, easing, delay));
	return (Tween<float>*) &m_Queue.back();
}

glib::Tween<Vec2>* glib::TweenManager::QueueVec2Tween(TweenData<Vec2> data, float duration, Easing::EasingFunc easing, float delay)
{
	m_Queue.push_back(Tween<Vec2>::Construct(data.to, data.ptr, duration, easing, delay));
	return (Tween<Vec2>*) &m_Queue.back();
}

glib::Tween<Vec3>* glib::TweenManager::QueueVec3Tween(TweenData<Vec3> data, float duration, Easing::EasingFunc easing, float delay)
{
	m_Queue.push_back(Tween<Vec3>::Construct(data.to, data.ptr, duration, easing, delay));
	return (Tween<Vec3>*) &m_Queue.back();
}

glib::Tween<Color>* glib::TweenManager::QueueColorTween(TweenData<Color> data, float duration, Easing::EasingFunc easing, float delay)
{
	m_Queue.push_back(Tween<Color>::Construct(data.to, data.ptr, duration, easing, delay));
	return (Tween<Color>*) &m_Queue.back();
}

void glib::TweenManager::CancelQueuedTweens()
{
	m_Queue.clear();
	m_Current = nullptr;
}

void glib::TweenManager::CancelTweens()
{
	m_Tweens.clear();
}

glib::Tween<float>* glib::TweenManager::TweenFloat(TweenData<float> data, float duration, Easing::EasingFunc easing, float delay)
{
	m_Tweens.push_back(Tween<float>::Construct(data.to, data.ptr, duration, easing, delay, true));
	return (Tween<float>*) &m_Tweens.back();
}

glib::Tween<Vec2>* glib::TweenManager::TweenVec2(TweenData<Vec2> data, float duration, Easing::EasingFunc easing, float delay)
{
	m_Tweens.push_back(Tween<Vec2>::Construct(data.to, data.ptr, duration, easing, delay, true));
	return (Tween<Vec2>*) &m_Tweens.back();
}

glib::Tween<Vec3>* glib::TweenManager::TweenVec3(TweenData<Vec3> data, float duration, Easing::EasingFunc easing, float delay)
{
	m_Tweens.push_back(Tween<Vec3>::Construct(data.to, data.ptr, duration, easing, delay, true));
	return (Tween<Vec3>*)& m_Tweens.back();
}

glib::Tween<Color>* glib::TweenManager::TweenColor(TweenData<Color> data, float duration, Easing::EasingFunc easing, float delay)
{
	m_Tweens.push_back(Tween<Color>::Construct(data.to, data.ptr, duration, easing, delay, true));
	return (Tween<Color>*) &m_Tweens.back();
}
