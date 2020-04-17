#include "ft_ping.h"

t_env	*init_env(void)
{
	t_env *env;

	if (!(env = malloc(sizeof(*env))))
		ft_exit("Error, could not malloc env, exiting.", EXIT_FAILURE);
	env->dst = NULL;
	env->count = 0;
	env->params.v = 0;
	env->payload_len = 0;
	//env->packet_len = sizeof(env->packet->ip_header) + sizeof(env->packet->icmp_header) + env->payload_len;
	env->packet_len = sizeof(env->packet->ip_header) + ICMP_MINLEN + env->payload_len;
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
	printf("Dst: '%s', v: %d\n", env->dst, env->params.v);
	init_connect(env);
	printf("PING %s (%s) %d(%d) bytes of data.\n", env->dst, env->addr, env->payload_len, env->payload_len + 28); 
	signal(1, (void (*)(int32_t))sighandler);
	sighandler(1, env);
	if (!(env->packet = malloc(sizeof(*env->packet))))
		ft_exit("Error, could not malloc packet.", EXIT_FAILURE);
	if (!(env->packet->payload = malloc(env->payload_len)))
		ft_exit("Error, could not malloc packet payload.", EXIT_FAILURE);
	build_packet(env);
	send_ping(env);
	return EXIT_SUCCESS;
}
