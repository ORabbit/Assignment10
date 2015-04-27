/* Simple echo server demonstrating listening to, reading from,
 * and writing to a network socket. */
/* DWB */
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

#define BUF_SIZE 500

int
main (int argc, char *argv[])
{
  int sockfd, myport, nread, their_addr_len;
  struct sockaddr_in my_addr, their_addr;
  char buf[BUF_SIZE];

  if (argc != 2)
    {
      fprintf (stderr, "Usage: %s portnum\n", argv[0]);
      exit (EXIT_FAILURE);
    }

  myport = atoi (argv[1]);

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);

  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons (myport);
  my_addr.sin_addr.s_addr = INADDR_ANY;
  memset (&(my_addr.sin_zero), '\0', 8);

  if (bind (sockfd, (struct sockaddr *) &my_addr, sizeof (struct sockaddr)))
    {
      close (sockfd);
      fprintf (stderr, "Failed to bind socket!\n");
      exit (EXIT_FAILURE);
    }
  else
    {
      printf ("Server listening on port %d\n", myport);
    }

  their_addr_len = sizeof (struct sockaddr_in);
  nread = recvfrom (sockfd, buf, BUF_SIZE, 0,
		    (struct sockaddr *) &their_addr, &their_addr_len);

  char host[NI_MAXHOST], service[NI_MAXSERV];
  int result;
  result = getnameinfo ((struct sockaddr *) &their_addr, their_addr_len,
			host, NI_MAXHOST, service, NI_MAXSERV,
			NI_NUMERICSERV);

  if (result == 0)
    printf ("Received %d bytes from %s:%s\n", (long) nread, host, service);
  else
    fprintf (stderr, "getnameinfo: %s\n", gai_strerror (result));

  if (sendto (sockfd, buf, nread, 0, (struct sockaddr *) &their_addr,
	      their_addr_len) != nread)
    {
      fprintf (stderr, "Error sending response\n");
    }
  close (sockfd);
  return 0;
}
