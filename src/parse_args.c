#include "ft_ping.h"


static int32_t parse_dest(t_env *env, char *arg)
{
	uint32_t i = 0;
	uint8_t digit_count = 0;
	uint8_t num_count = 0;
	uint8_t last_char = 0;

	while (arg[++i])
	{
		if (arg[i] != '.' && (arg[i] < '0' || arg[i] > '9'))
			return resolve_host(env, arg);
		if (arg[i] == '.')
		{
			if (last_char == '.')
				return resolve_host(env, arg);
			digit_count = 0;
			if (++num_count > 3)
				return resolve_host(env, arg);
		}
		else if (++digit_count > 3)
			return resolve_host(env, arg);
		last_char = arg[i];
	}
	if (num_count != 3 || !digit_count)
		return resolve_host(env, arg);
	env->dst = arg;
	return resolve_host(env, arg);
}

static void parse_c(t_env *env, char *arg, uint8_t i, int32_t *index)
{
	int32_t result = ft_atoi(arg + ++i);
	++*index;
	if (result <= 0)
		ft_exit("ping: bad number of packets to transmit.", EXIT_FAILURE);
	env->params.c = result;
}

static void parse_s(t_env *env, char *arg, uint8_t i, int32_t *index)
{
	int32_t result = ft_atoi(arg + ++i);
	++*index;
	if (result <= -1)
	{
		printf("ping: illegal negative packet size %d.\n", result);
		exit(EXIT_FAILURE);
	}
	if ((uint32_t)result > 0xFFFF - env->ip_header_len - env->icmp_header_len)
	{
		printf("Error: packet size %d is too large. Maximum is %d\n", result, 0xFFFF - env->ip_header_len - env->icmp_header_len);
		exit(EXIT_FAILURE);
	}
	set_payload_len(env, result);
}

static void parse_arg(t_env *env, char *arg, int32_t *index)
{
	if (!arg[0])
		return;
	if (arg[0] == '-')
	{
		if (!arg[1])
			parse_dest(env, arg);
		uint32_t i = 0;
		while (arg[++i])
		{
			if (arg[i] == 'h')
				print_usage(EXIT_SUCCESS);
			else if (arg[i] == 'v')
				env->params.v = 1;
			else if (arg[i] == 'c')
			{
				parse_c(env, arg, ++i, index);
				return;
			}
			else if (arg[i] == 's')
			{
				parse_s(env, arg, ++i, index);
				return;
			}
			else
				print_invalid_param(arg[i]);
		}
	}
	else if (env->dst)
		print_unknown_dst(arg);
	else
		parse_dest(env, arg);
}

void parse_args(t_env *env, int argc, char **argv)
{
	int32_t i = 0;
	while (++i < argc)
		parse_arg(env, argv[i], &i);
}
