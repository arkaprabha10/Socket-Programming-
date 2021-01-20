#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define	READ	0      /* The index of the "read" end of the pipe */ 
#define	WRITE	1      /* The index of the "write" end of the pipe */ 

char *questions[]={"Quit", "In which university do you study?", "Which course are you studying?", "What is your area of interest?"}; 

char* answers[] = {"Quit", "DAIICT", "Systems Software", "Kernel Programming"};

int main(void) 
{
	int fd1[2],fd2[2], bytesRead; //fd1 is for parent to child communication //fd2 is for child to parent communication	
	char message1[100],message2[100];   /* Parent process' message buffer */
	pipe(fd1);  
	pipe(fd2);
    int choice =-1,stat;

	FILE *filePointer ; // Open the existing file using fopen() 
	filePointer = fopen("filehandlingtrial.txt", "a") ; 
	if ( filePointer == NULL ) 
	{
		printf( "file failed to open." ) ;
		exit(0);
	}	 
	
	if ( fork() == 0 )  /* Child */
	{
		close(fd1[WRITE]);
		close(fd2[READ]);
		bytesRead = read(fd1[READ], message1, 100 );
		int i;
		for(i=0;i<3;i++)
		{
			if(message1==questions[i])
				break;
		}
		fputs(answers[i], filePointer) ; 
		fputs("\n", filePointer) ;
		write(fd2[WRITE], answers[i],strlen(answers[i])+1);
		if(i==0)
		{
		close(fd1[READ]);
		close(fd2[WRITE]);			
		}
	} 
	else      /* Parent */
	{	
		close(fd1[READ]); // Parent will send choice to child
		close(fd2[WRITE]);
		while(choice!=0)
		{
			printf("Select Option (0,1,2,3) : ");
			scanf("%d",&choice);
			if(choice>=0 && choice <=3)
			{	write(fd1[WRITE], questions[choice],strlen(questions[choice])+1); /* Send */	
				fputs(questions[choice], filePointer) ; 
				fputs("\n", filePointer) ;
				bytesRead = read( fd2[READ], message2, 100 ); /* Receive */
				printf("Read %d bytes: %s \n", bytesRead, message2 );
			}
			else
			printf("You have chose an unavailable option \n");
		}
		wait(&stat);
		close(fd1[WRITE]);
		close(fd2[READ]);	 
	}
	fclose(filePointer) ;
    return 0;
}