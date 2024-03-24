#include "ft_traceroute.h"

void	init_options(t_traceroute *t)
{
	t->resolve = 0;
	t->max_hop = 30;
	t->min_hop = 1;
	t->num_pkts = 3;
	t->wait = secs2time(3);
	t->debug = 0;
	t->hostname = NULL;
	t->size = -1;
}

int	set_resolve(t_traceroute *t, char **args, size_t *i)
{
	t->resolve = 1;
	(void)args;
	(void)i;
	return (0);
}

int	set_max_hop(t_traceroute *t, char **args, size_t *i)
{
	int	max_hop;

	if (!args[1])
		return (printf("Please specify a max hop\n"));
	max_hop = ft_atoi(args[1]);
	if (max_hop < 1 || max_hop > 255)
		return (printf("Please specify a valid max hop\n"));
	t->max_hop = max_hop;
	++(*i);
	return (0);
}

int	set_min_hop(t_traceroute *t, char **args, size_t *i)
{
	int	min_hop;

	if (!args[1])
		return (printf("Please specify a min hop\n"));
	min_hop = ft_atoi(args[1]);
	if (min_hop < 1 || min_hop > 255)
		return (printf("Please specify a valid min hop\n"));
	t->min_hop = min_hop;
	++(*i);
	return (0);
}

int	set_num_pkts(t_traceroute *t, char **args, size_t *i)
{
	int	num_pkts;

	if (!args[1])
		return (printf("Please specify a number of packets\n"));
	num_pkts = ft_atoi(args[1]);
	if (num_pkts < 1 || num_pkts > 10)
		return (printf("Please specify a valid number of packets\n"));
	t->num_pkts = num_pkts;
	++(*i);
	return (0);
}

int	set_wait(t_traceroute *t, char **args, size_t *i)
{
	int	wait;

	if (!args[1])
		return (printf("Please specify a wait\n"));
	wait = ft_atoi(args[1]);
	if (wait < 1 || wait > 60)
		return (printf("Please specify a valid wait\n"));
	t->wait = secs2time(wait);
	++(*i);
	return (0);
}

int	set_debug(t_traceroute *t, char **args, size_t *i)
{
	t->debug = 1;
	(void)args;
	(void)i;
	return (0);
}

char	*flags_usage[] = {
	"-h", "prints the usage",
	"-n", "resolve hostnames",
	"-m", "set max hop count",
	"-M", "set min hop count",
	"-q", "set number of packets per hop",
	"-w", "set wait time for response (seconds)",
	"-d", "set debug mode",
	NULL, NULL
};

int	(*flags[])(t_traceroute *t, char **args, size_t *i) = {
	&set_resolve,
	&set_max_hop,
	&set_min_hop,
	&set_num_pkts,
	&set_wait,
	&set_debug,
	NULL
};

int	usage(char *name)
{
	size_t	i;

	printf("Usage: %s [flags] host [packet size]\n", name);
	i = 0;
	while (flags_usage[i])
	{
		printf("\t%s\t%s\n", flags_usage[i], flags_usage[i + 1]);
		i += 2;
	}
	return (1);
}

int	arg_parse(t_traceroute *t, size_t argc, char **argv)
{
	size_t	i;
	size_t	j;
	int		is_host;

	if (argc < 2)
		return (usage(argv[0]));
	argc -= 1;
	argv = &argv[1];
	init_options(t);

	i = 0;
	while (i < argc)
	{
		is_host = 1;
		j = 0;
		if (!ft_strncmp(flags_usage[0], argv[i], 3))
			return (usage(argv[0]));
		while (flags_usage[(j + 1) * 2] != NULL)
		{
			if (!ft_strncmp(flags_usage[(j + 1) * 2], argv[i],
				ft_strlen(flags_usage[(j + 1) * 2]) + 1))
			{
				if (flags[j](t, &argv[i], &i))
					return (1);
				is_host = 0;
				break ;
			}
			++j;
		}
		if (is_host)
		{
			if (t->hostname != NULL)
			{
				if (t->size != -1)
					return (printf("Invalid argument: %s\n", argv[i]));
				t->size = atoi(argv[i]);
				if (t->size < 0 || t->size > (65535 - 20 - 8))
					return (printf("Invalid packet size\n"));
			}
			else
				t->hostname = argv[i];
		}
		++i;
	}
	if (t->size == -1)
		t->size = 60;
	return (0);
}
