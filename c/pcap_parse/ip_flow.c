#include "rte_jhash.h"
#include "ip_flow.h"

int ip_flow_hash(IP_FLOW*flow)
{
	int hash = 0;
	if(!flow)
		return -1;
	int c = (flow->high_port<<16)|flow->low_port;
	hash = rte_jhash_3words(flow->high_ip, flow->low_ip, c,hash);
	flow->hash = hash;
	return hash;
}

