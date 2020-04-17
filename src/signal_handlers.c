#include "ft_ping.h"

static void handle_sigalrm(t_env *env)
{
	(void)env;
}

static void handle_sigint(t_env *env)
{
	(void)env;
	exit(EXIT_SUCCESS);
}

int32_t sighandler(int32_t signal, void *ptr)
{
	static t_env *env = NULL;
	if (!env)
		env = ptr;
	if (signal == SIGALRM)
		handle_sigalrm(env);
	else if (signal == SIGINT)
		handle_sigint(env);
	return 0;
}
