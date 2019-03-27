/* 
   EchoServer.cpp

   Copyright (C) 2002-2004 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/


#include "Socket.h"
#include <process.h>
#include <string>
#include "stdio.h" /* SJW */


unsigned __stdcall Answer(void* a) {
  Socket* s = (Socket*) a;
  printf ("\nCall answered at %p",s);
  unsigned long receivedbytes = 0 ;	// T.L. 14.07.2014 by suggestion of SJW
  while (1) {
    std::string r = s->ReceiveLine();
    if (r.empty()) break;
    if (s->rxdatafile != 0) /* SJW */
    {
        fprintf ( s -> rxdatafile, "%s", r.c_str()  ) ;	// T.L. 14.07.2014 by suggestion of SJW
		receivedbytes += strlen (r.c_str() );	// T.L. 14.07.2014 by suggestion of SJW
        if ( receivedbytes >= 500000 )	// T.L. 23.07.2014
        {
            s->close_reopen_outputfile(); /* ST */
            receivedbytes = 0;
        }
    }
    if ( r.compare ( 0, 20, "VCAST.SENDLINECOUNT:" ) == 0 )
    {
      printf("\nserver receives %s", r.c_str() );
      char buffer [30] ;
      sprintf ( buffer, "VCAST.RECVLINECOUNT: %lu\n", s -> recvlinecount );
      s -> SendLine (buffer) ;
      fprintf ( s -> rxdatafile, "%s\n", buffer );
    }
  } //while()
  if (s->rxdatafile != 0)
  {
     fclose ( s->rxdatafile ) ; /* SJW */
  }
  delete s;
  printf ("\nCall ends, thread terminated at %p",s);
  return 0;
}


int main(int argc, char* argv[]) {

  SocketServer in(46999,5);
  printf("\nCoverage server listening on port 46999");  
  while (1) {
    Socket* s=in.Accept();

    unsigned ret;
    _beginthreadex(0,0,Answer,(void*) s,0,&ret);
  }
 
  return 0;
}
