#include "ft_ping.h"


static void parse_dest(t_env *env, char *arg)
{
	uint32_t i = 0;
	uint8_t digit_count = 0;
	uint8_t num_count = 0;
	uint8_t last_char = 0;

	while (arg[++i])
	{
		if (arg[i] != '.' && (arg[i] < '0' || arg[i] > '9'))
			resolve_dns(env, arg);
		if (arg[i] == '.')
		{
			if (last_char == '.')
				resolve_dns(env, arg);
			digit_count = 0;
			if (++num_count > 3)
				resolve_dns(env, arg);
		}
		else if (++digit_count > 3)
			resolve_dns(env, arg);
		last_char = arg[i];
	}
	if (num_count != 3 || !digit_count)
		resolve_dns(env, arg);
	env->dst = arg;
}

static void parse_arg(t_env *env, char *arg)
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
			if (arg[i] == 'v')
				env->params.v = 1;
			else
				print_invalid_param(arg[i]);
		}
	}
	else
		parse_dest(env, arg);
}

void parse_args(t_env *env, int argc, char **argv)
{
	int32_t i = 0;
	while (++i < argc)
		parse_arg(env, argv[i]);
}
