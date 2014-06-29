#include "common.h"
#include "video.h"
#include "audio.h"

void	init(int argc, char *argv[])
{
	common_init(argc, argv);

	if (unz_exists("config.lua")) {
		script_load("config.lua");
	}

	audio_init();
	video_init();
	base_init();
	script_load("game.lua");
	script_call("init", NULL);
}

void	quit(int sig)
{
	if (sig != -1) {
		script_call("shutdown", NULL);
	}
	base_shutdown();
	video_shutdown();
	audio_shutdown();
	common_shutdown();
	exit(0);
}

int	main(int argc, char *argv[])
{
	init(argc, argv);

	signal(SIGTERM, quit);
	signal(SIGINT, quit);

	for (;;) {
		frame();
	}
	
	return 0;
}

