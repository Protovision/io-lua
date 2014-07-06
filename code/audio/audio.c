#include "audio.h"

var_t			*s_driver, *s_device, *s_freq, *s_format, *s_channels, *s_samples, *s_volume, *s_mute;
SDL_AudioDeviceID	dev;

SDL_AudioSpec audio_spec;

gameVar_t sound_vars[] = {
	{ &s_device, "s_device", "0" },
	{ &s_freq, "s_freq", "44100" },
	{ &s_format, "s_format", "16" },
	{ &s_channels, "s_channels", "2" },
	{ &s_samples, "s_samples", "512" },
	{ &s_volume, "s_volume", "1.0" },
	{ &s_mute, "s_mute", "0" },
	{ NULL, NULL, NULL }
};

void	audio_init()
{
	const char *device, *driver;
	SDL_AudioSpec desired;
	
	extern void audio_callback(void *, Uint8 *, int);

	var_load(sound_vars);

	s_driver = var_get("s_driver");

	if (s_driver == NULL) {	
		if (strcmp(platform, "Windows") == 0) {
			s_driver = var_set("s_driver", "2");
		} else if (
			strcmp(platform, "Mac OS X") == 0 ||
			strcmp(platform, "Linux") == 0 ||
			strcmp(platform, "iOS") == 0
		) {
			s_driver = var_set("s_driver", "0");
		}
	}

	if (!SDL_WasInit(SDL_INIT_AUDIO)) {
		if (SDL_InitSubSystem(SDL_INIT_AUDIO))
			FATAL(SDL_GetError());
	}

	Mix_Init(MIX_INIT_OGG);

//	sound_init();

	device = SDL_GetAudioDeviceName(s_device->integer, 0);
	driver = SDL_GetAudioDriver(s_driver->integer);

	if (SDL_AudioInit(driver))
		FATAL(SDL_GetError());

	memset(&desired, 0, sizeof(desired));
	desired.freq = s_freq->integer;
	switch (s_format->integer) {
	case 8: desired.format = AUDIO_U8; break;
	case 16: desired.format = AUDIO_S16SYS; break;
	case 32: desired.format = AUDIO_S32SYS; break;
	}
	desired.channels = s_channels->integer;
	desired.samples = s_samples->integer;
	//desired.callback = audio_callback;

	if (Mix_OpenAudio(desired.freq, desired.format, desired.channels, desired.samples))
		FATAL(Mix_GetError());

	if (Mix_AllocateChannels(16) < 16)
		FATAL(Mix_GetError());

	Mix_Resume(-1);
}

void	audio_shutdown()
{
	Mix_Pause(-1);
	Mix_CloseAudio();
	SDL_AudioQuit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);	
}

static int old_volume;

void	audio_mute()
{
	old_volume = audio_get_volume();
	audio_set_volume(0);
}

void	audio_unmute()
{
	audio_set_volume(old_volume);
}
