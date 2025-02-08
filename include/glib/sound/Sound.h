#pragma once

#include "../DLLDefs.h"
#include "AudioDataSource.h"
#include "../tween/Easing.h"
#include "SoundCategory.h"

namespace glib
{
	class SoundImpl;
	class SoundManagerImpl;

	class Sound
	{
	private:
		SoundImpl* impl;
	public:
		Sound(AudioDataSource* source, SoundCategory* category);
		~Sound();

		GLIB_API void Play();
		GLIB_API void Stop();
		GLIB_API void Pause();
		GLIB_API void Resume();
		GLIB_API float GetTimePosition();
		GLIB_API float GetLength();
		GLIB_API void SetLooping(bool looping);
		GLIB_API void SetVolume(float volume);
		GLIB_API void SetPitch(float pitch);
		GLIB_API void SyncWith(Sound* snd);
		GLIB_API void SetTimePosition(float pos);
		GLIB_API void FadeIn(float time, Easing::EasingFunc func, float delay = 0.0f);
		GLIB_API void FadeOut(float time, Easing::EasingFunc func, float delay = 0.0f);
		GLIB_API void Fade(float time, float volume, Easing::EasingFunc func, float delay = 0.0f);
		GLIB_API bool IsFinished();

		/*
		GLIB_API SoundEffect* AddEffect(unsigned int type, unsigned int slot = -1);
		GLIB_API void RemoveEffect(unsigned int type, int index = -1);
		GLIB_API void RemoveEffect(SoundEffect* effect);
		GLIB_API SoundEffect* GetEffectFromType(unsigned int type, int rel_index = -1);
		*/

		void Update(float delta); // Internal

		friend class SoundManagerImpl;
		friend class SoundImpl;
	};
}