#include "glib/graphics/Sprite.h"
#include "glib/tween/Easing.h"
#include "glib/animation/AnimationManager.h"
#include "glib/graphics/camera/Camera.h"
#include "glib/Instance.h"
#include "glib/utils/glibGlobals.h"

namespace glib
{
	class SpriteImpl : public Animatable
	{
	private:
		Sprite* m_Sprite;
		AnimationManager m_AnimationManager;
		bool m_AutoAnimCenter;
		Axis m_AutoAnimCenterAxis;
		Vec2 m_AutoAnimCenterContainerSize;
		bool m_DoShake = false;
		float m_ShakeStrength = 0.0f;
		float m_ShakeDuration = 0.0f;
		float m_ShakeFade = 0.0f;
		Vec2 m_ShakeOffset;
		float m_ShakeFadeElapsed = 0.0f;
		float m_ShakeStrengthInitial = 0.0f;
	public:
		SpriteImpl(Sprite* s) : m_Sprite(s), m_AnimationManager(this)
		{
		}

		void Update(float delta)
		{
			if (m_DoShake)
			{
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
							m_AnimationManager.Update(delta);
							return;
						}
					}
					else
					{
						m_DoShake = false;
						m_ShakeOffset = GLIB_VEC2_ZERO;
						m_AnimationManager.Update(delta);
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
				m_Sprite->offset = m_ShakeOffset;
			}

 			m_AnimationManager.Update(delta);
		}

		void SetValues(const AnimValues& values) override
		{
			m_Sprite->tex = values.tex;
			m_Sprite->size = values.size;
			m_Sprite->size.x = values.uvSize.x * values.tex->width;
			m_Sprite->size.y = values.uvSize.y * values.tex->height;

			m_Sprite->offset = values.offset;

			if (m_Sprite->flipX)
			{
				m_Sprite->offset.x = -m_Sprite->offset.x;
			}
			if (m_Sprite->flipY)
			{
				m_Sprite->offset.y = -m_Sprite->offset.y;
			}
			m_Sprite->offset += m_ShakeOffset;
			m_Sprite->uvPos = values.uvPos;
			m_Sprite->uvSize = values.uvSize;
			m_Sprite->rotation = values.rotation;
			m_Sprite->pivotPoint = values.pivot;
			if (m_AutoAnimCenter)
			{
				m_Sprite->Center(m_AutoAnimCenterAxis, m_AutoAnimCenterContainerSize);
			}
		}

		const AnimValues GetValues() override
		{
			return {};
		}

		void AddAnimation(const std::string& name, Animation* animation)
		{
			m_AnimationManager.AddAnimation(name, animation);
		}

		void PlayAnimation(const std::string& animationName)
		{
			m_AnimationManager.PlayAnimation(animationName);
		}

		void AddAllAnimations(const std::map<std::string, Animation*> animations)
		{
			m_AnimationManager.AddAll(animations);
		}

		Animation* GetAnimation(const std::string& name)
		{
			return m_AnimationManager.GetAnimation(name);
		}

		void SetDefaultAnimation(Animation* animation)
		{
			m_AnimationManager.SetDefault(animation);
		}

		void SetDefaultAnimation(const std::string& name)
		{
			m_AnimationManager.SetDefault(name);
		}

		const Animation* GetCurrentAnimation() const
		{
			return m_AnimationManager.GetCurrentAnimation();
		}

		const std::string& GetCurrentAnimationName() const
		{
			return m_AnimationManager.GetCurrentAnimationName();
		}

		void Shake(float strength, float duration, float fadeDuration)
		{

		}

		void EnableAutoAnimationCenter(glib::Axis axis, const Vec2& containerSize)
		{
			m_AutoAnimCenter = true;
			m_AutoAnimCenterAxis = axis;
			m_AutoAnimCenterContainerSize = containerSize;
		}

		void DisableAutoAnimationCenter()
		{
			m_AutoAnimCenter = false;
		}
	};
}

using namespace glib;

glib::Sprite::Sprite() : overlay(false), scale(Vec2(1.0f, 1.0f)), tex(nullptr), color({ 1.0f, 1.0f, 1.0f, 1.0f }), scrollFactor(Vec2(1.0f, 1.0f)), uvPos(Vec2(0.0f, 0.0f)), uvSize(Vec2(1.0f, 1.0f)), pivotPoint(Vec2(0.5f, 0.5f)), spriteShader(nullptr), _drawDebugBox(false)
{
	impl = new SpriteImpl(this);
	visible = true;

	RegisterField(GLIB_FIELD_STRUCT(pos, VEC2));
	RegisterField(GLIB_FIELD_STRUCT(size, VEC2));
	RegisterField(GLIB_FIELD_STRUCT(scale, VEC2));
	RegisterField(GLIB_FIELD_STRUCT(offset, VEC2));
	RegisterField(GLIB_FIELD_STRUCT(scrollFactor, VEC2));
	RegisterField(GLIB_FIELD_STRUCT(color, COLOR));
	RegisterField(GLIB_FIELD_STRUCT(uvPos, VEC2));
	RegisterField(GLIB_FIELD_STRUCT(uvSize, VEC2));
	RegisterField(GLIB_FIELD_STRUCT(pivotPoint, VEC2));
	RegisterField(GLIB_FIELD_STRUCT(overlay, BOOL));
	RegisterField(GLIB_FIELD_STRUCT(_drawDebugBox, BOOL));
	RegisterField(GLIB_FIELD_STRUCT(tex, PTR));
	RegisterField(GLIB_FIELD_STRUCT(visible, BOOL));
	RegisterField(GLIB_FIELD_STRUCT(drawOrder, INT32));
	RegisterField(GLIB_FIELD_STRUCT(flipX, BOOL));
	RegisterField(GLIB_FIELD_STRUCT(flipY, BOOL));
}

glib::Sprite::Sprite(const Vec2& pos, Texture* tex) : Sprite()
{
	this->pos = pos;
	SetTexture(tex);
}

glib::Sprite::Sprite(const Vec2& pos, Texture* tex, const Vec2& scrollFactor) : Sprite(pos, tex)
{
	this->scrollFactor = scrollFactor;
}

glib::Sprite::Sprite(const Vec2& pos) : Sprite()
{
	this->pos = pos;
}

glib::Sprite::Sprite(const Vec2& pos, const Vec2& scrollFactor) : Sprite(pos)
{
	this->scrollFactor = scrollFactor;
}

glib::Sprite::~Sprite()
{
	delete impl;
}

void glib::Sprite::Center(const Axis& axis, const Vec2& containerSize)
{
	switch (axis)
	{
	case Axis::X:
	{
		pos.x = containerSize.x / 2.0f - size.x / 2.0f;
		break;
	}
	case Axis::Y:
	{
		pos.y = containerSize.y / 2.0f - size.y / 2.0f;
		break;
	}
	case Axis::XY:
	{
		pos.x = containerSize.x / 2.0f - size.x / 2.0f;
		pos.y = containerSize.y / 2.0f - size.y / 2.0f;
		break;
	}
	}
}

void glib::Sprite::SetTexture(Texture* tex)
{
	size = Vec2(tex->width, tex->height);
	this->tex = tex;
}

void glib::Sprite::Draw(const DrawData& data)
{
	Backend* backend = gInstance->GetBackend();
	Shader* shd = backend->GetSpriteShader();
	if (spriteShader != nullptr)
	{
		shd = spriteShader;
		shd->Use();
		Mat4 m = Mat4::Ortho(0.0f, data.wnd->GetInitialSize().x, data.wnd->GetInitialSize().y, 0.0f);
		shd->SetMat4("glib_projection_matrix", m);
	}

	Vec2 _pos = pos;
	_pos.x -= data.camera->pos.x * scrollFactor.x;
	_pos.y -= data.camera->pos.y * scrollFactor.y;

	Mat4 modelMat;
	Vec2 diff = Vec2((size.x * scale.x) - size.x, (size.y * scale.y) - size.y);

	modelMat.Translate(Vec2(_pos.x - (diff.x / 2.0f), _pos.y - (diff.y / 2.0f)));

	modelMat.Translate(Vec2(size.x * pivotPoint.x * scale.x, size.y * pivotPoint.y * scale.y));
	modelMat.Rotate(Vec3(0.0f, 0.0f, rotation));
	modelMat.Translate(Vec2(-(size.x * pivotPoint.x) * scale.x, -(size.y * pivotPoint.y) * scale.y));
	modelMat.Translate(offset);

	modelMat.Scale(Vec2(size.x + diff.x, size.y + diff.y));
	
	shd->Use();
	shd->SetMat4("glib_view_matrix", *data.matrix);
	shd->SetMat4("glib_model_matrix", modelMat);
	shd->SetColor("glib_color", color);
	shd->SetVec2("glib_uv_coord", uvPos);
	shd->SetVec2("glib_uv_size", uvSize);
	shd->SetInt("glib_flip_x", flipX);
	shd->SetInt("glib_flip_y", flipY);
	shd->SetInt("glib_solid_color", false);
	shd->SetInt("glib_overlay", overlay);

	if (tex != nullptr)
	{
		tex->Bind();
	}

	backend->DrawQuad();

	if (tex != nullptr)
	{
		tex->Unbind();
	}

	shd->SetInt("glib_solid_color", true);
	shd->SetColor("glib_color", GLIB_COLOR_WHITE);
	if (_drawDebugBox)
	{
		backend->DrawOutlinedQuad();
	}
}

void glib::Sprite::Update(float delta)
{
	impl->Update(delta);
	tw.Update(delta);
}

void glib::Sprite::Shake(float strength, float duration, float fadeDuration)
{
	impl->Shake(strength, duration, fadeDuration);
}

void glib::Sprite::EnableAutoAnimationCenter(glib::Axis axis, const Vec2& containerSize)
{
	impl->EnableAutoAnimationCenter(axis, containerSize);
}

void glib::Sprite::DisableAutoAnimationCenter()
{
	impl->DisableAutoAnimationCenter();
}

void glib::Sprite::AddAnimation(const std::string& name, Animation* animation)
{
	impl->AddAnimation(name, animation);
}

void glib::Sprite::PlayAnimation(const std::string& animationName)
{
	impl->PlayAnimation(animationName);
}

void glib::Sprite::AddAllAnimations(const std::map<std::string, Animation*> animations)
{
	impl->AddAllAnimations(animations);
}

Animation* glib::Sprite::GetAnimation(const std::string& name)
{
	return impl->GetAnimation(name);
}

void glib::Sprite::SetDefaultAnimation(Animation* animation)
{
	impl->SetDefaultAnimation(animation);
}

void glib::Sprite::SetDefaultAnimation(const std::string& name)
{
	impl->SetDefaultAnimation(name);
}

const Animation* glib::Sprite::GetCurrentAnimation() const
{
	return impl->GetCurrentAnimation();
}

const std::string& glib::Sprite::GetCurrentAnimationName() const
{
	return impl->GetCurrentAnimationName();
}
