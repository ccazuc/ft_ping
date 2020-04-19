#include "ft_ping.h"

t_env	*init_env(void)
{
	t_env *env;

	if (!(env = malloc(sizeof(*env))))
		ft_exit("Error, could not malloc env, exiting.", EXIT_FAILURE);
	env->dst = NULL;
	env->count = 0;
	env->params.v = 0;
	env->payload_len = 56;
	env->receiving = 1;
	env->ip_header_len = sizeof(env->send_packet->ip_header);
	env->icmp_header_len = ICMP_MINLEN;
	env->headers_len = env->ip_header_len + env->icmp_header_len;
	env->packet_len = env->headers_len + env->payload_len;
	env->send_packet = NULL;
	env->receive_packet = NULL;
	env->ping_min = 0xFFFFFFFFFFFFFFFF;
	env->ping_max = 0;
	env->ping_total = 0;
	env->ping_received = 0;
	env->ping_sent = 0;
	env->start_time = 0;
	env->params.c = 0xFFFFFFFF;
	return env;
}

void set_payload_len(t_env *env, uint32_t len)
{
	env->payload_len = len;
	env->packet_len = env->headers_len + env->payload_len;
}

int	main(int argc, char **argv)
{
	t_env *env;

	if (argc == 1)
		print_usage(EXIT_FAILURE);
	env = init_env();
	parse_args(env, argc, argv);
	if (!env->dst)
		print_usage(EXIT_FAILURE);
	init_connect(env);
	printf("PING %s (%s) %d(%d) bytes of data.\n", env->dst, env->addr, env->payload_len, env->payload_len + 28); 
	signal(SIGINT, (void (*)(int32_t))sighandler);
	signal(SIGALRM, (void (*)(int32_t))sighandler);
	sighandler(1, env);
	if (!(env->send_packet = malloc(sizeof(env->send_packet->ip_header) + sizeof(env->send_packet->icmp_header) + env->payload_len)))
		ft_exit("Error, could not malloc send_packet.", EXIT_FAILURE);
	memset(env->send_packet, 0, sizeof(env->send_packet->ip_header) + sizeof(env->send_packet->icmp_header) + env->payload_len);
	if (!(env->receive_packet = malloc(sizeof(env->receive_packet->ip_header) + sizeof(env->receive_packet->icmp_header) + env->payload_len)))
		ft_exit("Error, could not malloc recive_packet.", EXIT_FAILURE);
	memset(env->receive_packet, 0, sizeof(env->receive_packet->ip_header) + sizeof(env->receive_packet->icmp_header) + env->payload_len);
	build_packet(env);
	env->start_time = get_time();
	send_ping(env);
	alarm(1);
	while (env->receiving)
		receive_ping(env);
	return EXIT_SUCCESS;
}
