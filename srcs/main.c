#include "ft_traceroute.h"

int traceroute(t_traceroute *t)
{
	int	ret;

	t->last_resolved = 0;
	while (t->min_hop <= t->max_hop)
	{
		ret = setsockopt(t->sockfd, IPPROTO_IP, IP_TTL,
			(void *)&t->min_hop, sizeof(t->min_hop));
		if (ret)
			return (printf("Error on setsockopt: %s\n", strerror(errno)));
		if (send_probes(t))
			return (1);
		if (recv_probes(t))
			return (1);
		if (t->last_resolved == t->dest_addr.s_addr)
			break ;
		t->min_hop++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_traceroute	t;

	if (getuid() != 0)
	{
		printf("Permission Denied (try with sudo)\n");
		return (1);
	}
	if (arg_parse(&t, argc, argv))
		return (1);
	debug(&t);

	if (setup(&t))
		return (1);

	if (traceroute(&t))
	{
		close(t.sockfd);
		return (1);
	}

	close(t.sockfd);
	return (0);
}
