/* 
   01/05/2003 Nrg2Iso v 0.1

   Copyright (C) 2003 Grégory Kokanosky <gregory.kokanosky@free.fr>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
 
#define VERSION "0.4"

void usage(){
  printf("Nrg2Iso v%s by G. Kokanosky\n", VERSION);
  printf("released under the GNU GPL v2 or later\n\n");

  printf("Usage :\n");
  printf("nrg2iso image.nrg image.iso\n\n");
  printf("--version    display version number\n");
  printf("--help       display this notice\n\n");

}


int checkIso(char *filename)
{
  int iso = 0 ;
  FILE *f;
  char buf[17*2048];
  
  if(!(f=fopen(filename,"rb"))) return 0;
  
  if( fread( buf, 1,17*2048 ,f) == 17*2048 ) {

    // taken from k3b
    // check if this is an iso9660-image
    // the beginning of the 16th sector needs to have the following format:
    
    // first byte: 1
    // second to 11th byte: 67, 68, 48, 48, 49, 1 (CD001)
    // 12th byte: 0
    
    iso = ( buf[16*2048] == 1 &&
	    buf[16*2048+1] == 67 &&
	    buf[16*2048+2] == 68 &&
	    buf[16*2048+3] == 48 &&
	    buf[16*2048+4] == 48 &&
	    buf[16*2048+5] == 49 &&
	    buf[16*2048+6] == 1 &&
	    buf[16*2048+7] == 0 );
  }
  if(iso){
    printf("It seems that %s is already an ISO 9660 image \n",filename);
    printf("[Aborting conversion]\n");
  }

  
  fclose(f);
 return iso;
}

#define NUM_OF_COLUMNS 70

int main(int argc, char **argv){
  FILE *nrgFile, *isoFile;
  char buffer[1024 * 1024];
  size_t i=0,j=0;
  size_t size=0,k=0,l;
  size_t nrgSize=0;
  int percent = 0;
  int old_percent = -1;
  struct stat buf;
  
  if( argc==2 && (strcmp(argv[1],"--version") == 0) ){
    printf("nrg2iso v%s\n", VERSION);
    return 0;
  }
  
  
  if(argc!=3 ||(argc==2 && strcmp(argv[1],"--help")==0)){
    usage();
    return -1;
  }
  
  
  if( stat(argv[1],&buf)==0){
    
    if(!checkIso(argv[1])){
      
      nrgSize = buf.st_size;
      nrgFile=fopen(argv[1],"rb");
      fseek (nrgFile, 307200, SEEK_SET);
      
      isoFile=fopen(argv[2],"wb+");
      
      while((i=  fread( buffer, 1, sizeof(buffer), nrgFile ))>0){
	fwrite(buffer,i,1,isoFile);
	
	size+=i;
	percent = (int)(size * 100.0 / nrgSize);
	
	if(percent != old_percent) {
	  old_percent = percent;
	  printf("\r|");
	  for(l = 0; l < percent * NUM_OF_COLUMNS / 100; l++) {
	    printf("=");
	  }
	  printf(">[%d%%]",percent);
	  fflush(stdout);
	}
      }
      printf("\r|");
      for(l = 0; l < NUM_OF_COLUMNS; l++) {
	printf("=");
      }
      printf(">[100%%]");
      fflush(stdout);
      
      fclose(nrgFile);
      fclose(isoFile);
      printf("\n%s written : %lu bytes\n",argv[2],size);
    }
  }
  else 
    printf("%s : No such file\n",argv[1]);
  
  
  
  return 0;
  
}
