#include "AudioManager.h"

AudioSample::AudioSample() : 
	sound(NULL), 
	channel(NULL)
{
}

AudioSample::~AudioSample()
{
	if (sound != NULL) {
		sound->release();
		sound = NULL;
	}
}

 bool AudioManager::ERRCHECK(FMOD_RESULT result)
 {
	if (result != FMOD_OK)
	{
		return false;
	}
	return true;
 }

AudioManager::AudioManager(void)
{
}


AudioManager::~AudioManager(void)
{
}

void AudioManager::Init()
{
	FMOD_RESULT       result;
	unsigned int      version;
	void             *extradriverdata = 0;

	//Common_Init(&extradriverdata);

	/*
	Create a System object and initialize
	*/
	result = FMOD::System_Create(&system);
	ERRCHECK(result);

	result = system->getVersion(&version);
	ERRCHECK(result);

	if (version < FMOD_VERSION)
	{
		//Common_Fatal("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
	}

	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
	ERRCHECK(result);
}

void AudioManager::Shutdown()
{

}

void AudioManager::Update(float dt)
{
	if(system)
		system->update();
}

AudioSample* AudioManager::Load(std::string filename)
{
	if (filename.length() == 0) 
		return nullptr;

	AudioSample *sample = Load(filename,filename);

	return sample;
}

AudioSample* AudioManager::Load(std::string filename, std::string name)
{
	if (filename.length() == 0 || name.length() == 0) 
		return false;


	AudioSample *sample = new AudioSample();

	FMOD_RESULT       result;
	result = system->createSound(filename.c_str(), FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &sample->sound);
	ERRCHECK(result);
	
	if(sample)
	{
		sample->setName(filename);
		samples.push_back(sample);
	}

	return sample;

}

bool AudioManager::SampleExists(std::string name)
{
	for (SamplesIterator i = samples.begin(); i != samples.end(); ++i)
	{
		if ((*i)->getName() == name)
		{
			return true;
		}         
	}

	return false;
}

bool AudioManager::IsPlaying(std::string name)
{
	AudioSample *sample = FindSample(name);
	if (sample == NULL || sample->channel == NULL) 
		return false;

	FMOD_RESULT       result;
	bool isPlaying;
	result = sample->channel->isPlaying(&isPlaying);
	ERRCHECK(result);

	return isPlaying;
}

AudioSample *AudioManager::FindSample(std::string name)
{
	AudioSample *sample = NULL;
	for (SamplesIterator i = samples.begin(); i != samples.end(); ++i)
	{
		if ((*i)->getName() == name)
		{
			sample = (*i);
			break;
		}
	}
	return sample;
}


bool AudioManager::Play(std::string name)
{
	AudioSample *sample = FindSample(name);
	return Play(sample);
}

bool AudioManager::Play(AudioSample *sample)
{
	if (sample && sample->sound) 
	{
		FMOD_RESULT       result;
		result = system->playSound(sample->sound, NULL, false, &sample->channel);
		ERRCHECK(result);
	}

	//FMOD::Channel_SetLoopCount(sample->channel, -1);
	//FMOD::Channel_SetPaused(sample->channel, false);

	return true;
}

void AudioManager::Stop(AudioSample *sample)
{
	if(sample && sample->channel)
		sample->channel->stop();
		
}

void AudioManager::Stop(std::string name)
{
	AudioSample *sample = FindSample(name);
	Stop(sample);
}

void AudioManager::StopAll()
{
	AudioSample *sample = NULL;
	for (SamplesIterator i = samples.begin(); i != samples.end(); ++i)
	{
		Stop(*i);
	}
}

void AudioManager::StopAllExcept(std::string name)
{
	AudioSample *sample = NULL;
	for (SamplesIterator i = samples.begin(); i != samples.end(); ++i)
	{
		if ((*i)->getName() != name)
		{
			Stop(*i);
		}
	}
}
