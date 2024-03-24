#include "ft_traceroute.h"

int	get_dest_addr(t_traceroute *t)
{
	struct addrinfo	*server_res;
	struct addrinfo	*tmp;
	struct addrinfo	hints;
	int				ret;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	hints.ai_flags = AI_CANONNAME;
	ret = getaddrinfo(t->hostname, NULL, &hints, &server_res);
	if (ret < 0)
		return (printf("Error on getaddrinfo: %s\n", gai_strerror(ret)));
	tmp = server_res;
	while (tmp)
	{
		if (tmp->ai_family == AF_INET &&
			tmp->ai_socktype == SOCK_RAW &&
			tmp->ai_protocol == IPPROTO_ICMP &&
			tmp->ai_addrlen == sizeof(struct sockaddr_in) &&
			tmp->ai_addr != NULL)
			break ;
		tmp = tmp->ai_next;
	}
	if (tmp)
	{
		t->dest_info = *tmp;
		t->dest_sockaddr = *tmp->ai_addr;
		t->dest_addr = ((struct sockaddr_in *)tmp->ai_addr)->sin_addr;
	}
	freeaddrinfo(server_res);
	return (0);
}

int	resolve_addr(t_traceroute *t)
{
	struct sockaddr_in	addr;
	int					ret;

	addr.sin_addr = t->dest_addr;
	addr.sin_family = AF_INET;
	inet_ntop(addr.sin_family, &addr.sin_addr, t->ip, sizeof(t->ip));
	if (t->debug)
		printf("Resolved %s to %s\n", t->hostname, t->ip);
	if (t->resolve)
	{
		ret = getnameinfo((struct sockaddr *)&addr, sizeof(addr),
			t->addr, sizeof(t->addr), NULL, 0, 0);
		if (ret < 0)
			return (printf("Error on getnameinfo: %s\n", gai_strerror(ret)));
		if (t->debug)
			printf("Resolved %s to %s\n", t->ip, t->addr);
	}
	return (0);
}

int	setup(t_traceroute *t)
{
	if (get_dest_addr(t))
		return (1);
	if (resolve_addr(t))
		return (1);
	t->sockfd = socket(t->dest_info.ai_family, SOCK_RAW, IPPROTO_ICMP);
	if (t->sockfd < 0)
		return (printf("Error on socket: %s\n", strerror(errno)));
	printf("traceroute to %s [%s], with %d max hops, %d bytes packets\n",
		t->hostname, t->ip, t->max_hop, t->size);
	return (0);
}
