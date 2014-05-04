#if defined(__linux__) || defined(__APPLE__)

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

void	sys_copy(const char *from, const char *to)
{
	pid_t pid;
	struct stat st;

	pid = fork();
	
	stat(from, &st);
	
	if (pid == 0) {
		if (st.st_mode & S_IFDIR) {
#ifdef __APPLE__
			execl("/bin/cp", "/bin/cp", "-R", from, to, NULL);
#else
			execl("/bin/cp", "-R", from, to, NULL);
#endif
		} else {
#ifdef __APPLE__
			execl("/bin/cp", "/bin/cp", from, to, NULL);
#else
			execl("/bin/cp", from, to, NULL);
#endif
		}
	}
	waitpid(pid, NULL, 0);
}

int	sys_exists(const char *file)
{
	struct stat st;
	
	return stat(file, &st) == 0;
}

#endif
