#include "common/common.h"
#include "video/video.h"
#include "audio/audio.h"

void	init(int argc, char *argv[])
{
	common_init(argc, argv);
	script_load( basepath("constants.lua") );
	script_load( gamepath("config.lua") );
	video_init();
	audio_init();
	script_load( gamepath("game.lua") );
	script_call("main", "i", EVENT_INIT);
}

void	quit(int sig)
{
	if (sig != -1) {
		script_call("main", "i", EVENT_SHUTDOWN);
	}
	audio_shutdown();
	video_shutdown();
	common_shutdown();
	exit(0);
}

int	main(int argc, char *argv[])
{
	Uint32 start, elapsed, delay;
	extern var_t *c_fps;
	extern void input(), update();

	init(argc, argv);

	signal(SIGTERM, quit);
	signal(SIGINT, quit);

	for (;;) {
		start = SDL_GetTicks();

		input();
		update();

		delay = 1000/c_fps->integer;
		elapsed = SDL_GetTicks()-start;

		if (elapsed < delay) {
			SDL_Delay(delay-elapsed);
		}
	}

	
	return 0;
}

