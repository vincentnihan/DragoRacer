#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "Manager.h"
#include <string>
#include "fmod.hpp"
#include <vector>

class AudioSample
    {
    private:
        std::string name;
    
    public:
        FMOD::Sound  *sound;
        FMOD::Channel *channel;
        
    public:
        AudioSample(void);
        ~AudioSample(void);
        std::string getName() { return name; }
        void setName(std::string value) { name = value; }
    };

class AudioManager : public IManager
{
public:

	AudioManager(void);
	~AudioManager(void);

	FMOD::System* getSystem() { return system; }
        
    void Init();
	void Shutdown();

    void Update(float dt); //must be called once per frame
    AudioSample* Load(std::string filename, std::string name);
    AudioSample* Load(std::string filename);
    bool Play(std::string name);
    bool Play(AudioSample *sample);
    void Stop(std::string name);
	void Stop(AudioSample *sample);
    void StopAll();
    void StopAllExcept(std::string name);
        
    bool IsPlaying(std::string name);
    bool SampleExists(std::string name);
    AudioSample *FindSample(std::string name);

private:
		FMOD::System *system;
        typedef std::vector<AudioSample*> Samples;
        typedef std::vector<AudioSample*>::iterator SamplesIterator;
        Samples samples;

		static bool ERRCHECK(FMOD_RESULT result); 

protected:
};

#endif


