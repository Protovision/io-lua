#include "common/common.h"
#include "video/video.h"
#include "audio/audio.h"

void	init(int argc, char *argv[])
{
	const char *path;

	common_init(argc, argv);

	path = gamepath("config.lua");
	if (sys_exists(path)) {
		script_load(path);
	}

	audio_init();
	video_init();
	base_init();
	script_load( gamepath("game.lua") );
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

