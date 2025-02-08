#pragma once

#include "../DLLDefs.h"
#include "Sound.h"
#include "AudioDataSource.h"
#include "SoundCategory.h"

#include <string>

namespace glib
{
	class SoundManagerImpl;

	class SoundManager
	{
	private:
		SoundManagerImpl* impl;
	public:
		SoundManager(); // Internal
		~SoundManager(); // Internal
		
		GLIB_API void PlaySound(AudioDataSource* source, float volume = 1.0f);
		GLIB_API void PlaySound(const std::string& sourceName, float volume = 1.0f);
		GLIB_API void PlaySound(AudioDataSource* source, const std::string& categoryName);
		GLIB_API void PlaySound(const std::string& sourceName, const std::string& categoryName);
		GLIB_API Sound* CreateSound(AudioDataSource* source, float volume = 1.0f);
		GLIB_API Sound* CreateSound(const std::string& sourceName, float volume = 1.0f);
		GLIB_API Sound* CreateSound(AudioDataSource* source, const std::string& categoryName);
		GLIB_API Sound* CreateSound(const std::string& sourceName, const std::string& categoryName);
		
		/**
		* Loads an audio file and stores the data inside a AudioDataSource to be played by a Sound.
		* 
		* @param sourceName - The name for the source (if caching is enabled)
		* @param path - The path to the audio file (e.g. assets/sounds/MySound.ogg)
		* @param cache - Whether caching is enabled
		* 
		* @returns The AudioDataSource with the loaded audio data
		*/
		GLIB_API AudioDataSource* LoadAudio(const std::string& sourceName, const std::string& path, bool cache = true);

		/**
		* Loads an audio file from a package (apkg) and stores the data inside a AudioDataSource to be played by a Sound.
		*
		* @param sourceName - The name for the source (if caching is enabled)
		* @param packagePath - The path to the package (e.g. assets.apkg)
		* @param path - The path to the audio file (e.g. assets/sounds/MySound.ogg)
		* @param cache - Whether caching is enabled
		*
		* @returns The AudioDataSource with the loaded audio data
		*/
		GLIB_API AudioDataSource* LoadAudioFromPackage(const std::string& sourceName, const std::string& packagePath, const std::string& path, bool cache = true);
		
		GLIB_API void RemoveSourceFromCache(const std::string& sourceName, bool free = true);

		/**
		* DO NOT use this if you don't know what you're doing!
		*/
		GLIB_API void ChangeOutputDevice(const std::string& device); // !!! Invalidates all previously active or created sounds and loaded data! !!!
		
		/**
		* Sets the volume that affects all sounds.
		* 
		* @param volume - The new volume (from 0 to 1)
		*/
		GLIB_API void SetGlobalVolume(float volume);

		/**
		* 
		*/
		GLIB_API SoundCategory* CreateCategory(const std::string& name, float volume);
		GLIB_API SoundCategory* GetCategory(const std::string& name);
		GLIB_API void DeleteCategory(const std::string& name);

		void Update(float delta); // Internal
	};
}