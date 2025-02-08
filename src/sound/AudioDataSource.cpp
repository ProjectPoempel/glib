#include "glib/sound/AudioDataSource.h"

#define AL_LIBTYPE_STATIC
#include <AL/al.h>

using namespace glib;

glib::AudioDataSource::AudioDataSource(unsigned int id, const AudioMetaData& data) : m_ID(id), m_Data(data)
{
}

glib::AudioDataSource::~AudioDataSource()
{
	alDeleteBuffers(1, &m_ID);
}

unsigned int glib::AudioDataSource::GetID()
{
	return m_ID;
}

AudioMetaData& glib::AudioDataSource::GetMetaData()
{
	return m_Data;
}
