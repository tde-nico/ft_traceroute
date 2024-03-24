#include "ft_traceroute.h"

void	debug(t_traceroute *t)
{
	if (!t || !t->debug)
		return ;
	printf("resolve: %d\n", t->resolve);
	printf("max_hop: %d\n", t->max_hop);
	printf("min_hop: %d\n", t->min_hop);
	printf("num_pkts: %d\n", t->num_pkts);
	printf("wait: %ld\n", t->wait.tv_sec);
	printf("debug: %d\n", t->debug);
	printf("hostname: %s\n", t->hostname);
	printf("packet size: %d\n", t->size);
	printf("\n");
}
