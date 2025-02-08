#include "glib/sound/Sound.h"
#include "glib/tween/TweenManager.h"

#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <vector>
#include <iostream>
#include <algorithm>

#include "glib/sound/effect/ReverbEffect.h"

extern unsigned int __glib_snd_get_effect_id(glib::SoundEffect* effect);

namespace glib
{
	class SoundImpl
	{
	private:
		ALuint m_Source;
		AudioDataSource* m_DataSource;
		SoundCategory* m_Category;
		float m_Volume;
		bool m_Started = false;
		float m_Length;
		bool m_Fading = false;
		TweenManager tw;
		Tween<float>* m_CurTween = nullptr;
	public:
		float m_GlobalVolume;
	public:
		SoundImpl(AudioDataSource* source, SoundCategory* category) : m_DataSource(source), m_Volume(1.0f), m_Category(category)
		{
			alGenSources(1, &m_Source);
			alSourcei(m_Source, AL_BUFFER, source->GetID());
			UpdateVolume();

			ALint sizeInBytes;
			ALint channels;
			ALint bits;

			alGetBufferi(source->GetID(), AL_SIZE, &sizeInBytes);
			alGetBufferi(source->GetID(), AL_CHANNELS, &channels);
			alGetBufferi(source->GetID(), AL_BITS, &bits);

			ALuint lengthInSamples = sizeInBytes * 8 / (channels * bits);
			m_Length = ((float)lengthInSamples / source->GetMetaData().sampleRate) * 1000.0f;
		}

		~SoundImpl()
		{
			Stop();
			alDeleteSources(1, &m_Source);
		}

		void Play()
		{
			m_Started = true;
			alSourcePlay(m_Source);
		}

		void Stop()
		{
			m_Started = false;
			alSourceStop(m_Source);
		}

		void Pause()
		{
			alSourcePause(m_Source);
		}

		void Resume()
		{
			alSourcePlay(m_Source);
		}

		float GetTimePosition()
		{
			float pos = (float)GetSampleOffset() / m_DataSource->GetMetaData().sampleRate;
			return pos * 1000.0f;
		}

		void SetLooping(bool looping)
		{
			alSourcei(m_Source, AL_LOOPING, looping);
		}

		void SetVolume(float volume)
		{
			m_Volume = volume;
			UpdateVolume();
		}

		void UpdateVolume()
		{
			alSourcef(m_Source, AL_GAIN, m_Volume * m_Category->volume * m_GlobalVolume);
		}

		void SetPitch(float pitch)
		{
			alSourcef(m_Source, AL_PITCH, pitch);
		}

		bool IsFinished()
		{
			ALint state = 0;
			alGetSourcei(m_Source, AL_SOURCE_STATE, &state);
			return m_Started && state != AL_PLAYING;
		}

		void SyncWith(Sound* snd)
		{
			alSourcei(m_Source, AL_SAMPLE_OFFSET, snd->impl->GetSampleOffset());
		}

		void SetTimePosition(float pos)
		{
			alSourcei(m_Source, AL_SAMPLE_OFFSET, (pos / 1000.0f) * m_DataSource->GetMetaData().sampleRate);
		}

		int GetSampleOffset()
		{
			int offset;
			alGetSourcei(m_Source, AL_SAMPLE_OFFSET, &offset);
			return offset;
		}

		float GetLength()
		{
			return m_Length;
		}

		void Fade(float time, float volume, Easing::EasingFunc func, float delay)
		{
			m_Fading = true;
			m_CurTween = tw.TweenFloat({ volume, &m_Volume }, time, func, delay);
		}

		void Update(float delta)
		{
			if (m_Fading)
			{
				tw.Update(delta);

				if (m_CurTween != nullptr)
				{
					if (m_CurTween->IsFinished())
					{
						m_CurTween = nullptr;
						m_Fading = false;
					}
				}

				UpdateVolume();
			}
		}
	};
}

using namespace glib;

void __glib_snd_update_volume(SoundImpl* snd, float globalVolume)
{
	snd->m_GlobalVolume = globalVolume;
	snd->UpdateVolume();
}

glib::Sound::Sound(AudioDataSource* source, SoundCategory* category)
{
	impl = new SoundImpl(source, category);
}

glib::Sound::~Sound()
{
	delete impl;
}

void glib::Sound::Play()
{
	impl->Play();
}

void glib::Sound::Stop()
{
	impl->Stop();
}

void glib::Sound::Pause()
{
	impl->Pause();
}

void glib::Sound::Resume()
{
	impl->Resume();
}

float glib::Sound::GetTimePosition()
{
	return impl->GetTimePosition();
}

float glib::Sound::GetLength()
{
	return impl->GetLength();
}

void glib::Sound::SetLooping(bool looping)
{
	impl->SetLooping(looping);
}

void glib::Sound::SetVolume(float volume)
{
	impl->SetVolume(volume);
}

void glib::Sound::SetPitch(float pitch)
{
	impl->SetPitch(pitch);
}

void glib::Sound::SyncWith(Sound* snd)
{
	impl->SyncWith(snd);
}

void glib::Sound::SetTimePosition(float pos)
{
	impl->SetTimePosition(pos);
}

void glib::Sound::FadeIn(float time, Easing::EasingFunc func, float delay)
{
	impl->Fade(time, 1.0f, func, delay);
}

void glib::Sound::FadeOut(float time, Easing::EasingFunc func, float delay)
{
	impl->Fade(time, 0.0f, func, delay);
}

void glib::Sound::Fade(float time, float volume, Easing::EasingFunc func, float delay)
{
	impl->Fade(time, volume, func, delay);
}

bool glib::Sound::IsFinished()
{
	return impl->IsFinished();
}

void glib::Sound::Update(float delta)
{
	impl->Update(delta);
}
