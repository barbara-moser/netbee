/*****************************************************************************/
/*                                                                           */
/* Copyright notice: please read file license.txt in the NetBee root folder. */
/*                                                                           */
/*****************************************************************************/

#pragma once

#include <cstdint>
// struct timeval
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/time.h>
#endif

struct pcap_pkthdr {
	struct timeval ts;	/* time stamp */
	uint32_t caplen;	/* length of portion present */
	uint32_t len;	/* length this packet (off wire) */
};
