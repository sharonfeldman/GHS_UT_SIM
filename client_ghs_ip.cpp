

#include "INTEGRITY.h"
#include "sys\types.h"
#include "sys\socket.h"
#include "unistd.h"
#include "stdio.h"
#include "string.h"

// add libsocket.a to the VAS and libip4.a to the kernel

extern int ip0;
extern int ip1;
extern int ip2;
extern int ip3;

struct  in_addr /* I invented this one, did not find it in socket.h or in the manual */
{
    u_char          addr_bytes[4] ;
} ;

/* this is in the reference manual but it isnt in socket.h, why not ? */
struct sockaddr_in
{
    u_char          sin_len;
    u_char          sin_family;
    u_short         sin_port;
    struct  in_addr sin_addr;
    char            sin_zero[8];
};

int g_clientsocket = -1 ; /* illegal value */


int vcast_clientsocket_startlistener(void)
{
  // now global int clientsocket ;
  if ( g_clientsocket != -1 )
  {
     return g_clientsocket ;
  }
  sockaddr_in destinationaddress ;
  destinationaddress.sin_len = sizeof ( sockaddr_in ) ;
  destinationaddress.sin_family = AF_INET ;
  destinationaddress.sin_port = 46999 ;
  //destinationaddress.sin_addr.addr_bytes [0] = 222 ; // 16.07.17 MK
  //destinationaddress.sin_addr.addr_bytes [1] = 108 ; // 16.07.17 MK
  //destinationaddress.sin_addr.addr_bytes [2] = 202 ; // 16.07.17 MK
  //destinationaddress.sin_addr.addr_bytes [3] =   1 ; // 16.07.17 MK

destinationaddress.sin_addr.addr_bytes [0] = ip0;
destinationaddress.sin_addr.addr_bytes [1] = ip1;
destinationaddress.sin_addr.addr_bytes [2] = ip2;
destinationaddress.sin_addr.addr_bytes [3] = ip3;

  printf("\nHello from client"); 
  g_clientsocket = socket ( AF_INET, SOCK_STREAM, 0 );
  printf ("\nclientsocket is %d", g_clientsocket);
  if ( g_clientsocket != -1 )
  {
     int connectresult = connect ( g_clientsocket, (const sockaddr *) & destinationaddress, sizeof ( sockaddr_in ) );
     printf ("\nconnect result is %d", connectresult) ;
     if ( connectresult != 0 )
     {
        g_clientsocket = -1 ; // return the error
     }
	 /*
     else
     {
       send ( g_clientsocket, "VCAST.SENDLINESTART: 0\n", 24, 0);
     }*/
  }

  return g_clientsocket ; // expect != -1
}

int vcast_clientsocket_send ( const char * msg, int msglen )
{
   int sendresult ;

   sendresult = send ( g_clientsocket, msg, msglen, 0 );
   return sendresult ; // expect > 0
}

int  vcast_clientsocket_sendlinecount (unsigned long clientsendlinecount )
{
    char buffer [40] ;
    sprintf ( buffer, "VCAST.SENDLINECOUNT:%lu\n", clientsendlinecount);
    return send ( g_clientsocket, buffer, strlen (buffer), 0 );
}

#define RECVBUFSIZE 100
char recvbuf [RECVBUFSIZE+1] ;

int vcast_clientsocket_waitrecv_echo ()
{
    char linebuf [100] ; // for accumulating a line
    int destindex = 0 ; // for accumulating a line
    int seenvcastecho = 0 ;
    while (seenvcastecho == 0)
    {
        int recvlen = recv ( g_clientsocket , (void*) recvbuf, sizeof(recvbuf)-1, 0 );
        if ( recvlen <= 0 )
        {
            break;
        }
        recvbuf [recvlen] = 0 ;
        printf("\nThe client received the echo = %s", recvbuf );
        int srcindex = 0 ;
        while (srcindex < recvlen )
        {
              if ( recvbuf [srcindex] == '\n' )
              {
                   destindex = 0 ;
              }
              else if ( destindex >= RECVBUFSIZE )
              {
                   destindex = 0 ; // start the line accumulate process again
              }
              else
              {
                   linebuf [destindex++] = recvbuf[srcindex] ;
                   linebuf [destindex] = 0 ; // for strcmp
                   if ( destindex >= 20 && strcmp ( (const char*)linebuf, "VCAST.RECVLINECOUNT:" ) == 0 )
                   {
                       printf("\nAll done, exiting, seen %s", linebuf);
                       seenvcastecho= 1 ; // to exit outer while loop
                       break ;
                   }
              }
              srcindex++ ;
         }
    }
    return 0;
}

int vcast_clientsocket_close ()
{
    shutdown (g_clientsocket, 2 ) ; // 2 means both send and rx
    close (g_clientsocket) ;
    g_clientsocket = -1 ;
    return 0;
}

#if 0
// remaining code was the echo server for testing during development
              if ( sendresult <= 0 )
              {
                   break;
              }
          }
          if ( sendresult > 0 )
          {
               sendresult = send ( clientsocket, "\nVCAST is Done!", 15, 0 );
          }
          if ( sendresult > 0 )
          {
             u_char recvbuf [100];
             u_char linebuf [100] ; // for accumulating a line
             int destindex = 0 ; // for accumulating a line
             while (1)
             {
                  int recvlen = recv ( clientsocket , (void*) recvbuf, sizeof(recvbuf)-1, 0 );
                  if ( recvlen <= 0 )
                  {
                      break;
                  }
                  recvbuf [recvlen] = 0 ;
                  printf("\nThe client received the echo = %s", recvbuf );
                  int srcindex = 0 ;
                  while (srcindex < recvlen )
                  {
                      if ( recvbuf [srcindex] == '\n' )
                      {
                           destindex = 0 ;
                      }
                      else if ( destindex >= 100 )
                      {
                           destindex = 0 ; // start the line accumulate process again
                      }
                      else
                      {
                           linebuf [destindex++] = recvbuf[srcindex] ;
                           linebuf [destindex] = 0 ; // for strcmp
                           if ( destindex >= 14 && strcmp ( (const char*)linebuf, "VCAST is Done!" ) == 0 )
                           {
                               printf("\nAll done, exiting, seen %s", linebuf);
                               break ;
                           }
                      }
                      srcindex++ ;
                  }

              }
          }
     }
  }
  exit(0);
}


void main (void)
{
  int listenersocket ;
  sockaddr_in listenaddress ;
  listenaddress.sin_len = sizeof ( sockaddr_in ) ;
  listenaddress.sin_family = AF_INET ;
  listenaddress.sin_port = 2000 ;
  listenaddress.sin_addr.addr_bytes [0] = 127 ;
  listenaddress.sin_addr.addr_bytes [1] = 0 ;
  listenaddress.sin_addr.addr_bytes [2] = 0 ;
  listenaddress.sin_addr.addr_bytes [3] = 1 ;
  sockaddr_in acceptaddress ;
  acceptaddress.sin_len = sizeof ( sockaddr_in ) ;
  acceptaddress.sin_family = AF_INET ;
  int acceptsize = sizeof ( acceptaddress );
  printf("\nHello from listener");
  listenersocket = socket ( AF_INET, SOCK_STREAM, 0 );
  printf ("\nlistenersocket is %d", listenersocket);
  if ( listenersocket != -1 )
  {
     int bindresult = bind ( listenersocket, (const sockaddr *) & listenaddress, sizeof ( sockaddr_in ) );
     printf ("\nbind result is %d", bindresult) ;
     if ( bindresult == 0 )
     {
         int listenresult = listen ( listenersocket, 1 ) ;
         printf ("\nListen result is %d", listenresult ) ;
         if ( listenresult == 0 )
         {
             int acceptsocket = accept ( listenersocket , (sockaddr*) & acceptaddress, & acceptsize );
             printf ("\n accept socket = %d", acceptsocket );
             if (acceptsocket >= 0)
             {
                 u_char recvbuf [100];
                 while (1)
                 {
                      int recvlen = recv ( acceptsocket , (void*) recvbuf, sizeof(recvbuf), 0 );
                      if ( recvlen <= 0 )
                      {
                          break;
                      }
                      int sendresult = send ( acceptsocket, recvbuf, recvlen, 0 ); 
                      if ( sendresult <= 0 )
                      {
                          break;
                      } 
                 }
             }
         }
     }
  }
  exit (0);
}
#endif
