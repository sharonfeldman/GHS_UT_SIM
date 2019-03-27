#include "vcast_basics.h"
#include "vcast_string.h"

#ifdef __cplusplus
extern "C" {
#endif
    /* Declarations of this file */
	int vcast_strlen(const char *VC_S);
	void vcast_custom_output(char *S); 
#ifdef __cplusplus 
} 
#endif 
	int vcast_clientsocket_startlistener(void) ;
	int vcast_clientsocket_send ( const char * msg, int msglen );
	int vcast_clientsocket_close ();
	int vcast_clientsocket_waitrecv_echo ();
	int vcast_clientsocket_sendlinecount (unsigned long);
	unsigned long g_vcast_clientsocket_sendlinecount = 0 ;
	static char * vcast_strncpy (char *VC_S, const char *VC_T, int maxdest);
static char *
vcast_strncpy (char *VC_S, const char *VC_T, int maxdest)
{
    while ( maxdest > 1 && *VC_T )
    {
	maxdest-- ;
	*VC_S++ = *VC_T++;
    }
    *VC_S = 0 ;
    return VC_S;
}

int
vcast_strlen(const char *VC_S)
{
   int vc_result = 0;

   while(*VC_S) {
      vc_result++;
      VC_S++;
   }

   return vc_result;
}

/** This function compares string VC_T to string VC_S */
int VCAST_strcmp ( char *VC_S, char *VC_T )
{
   int vcast_i;
   for (vcast_i=0;VC_S[vcast_i]==VC_T[vcast_i];vcast_i++) {
     if ( VC_S[vcast_i] == 0 )
       return 0;
   }
   
   
   return VC_S[vcast_i] - VC_T[vcast_i];
}

void vcast_custom_output(char *S)
{
	
	vcast_clientsocket_startlistener() ; /* should open the socket only once */
	
	char nlbuffer [50] ; /* 50 is enough for the longest plausible coverage output, the GHS output used 42 */
	vcast_strncpy ( nlbuffer, S, 50 );
	int nlbufferlen = vcast_strlen (S) ;
	/*nlbuffer [ nlbufferlen++ ] = '\n' ;*/
	int retbufferlen = 0 ;
	//printf("\nGOT TO SEND : %S - SIZE IS %d \n", S, nlbufferlen);
	int i;
    do
    {
      vcast_clientsocket_close();
      vcast_clientsocket_startlistener();
	  printf("ABOUT TO SEND\n");
	  for(i=0;i<nlbufferlen;i++)
	    printf("%c",nlbuffer[i]);
      retbufferlen = vcast_clientsocket_send ( nlbuffer, nlbufferlen );
	  printf("\nReturened len : %d - LEN WAS %d \n", retbufferlen, nlbufferlen);
    } while (nlbufferlen != retbufferlen);
    g_vcast_clientsocket_sendlinecount++;
	
}

