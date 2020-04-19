#include "ft_ping.h"

static void handle_sigalrm(t_env *env)
{
	send_ping(env);
	alarm(1);
}

static void handle_sigint(t_env *env)
{
	if (env->ping_sent)
	{
		printf("--- %s ping statistics ---\n", env->dst);
		printf("%ld packets transmitted, %ld received, %.0f%% packet loss, time %ldms\n", env->ping_sent, env->ping_received, (env->ping_sent - env->ping_received) / (float)env->ping_sent * 100.f, (get_time() - env->start_time) / 1000);
		if (env->ping_received)
			printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", env->ping_min / 1000.f, (env->ping_total / env->ping_received) / 1000.f, env->ping_max / 1000.f, 15.f);
	}
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
