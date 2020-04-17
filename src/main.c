#include "ft_ping.h"

t_env	*init_env(void)
{
	t_env *env;

	if (!(env = malloc(sizeof(*env))))
		ft_exit("Error, could not malloc env, exiting.", EXIT_FAILURE);
	env->dst = NULL;
	env->count = 0;
	env->params.v = 0;
	return env;
}

int	main(int argc, char **argv)
{
	t_env *env;

	if (argc == 1)
		print_usage(EXIT_FAILURE);
	env = init_env();
	parse_args(env, argc, argv);
	printf("Dst: '%s', v: %d", env->dst, env->params.v);
	return EXIT_SUCCESS;
}
