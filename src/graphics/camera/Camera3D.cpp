#include "glib/graphics/camera/Camera3D.h"
#include "glib/math/MathFunctions.h"
#include "glib/window/Window.h"
#include "glib/event/EventManager.h"

#include <vector>

namespace glib
{
	class Camera3DImpl : public EventSubscriber
	{
	private:
		Camera3D* m_Camera;
		std::vector<Drawable*> m_Drawables;
		Vec2 m_InitialSize;
		bool m_Freelook = false;
		Vec2 m_FreelookPitch = Vec2(-89.0f, 89.0f);
		Vec2 m_FreelookYaw;
		bool m_FirstMouseMove = true;
		float m_LastX = 0.0f;
		float m_LastY = 0.0f;
		float m_FreelookSpeed = 0.1f;
		Window* m_Wnd;
	public:
		Camera3DImpl(Camera3D* camera, Vec2 initialSize, Window* wnd) : m_Camera(camera), m_InitialSize(initialSize), m_Wnd(wnd)
		{
			wnd->GetEventManager().Subscribe(GLIB_EVENT_MOUSE_MOVE, this);
		}

		~Camera3DImpl()
		{
			m_Wnd->GetEventManager().Unsubscribe(this);
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
				drawable->Update(delta);
			}
		}

		void SetFreelook(bool enabled)
		{
			m_Freelook = enabled;
			m_FirstMouseMove = true;
		}

		void SetFreelookConstraints(const Vec2& pitchMinMax, const Vec2& yawMinMax)
		{
			m_FreelookPitch = pitchMinMax;
			m_FreelookYaw = yawMinMax;
		}

		void OnMouseMove(const MouseMoveEvent& event) override
		{
			if (!m_Freelook) return;

			if (m_FirstMouseMove)
			{
				m_LastX = event.x;
				m_LastY = event.y;
				m_FirstMouseMove = false;
			}

			float offsetX = event.x - m_LastX;
			float offsetY = m_LastY - event.y;
			offsetX *= m_FreelookSpeed;
			offsetY *= m_FreelookSpeed;

			m_LastX = event.x;
			m_LastY = event.y;

			m_Camera->yaw -= offsetX;
			m_Camera->pitch += offsetY;

			if (m_FreelookPitch.x != 0.0f)
			{
				if (m_Camera->pitch >= m_FreelookPitch.y) m_Camera->pitch = m_FreelookPitch.y;
				if (m_Camera->pitch <= m_FreelookPitch.x) m_Camera->pitch = m_FreelookPitch.x;
			}
			if (m_FreelookYaw.x != 0.0f)
			{
				if (m_Camera->yaw >= m_FreelookYaw.y) m_Camera->yaw = m_FreelookYaw.y;
				if (m_Camera->yaw <= m_FreelookYaw.x) m_Camera->yaw = m_FreelookYaw.x;
			}
		}

		void SetFreelookSpeed(float speed)
		{
			m_FreelookSpeed = speed;
		}

		Mat4 CalculateView()
		{
			Vec3 _front;
			_front.x = cos(ToRadians(m_Camera->yaw)) * cos(ToRadians(m_Camera->pitch));
			_front.y = sin(ToRadians(m_Camera->pitch));
			_front.z = sin(ToRadians(m_Camera->yaw)) * cos(ToRadians(m_Camera->pitch));
			m_Camera->front = _front.Normalize();

			m_Camera->right = m_Camera->front.Cross(m_Camera->worldUp).Normalize();
			m_Camera->up = m_Camera->right.Cross(m_Camera->front).Normalize();

			Mat4 mat = Mat4::LookAt(m_Camera->pos, m_Camera->pos + m_Camera->front, m_Camera->up);

			mat.Rotate(Vec3(m_Camera->rotation.x, m_Camera->rotation.y, m_Camera->rotation.z));

			return mat;
		}
	};
}

using namespace glib;

glib::Camera3D::Camera3D(Vec2 initialSize, Window* wnd) : pos(Vec3(0.0f, 0.0f, 0.0f)), yaw(90.0f), pitch(0.0f)
{
	impl = new Camera3DImpl(this, initialSize, wnd);

	up = glib::Vec3(0.0f, 1.0f, 0.0f);
	worldUp = up;
	front = glib::Vec3(0.0f, 0.0f, -1.0f);
}

glib::Camera3D::~Camera3D()
{
	delete impl;
}

void glib::Camera3D::Add(Drawable* drawable)
{
	impl->Add(drawable);
}

void glib::Camera3D::Add(Drawable& drawable)
{
	impl->Add(drawable);
}

void glib::Camera3D::Remove(Drawable* drawable)
{
	impl->Remove(drawable);
}

void glib::Camera3D::Remove(Drawable& drawable)
{
	impl->Remove(&drawable);
}

void glib::Camera3D::RemoveAll()
{
	impl->RemoveAll();
}

std::vector<Drawable*>& glib::Camera3D::GetDrawables()
{
	return impl->GetDrawables();
}

Mat4 glib::Camera3D::CalculateView()
{
	return impl->CalculateView();
}

void glib::Camera3D::SetFreelook(bool enabled)
{
	impl->SetFreelook(enabled);
}

void glib::Camera3D::SetFreelookConstraints(const Vec2& pitchMinMax, const Vec2& yawMinMax)
{
	impl->SetFreelookConstraints(pitchMinMax, yawMinMax);
}

void glib::Camera3D::SetFreelookSpeed(float speed)
{
	impl->SetFreelookSpeed(speed);
}

void glib::Camera3D::Update(float delta)
{
	impl->Update(delta);
	tw.Update(delta);
}
