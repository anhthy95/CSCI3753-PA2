#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
 
#define BUFFER_LENGTH 1024              
static char *user_buffer;     ///< The receive buffer from the LKM
 
int main(){


	int devfile;
	devfile = open("/dev/pa2_character_device", O_RDWR);  
	
	int loop = 1;
	char user_choice;
	
	
	while(loop){
	
	printf("r:read\nw:write\ns:seek\ne:exit\nEnter choice:\n");
	scanf("%c", &user_choice);

	switch(user_choice){
	
	//read
	case 'r': 
	printf("Enter the number of bytes you want to read:\n");
	int bytes;
	scanf("%d",&bytes);
	user_buffer= malloc(bytes);
	read(devfile, user_buffer, bytes);
	printf("Data read from device:%s", user_buffer);
	break;

	//write
	case 'w': 
	printf("Enter data you want to write:");
	char string_to_send[BUFFER_LENGTH];
	//scan string with no spaces
	scanf("%s", string_to_send);
	printf("writing data");
	write(devfile, string_to_send, strlen(string_to_send));
	break;

	//seek
	case 's':
	printf("Enter an offset value:\n");
	int whence;
	int offset;
	scanf("%d",&offset);
	printf("Enter a value for whence:\n");
	scanf("%d", &whence);
	llseek(devfile, offset, whence);
	break;
	
	//exit
	case 'e':
	loop=0;
	break;
	


}//switch
}//loop   
  
 printf("End of the program\n");
   return 0;
}
