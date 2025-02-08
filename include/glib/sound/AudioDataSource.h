#pragma once

#include "../DLLDefs.h"

namespace glib
{
	struct AudioMetaData
	{
		float sampleRate;
	};

	class AudioDataSource
	{
	private:
		unsigned int m_ID;
		AudioMetaData m_Data;
	public:
		AudioDataSource(unsigned int id, const AudioMetaData& data);
		~AudioDataSource();

		GLIB_API unsigned int GetID();
		GLIB_API AudioMetaData& GetMetaData();
	};
}