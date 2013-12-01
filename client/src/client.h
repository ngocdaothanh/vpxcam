#ifndef _CLIENT_H_
#define _CLIENT_H_

void tcp_connect(char* host, char* port);

void tcp_read(char* buffer, int length);

#endif
