#include "audio.h"

var_t			*s_driver, *s_device, *s_freq, *s_format, *s_channels, *s_samples, *s_volume;
SDL_AudioDeviceID	dev;

SDL_AudioSpec audio_spec;

gameVar_t sound_vars[] = {
	{ &s_driver, "s_driver", "0" },
	{ &s_device, "s_device", "0" },
	{ &s_freq, "s_freq", "44100" },
	{ &s_format, "s_format", "16" },
	{ &s_channels, "s_channels", "2" },
	{ &s_samples, "s_samples", "512" },
	{ &s_volume, "s_volume", "1.0" },
	{ NULL, NULL, NULL }
};

void	audio_init()
{
	const char *device, *driver;
	SDL_AudioSpec desired;
	
	extern void audio_callback(void *, Uint8 *, int);

	var_load(sound_vars);

	if (!SDL_WasInit(SDL_INIT_AUDIO)) {
		if (SDL_InitSubSystem(SDL_INIT_AUDIO))
			ERROR("Failed to initialize audio: %s", SDL_GetError());
	}

	sound_init();

	device = SDL_GetAudioDeviceName(s_device->integer, 0);
	driver = SDL_GetAudioDriver(s_driver->integer);

	if (SDL_AudioInit(driver))
		ERROR("Failed to initialize audio driver: %s", SDL_GetError());

	memset(&desired, 0, sizeof(desired));
	desired.freq = s_freq->integer;
	switch (s_format->integer) {
	case 8: desired.format = AUDIO_U8; break;
	case 16: desired.format = AUDIO_S16SYS; break;
	case 32: desired.format = AUDIO_S32SYS; break;
	}
	desired.channels = s_channels->integer;
	desired.samples = s_samples->integer;
	desired.callback = audio_callback;

	dev = SDL_OpenAudioDevice(device, 0, &desired, &audio_spec, SDL_AUDIO_ALLOW_ANY_CHANGE);
	if (dev == 0)
		ERROR("Failed to open audio device: %s", SDL_GetError());	

	
	SDL_PauseAudioDevice(dev, 0);
}

void	audio_shutdown()
{
	SDL_PauseAudioDevice(dev, 1);
	SDL_CloseAudioDevice(dev);
	SDL_AudioQuit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);	
}

void	audio_lock()
{
	SDL_LockAudioDevice(dev);
}

void	audio_unlock()
{
	SDL_UnlockAudioDevice(dev);
}

void	audio_pause()
{
	audio_lock();
	SDL_PauseAudioDevice(dev, 1);
	audio_unlock();		
}

void	audio_resume()
{
	audio_lock();
	SDL_PauseAudioDevice(dev, 0);
	audio_unlock();
}

void	audio_stop()
{
	extern void channel_clear();

	audio_lock();
	channel_clear();
	audio_unlock();	
}
