#include "glib/sound/SoundManager.h"

#include "glib/utils/AudioFileReader.h"
#include "glib/glibError.h"

#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>

#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alc.h>
#define AL_ALEXT_PROTOTYPES
#include <AL/alext.h>

extern void __glib_snd_update_volume(glib::SoundImpl* snd, float globalVolume);

ALCcontext* ALCCONTEXT;

namespace glib
{
	class SoundManagerImpl
	{
	private:
		// np = non persistent (aka the sounds are deleted after they finished playing)
		std::vector<Sound*> m_npSounds;
		// p = persistent
		std::vector<Sound*> m_pSounds;
		std::unordered_map<std::string, AudioDataSource*> m_Sources;
		std::unordered_map<std::string, SoundCategory*> m_Categories;

		ALCdevice* m_Device;
		ALCcontext* m_Context;
		float m_Volume;
	public:
		SoundManagerImpl() : m_Device(nullptr), m_Context(nullptr), m_Volume(1.0f)
		{
			CreateCategory("default", 1.0f);
		}

		~SoundManagerImpl()
		{
			for (Sound* snd : m_npSounds)
			{
				delete snd;
			}
			for (Sound* snd : m_pSounds)
			{
				delete snd;
			}
			for (const auto& v : m_Sources)
			{
				delete v.second;
			}
			for (const auto& v : m_Categories)
			{
				delete v.second;
			}

			alcMakeContextCurrent(m_Context);
			alcCloseDevice(m_Device);
			alcDestroyContext(m_Context);
		}

		AudioDataSource* GetDataSource(const std::string& name) const
		{
			if (m_Sources.count(name) < 1) return nullptr;
			return m_Sources.at(name);
		}

		ALuint GenerateAndLoadALBuffer(const AudioData& data)
		{
			ALuint buf;
			alGenBuffers(1, &buf);

			ALenum format = AL_INVALID_ENUM;
			if (data.depth == 8)
			{
				if (data.channels == 1)
				{
					format = AL_FORMAT_MONO8;
				}
				else if (data.channels == 2)
				{
					format = AL_FORMAT_STEREO8;
				}
			}
			else if (data.depth == 16)
			{
				if (data.channels == 1)
				{
					format = AL_FORMAT_MONO16;
				}
				else if (data.channels == 2)
				{
					format = AL_FORMAT_STEREO16;
				}
			}

			alBufferData(buf, format, data.buf, data.size, data.sampleRate);

			return buf;
		}

		AudioDataSource* LoadAudio(const std::string& name, const std::string& path)
		{
			if (m_Sources.count(name) > 0) return m_Sources.at(name);

			AudioData data = AudioFileReader::ReadFile(path);
			if (data.buf == nullptr)
			{
				if (data.depth == 1000)
				{
					Error::Throw(GLIB_SOUND_UNSUPPORTED_FORMAT, "Unsupported format: " + path);
				}
				else if (data.depth == 3000)
				{
					Error::Throw(GLIB_SOUND_FILE_OPEN_FAIL, "Failed to open: " + path);
				}
				return nullptr;
			}

			ALuint buf = GenerateAndLoadALBuffer(data);
			delete[] data.buf;

			AudioMetaData meta{};
			meta.sampleRate = data.sampleRate;
			
			AudioDataSource* source = new AudioDataSource(buf, meta);
			m_Sources.insert({ name, source });

			return source;
		}

		AudioDataSource* LoadAudioFromPackage(const std::string& name, const std::string& packagePath, const std::string& path)
		{
			AudioData data = AudioFileReader::ReadPackage(packagePath, path);
			if (data.buf == nullptr)
			{
				if (data.depth == 1000)
				{
					Error::Throw(GLIB_SOUND_UNSUPPORTED_FORMAT, "Unsupported format: " + path);
				}
				else if (data.depth == 3000)
				{
					Error::Throw(GLIB_SOUND_FILE_OPEN_FAIL, "Failed to open: " + path);
				}
				return nullptr;
			}

			ALuint buf = GenerateAndLoadALBuffer(data);
			delete[] data.buf;

			AudioMetaData meta{};
			meta.sampleRate = data.sampleRate;

			AudioDataSource* source = new AudioDataSource(buf, meta);
			m_Sources.insert({ name, source });

			return source;
		}

		void ChangeOutputDevice(const std::string& device)
		{
			if (m_Context != nullptr && m_Device != nullptr)
			{
				alcMakeContextCurrent(m_Context);
				alcCloseDevice(m_Device);
				alcDestroyContext(m_Context);
			}

			if (device.empty())
			{
				m_Device = alcOpenDevice(nullptr);
			}
			else
			{
				m_Device = alcOpenDevice(device.c_str());
			}

			if (alcIsExtensionPresent(m_Device, "ALC_EXT_EFX") == AL_FALSE)
			{
				Error::Throw(GLIB_SOUND_OPENAL_EXT_EFX_NOT_SUPPORTED, "Device does not support OpenAL Effects!");
				alcCloseDevice(m_Device);
				return;
			}
			
			m_Context = alcCreateContext(m_Device, nullptr);
			alcMakeContextCurrent(m_Context);
			ALCCONTEXT = m_Context;
		}

		void Update(float delta)
		{
			for (Sound* snd : m_pSounds)
			{
				snd->Update(delta);
			}

			for (Sound* snd : m_npSounds)
			{
				snd->Update(delta);
			}

			std::vector<Sound*> toRemove;
			for (Sound* snd : m_npSounds)
			{
				if (snd->IsFinished())
				{
					toRemove.push_back(snd);
				}
			}

			for (Sound* snd : toRemove)
			{
				m_npSounds.erase(std::find(m_npSounds.begin(), m_npSounds.end(), snd));
				delete snd;
			}
		}

		void SetGlobalVolume(float volume)
		{
			m_Volume = volume;
			for (Sound* snd : m_npSounds)
			{
				__glib_snd_update_volume(snd->impl, m_Volume);
			}
			for (Sound* snd : m_pSounds)
			{
				__glib_snd_update_volume(snd->impl, m_Volume);
			}
		}

		SoundCategory* CreateCategory(const std::string& name, float volume)
		{
			if (m_Categories.contains(name)) return m_Categories.at(name);
			
			SoundCategory* cat = new SoundCategory;
			cat->volume = volume;
			m_Categories.insert({ name, cat });

			return cat;
		}

		void DeleteCategory(const std::string& name)
		{
			if (!m_Categories.contains(name)) return;
			delete m_Categories.at(name);
			m_Categories.erase(name);
		}

		SoundCategory* GetCategory(const std::string& name)
		{
			if (!m_Categories.contains(name))
			{
				Error::Throw(GLIB_ERROR_NULLPTR, "Category doesn't exist: " + name, "SoundManager", "GetCategory");
				return nullptr;
			}
			return m_Categories.at(name);
		}

		Sound* MakeSound(AudioDataSource* source, SoundCategory* category)
		{
			Sound* snd = new Sound(source, category);
			__glib_snd_update_volume(snd->impl, m_Volume);
			return snd;
		}

		void PlayNPSound(AudioDataSource* source, SoundCategory* category, float volume = 1.0f)
		{
			Sound* snd = MakeSound(source, category);
			snd->SetVolume(volume);
			m_npSounds.push_back(snd);
			snd->Play();
		}

		Sound* CreatePSound(AudioDataSource* source, SoundCategory* category, float volume = 1.0f)
		{
			Sound* snd = MakeSound(source, category);
			snd->SetVolume(volume);
			m_pSounds.push_back(snd);
			return snd;
		}

		AudioDataSource* GetSourceFromName(const std::string& name)
		{
			if (!m_Sources.contains(name))
			{
				Error::Throw(GLIB_ERROR_NULLPTR, "Source not loaded: " + name, "SoundManager", "GetSourceFromName");
				return nullptr;
			}
			return m_Sources.at(name);
		}

		void RemoveSourceFromCache(const std::string& sourceName, bool free)
		{
			if (!m_Sources.contains(sourceName)) return;
			if (free)
			{
				delete m_Sources.at(sourceName);
			}
			m_Sources.erase(sourceName);
		}
	};
}

using namespace glib;

glib::SoundManager::SoundManager()
{
	impl = new SoundManagerImpl;
}

glib::SoundManager::~SoundManager()
{
	delete impl;
}

void glib::SoundManager::PlaySound(AudioDataSource* source, float volume)
{
	impl->PlayNPSound(source, impl->GetCategory("default"), volume);
}

void glib::SoundManager::PlaySound(const std::string& sourceName, float volume)
{
	impl->PlayNPSound(impl->GetSourceFromName(sourceName), impl->GetCategory("default"), volume);
}

void glib::SoundManager::PlaySound(AudioDataSource* source, const std::string& categoryName)
{
	impl->PlayNPSound(source, impl->GetCategory(categoryName), 1.0f);
}

void glib::SoundManager::PlaySound(const std::string& sourceName, const std::string& categoryName)
{
	impl->PlayNPSound(impl->GetSourceFromName(sourceName), impl->GetCategory(categoryName), 1.0f);
}

Sound* glib::SoundManager::CreateSound(AudioDataSource* source, float volume)
{
	return impl->CreatePSound(source, impl->GetCategory("default"), volume);
}

Sound* glib::SoundManager::CreateSound(const std::string& sourceName, float volume)
{
	return impl->CreatePSound(impl->GetSourceFromName(sourceName), impl->GetCategory("default"), volume);
}

Sound* glib::SoundManager::CreateSound(AudioDataSource* source, const std::string& categoryName)
{
	return impl->CreatePSound(source, impl->GetCategory(categoryName), 1.0f);
}

Sound* glib::SoundManager::CreateSound(const std::string& sourceName, const std::string& categoryName)
{
	return impl->CreatePSound(impl->GetSourceFromName(sourceName), impl->GetCategory(categoryName), 1.0f);
}

AudioDataSource* glib::SoundManager::LoadAudio(const std::string& name, const std::string& path, bool cache)
{
	return impl->LoadAudio(name, path);
}

AudioDataSource* glib::SoundManager::LoadAudioFromPackage(const std::string& name, const std::string& packagePath, const std::string& path, bool cache)
{
	return impl->LoadAudioFromPackage(name, packagePath, path);
}

void glib::SoundManager::RemoveSourceFromCache(const std::string& sourceName, bool free)
{
	impl->RemoveSourceFromCache(sourceName, free);
}

void glib::SoundManager::ChangeOutputDevice(const std::string& device)
{
	impl->ChangeOutputDevice(device);
}

void glib::SoundManager::SetGlobalVolume(float volume)
{
	impl->SetGlobalVolume(volume);
}

SoundCategory* glib::SoundManager::CreateCategory(const std::string& name, float volume)
{
	return impl->CreateCategory(name, volume);
}

SoundCategory* glib::SoundManager::GetCategory(const std::string& name)
{
	return impl->GetCategory(name);
}

void glib::SoundManager::DeleteCategory(const std::string& name)
{
	impl->DeleteCategory(name);
}

void glib::SoundManager::Update(float delta)
{
	impl->Update(delta);
}
