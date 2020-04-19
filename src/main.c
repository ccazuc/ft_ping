#include "ft_ping.h"

t_env	*init_env(void)
{
	t_env *env;

	if (!(env = malloc(sizeof(*env))))
		ft_exit("Error, could not malloc env, exiting.", EXIT_FAILURE);
	env->dst = NULL;
	env->count = 0;
	env->params.v = 0;
	env->payload_len = 51;
	env->receiving = 1;
	env->ip_header_len = sizeof(env->packet->ip_header);
	env->icmp_header_len = ICMP_MINLEN;
	env->headers_len = env->ip_header_len + env->icmp_header_len;
	env->packet_len = env->headers_len + env->payload_len;
	env->packet = NULL;
	return env;
}

int	main(int argc, char **argv)
{
	t_env *env;

	if (argc == 1)
		print_usage(EXIT_FAILURE);
	env = init_env();
	parse_args(env, argc, argv);
	init_connect(env);
	printf("PING %s (%s) %d(%d) bytes of data.\n", env->dst, env->addr, env->payload_len, env->payload_len + 28); 
	signal(SIGINT, (void (*)(int32_t))sighandler);
	signal(SIGALRM, (void (*)(int32_t))sighandler);
	sighandler(1, env);
	if (!(env->packet = malloc(sizeof(env->packet->ip_header) + sizeof(env->packet->icmp_header) + env->payload_len)))
		ft_exit("Error, could not malloc packet.", EXIT_FAILURE);
	memset(env->packet, 0, sizeof(env->packet->ip_header) + sizeof(env->packet->icmp_header) + env->payload_len);
	build_packet(env);
	send_ping(env);
	alarm(1);
	while (env->receiving)
		receive_ping(env);
	return EXIT_SUCCESS;
}
