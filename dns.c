#include <corecrt_malloc.h>
#include <string.h>
#include <stdio.h>
#include "dns.h"

typedef struct _Node {
  char *domain;
  IPADDRESS ip;
  struct _Node *next_pair;
} Node;

const int SIZE = 12837;
DNSHandle InitDNS() { //initializes DNS engine
  DNSHandle hDNS = (unsigned int) (Node *) calloc(SIZE, sizeof(Node));
  if ((Node *) hDNS != NULL) { //must check bc of calloc
    return hDNS;
  }
  return INVALID_DNS_HANDLE;
}

unsigned int Hash(const char *dmn) {
  unsigned int hash = 0;
  for (; *dmn; ++dmn) {
    hash += *dmn;
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  return hash % SIZE;
}

void Add_to_table(DNSHandle hDNS, char *dmn, IPADDRESS ip) { //fill the table
  Node *p;
  unsigned int uHostNameLength = strlen(dmn);
  unsigned int index_element = Hash(dmn);

  if (((Node *) hDNS)[index_element].domain == NULL) {
    ((Node *) hDNS)[index_element].domain = (char *) malloc(uHostNameLength + 1);
    strcpy(((Node *) hDNS)[index_element].domain, dmn);
    ((Node *) hDNS)[index_element].ip = ip; //bc ip isnt a pointer
  } else { //same hash - diff domain
    p = (Node *) malloc(sizeof(Node));
    p->domain = (char *) malloc(uHostNameLength + 1);
    strcpy(p->domain, dmn);
    p->ip = ip;
    p->next_pair = ((Node *) hDNS)[index_element].next_pair;
    ((Node *) hDNS)[index_element].next_pair = p;
  }
}

void LoadHostsFile(DNSHandle hDNS, const char *hostsFilePath) { // loads DNS information from the file
  FILE *fInput = NULL;
  fInput = fopen(hostsFilePath, "r");
  unsigned int ip1 = 0, ip2 = 0, ip3 = 0, ip4 = 0;
  char *string = (char *) malloc(201); //returns the address to the first byte of the memory area

  if (NULL == fInput) {
    return;
  }

  while (fscanf_s(fInput, "%d.%d.%d.%d %s", &ip1, &ip2, &ip3, &ip4, string, 201) != EOF) {
    IPADDRESS ip = (ip1 & 0xFF) << 24 | //converting and masking IP address
                    (ip2 & 0xFF) << 16 |
                    (ip3 & 0xFF) << 8 |
                    (ip4 & 0xFF);
    Add_to_table(hDNS, string, ip);
  }

  free(string);
  fclose(fInput);
}

IPADDRESS DnsLookUp(DNSHandle hDNS, const char *hostName) { //performs DN Look up in the DN engine specified
  unsigned int index = Hash(hostName);
  Node *it = &((Node *) hDNS)[index]; //address of the node (at index)
  while (it != NULL && strcmp(it->domain, hostName) != 0) {
    it = it->next_pair;
  }
  if (it != NULL) {
    return it->ip;
  }
  return INVALID_IP_ADDRESS;
}

void ShutdownDNS(DNSHandle hDNS) { //shuts down DNS engine and release all its resources
  Node *tmp;
  Node *current;
  for (int i = 0; i < SIZE; i++) {
    current = &(((Node *) hDNS)[i]);
    while (current) {
      tmp = current;
      current = current->next_pair;
      free(tmp->domain);
      //free(tmp);
    }
  }
  if ((Node *) hDNS != NULL) {
    free((Node *) hDNS);
  }
}