#include "glib/graphics/camera/Camera.h"

#include <vector>
#include <random>

namespace glib
{
	class CameraImpl
	{
	private:
		Camera* m_Camera;
		std::vector<Drawable*> m_Drawables;
		Vec2 m_InitialSize;
		bool m_DoShake = false;
		float m_ShakeStrength = 0.0f;
		float m_ShakeDuration = 0.0f;
		float m_ShakeFade = 0.0f;
		Vec2 m_ShakeOffset;
		float m_ShakeFadeElapsed = 0.0f;
		float m_ShakeStrengthInitial = 0.0f;
	public:
		CameraImpl(Camera* camera, Vec2 initialSize) : m_Camera(camera), m_InitialSize(initialSize)
		{
		}

		~CameraImpl()
		{
		}

		void Add(Drawable* drawable)
		{
			m_Drawables.push_back(drawable);
		}

		void Add(Drawable& drawable)
		{
			m_Drawables.push_back(&drawable);
		}

		void Remove(Drawable* drawable)
		{
			std::vector<Drawable*>::iterator it = std::find(m_Drawables.begin(), m_Drawables.end(), drawable);
			if (it == m_Drawables.end()) return;
			m_Drawables.erase(it);
		}

		std::vector<Drawable*>& GetDrawables()
		{
			return m_Drawables;
		}

		void RemoveAll()
		{
			m_Drawables.clear();
		}

		void Update(float delta)
		{
			for (Drawable* drawable : m_Drawables)
			{
				try
				{
					drawable->Update(delta);
				}
				catch (...)
				{
					std::cout << "glib Error: caught unknown exception in Camera::Update()" << std::endl;
				}
			}

			if (!m_DoShake) return;
			m_ShakeDuration -= delta;
			if (m_ShakeDuration <= 0.0f)
			{
				if (m_ShakeFadeElapsed > 0.0f)
				{
					m_ShakeFadeElapsed += delta;
					float prog = std::fminf(m_ShakeFadeElapsed / m_ShakeFade, 1.0f);
					m_ShakeStrength = m_ShakeStrengthInitial + (0.0f - m_ShakeStrengthInitial) * (delta / 1000.0f);
					if (m_ShakeStrength <= 0.0f)
					{
						m_DoShake = false;
						return;
					}
				}
				else
				{
					m_DoShake = false;
					m_ShakeOffset = GLIB_VEC2_ZERO;
					return;
				}
			}

			bool negX = (std::rand() % 2) - 1;
			bool negY = (std::rand() % 2) - 1;

			float x = std::rand() % 30;
			float y = std::rand() % 30;
			x *= m_ShakeStrength;
			y *= m_ShakeStrength;

			if (negX) x = -x;
			if (negY) y = -y;

			m_ShakeOffset.x = x;
			m_ShakeOffset.y = y;
		}

		Mat4 CalculateView()
		{
			Mat4 m;
			Vec2 diff = Vec2((m_InitialSize.x * m_Camera->zoom) - m_InitialSize.x, (m_InitialSize.y * m_Camera->zoom) - m_InitialSize.y);

			m.Translate(m_ShakeOffset);

			m.Translate(Vec2(m_InitialSize.x / 2.0f * m_Camera->zoom, m_InitialSize.y / 2.0f * m_Camera->zoom));
			m.Rotate(Vec3(0.0f, 0.0f, m_Camera->rotation));
			m.Translate(Vec2(-(m_InitialSize.x / 2.0f) * m_Camera->zoom, -(m_InitialSize.y / 2.0f) * m_Camera->zoom));

			m.Translate(Vec2(-diff.x / 2.0f, -diff.y / 2.0f));
			m.Scale(Vec2(m_Camera->zoom, m_Camera->zoom));

			return m;
		}

		void Shake(float strength, float duration, float fadeDuration)
		{
			m_DoShake = true;
			m_ShakeStrength = strength;
			m_ShakeDuration = duration;
			m_ShakeFade = fadeDuration;
			m_ShakeStrengthInitial = m_ShakeStrength;
			m_ShakeFadeElapsed = 0.0f;
		}

		void Organize()
		{
			std::vector<Drawable*> sorted;
			std::vector<Drawable*> sorted1;
			for (Drawable* d : m_Drawables)
			{
				if (d->drawOrder < 0)
				{
					sorted1.push_back(d);
				}
				else
				{
					sorted.push_back(d);
				}
			}

			std::sort(sorted.begin(), sorted.end(), [](Drawable* d1, Drawable* d2) { return d1->drawOrder > d2->drawOrder; });

			for (Drawable* d : sorted) sorted1.push_back(d);
			m_Drawables = sorted1;
		}
	};
}

using namespace glib;

glib::Camera::Camera(Vec2 initialSize) : pos(Vec2(0.0f, 0.0f)), zoom(1.0f), rotation(0.0f), canDraw(true), canUpdate(true), clipping(false)
{
	impl = new CameraImpl(this, initialSize);
}

glib::Camera::~Camera()
{
	delete impl;
}

void glib::Camera::Add(Drawable* drawable)
{
	impl->Add(drawable);
}

void glib::Camera::Add(Drawable& drawable)
{
	impl->Add(drawable);
}

void glib::Camera::Remove(Drawable* drawable)
{
	impl->Remove(drawable);
}

void glib::Camera::Remove(Drawable& drawable)
{
	impl->Remove(&drawable);
}

void glib::Camera::RemoveAll()
{
	impl->RemoveAll();
}

std::vector<Drawable*>& glib::Camera::GetDrawables()
{
	return impl->GetDrawables();
}

Mat4 glib::Camera::CalculateView()
{
	return impl->CalculateView();
}

void glib::Camera::Shake(float strength, float duration, float fadeDuration)
{
	impl->Shake(strength, duration, fadeDuration);
}

void glib::Camera::Organize()
{
	impl->Organize();
}

void glib::Camera::Update(float delta)
{
	if (!canUpdate) return;
	impl->Update(delta);
	tw.Update(delta);
}
