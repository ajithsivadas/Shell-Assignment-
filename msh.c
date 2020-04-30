/*

Name:Ajith Sivadas
SID:1001829098

*/

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
x

#define WHITESPACE " \t\n"	// We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 10     // Mav shell only supports ten arguments
#define NO_OF_PROCESS 15        //Maximunm process to show in the history
#define true 1                  //setting the value of true as 1
#define false 0                 // setting the value of false as 0
int main()
{

// Defining the showpids array and the History array
 char * cmd_str = (char*) malloc(MAX_COMMAND_SIZE);
 int showpids[NO_OF_PROCESS];
 int k = 0;
 char history [NO_OF_PROCESS][MAX_COMMAND_SIZE];
 int m;
 int j = 0;
 int count = 0;
 pid_t child_pid;

//Initializing the showpids array to 0 as well as the history array to 0
 while( k < NO_OF_PROCESS )
  {
      showpids[k] = 0;
      k++;
  }
 for( m = 0; m < NO_OF_PROCESS; m++ )
 {
     memset( history[m] , 0 , MAX_COMMAND_SIZE );
 }

 while( 1 )
 {
    // Print out the msh prompt
    printf ("msh> ");





    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (cmd_str,MAX_COMMAND_SIZE,stdin) );

    /* Parse input */
    char *token[MAX_NUM_ARGUMENTS];

    int   token_count = 0;
    // Pointer to point to the token
    // parsed by strsep
    char *arg_ptr;
    if(strlen(cmd_str)==1) 
    {
        continue;
    }
    // here it will check the !n and execute the nth command
    if( cmd_str[0] == '!')
    {
     	int index = atoi( &cmd_str[1] );
        memset( cmd_str , 0 , MAX_COMMAND_SIZE ); // memory allocation of the maximum size
        if(strcmp(history[index],"")==0)         // will check the value of the process and print accordingly
        {
            printf("Command not in history\n");
            continue;
        }
	else
	{
            strncpy( cmd_str , history[index] ,strlen(history[index]));    ///coping the nth commad to cmd_str for execution
        }

    }



    char *working_str  = strdup(cmd_str);

    // we are going to move the working_str pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *working_root = working_str;

    // Tokenize the input stringswith whitespace used as the delimiter
    while ( ( (arg_ptr = strsep(&working_str, WHITESPACE) ) != NULL) &&
                (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( arg_ptr, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 )
      {
        token[token_count] = NULL;
      }
       	token_count++;
    }

    if (count > 14)
    {
     	count = 0;
    }
    if(j > 14)
    {
     	j = 0;
    }
    else
    {
     	strncpy(history[j++],cmd_str,strlen(cmd_str));   //copying the command entered in the terminal to histroy 2D array
    }
    // Now print the tokenized input as a debug check
    // \TODO Remove this code and replace with your shell functionality

    //Defining the cmd array and the status for the waitpid
    //Defining the cmd array and the status for the waitpid
    char cmd[10];
    int status;
    char flag = false;


    sprintf(cmd, "%s", token[0]);   //copying the command from the terminal and setting it into cmd
    do {

        if ( strcmp (cmd,"quit") == 0 || strcmp (cmd, "exit") == 0 )    // check for the quit and exit commands
        {
            exit(0);
            flag = true;
        }
	else if( strcmp(cmd,"cd") == 0 )  // check for thr cd to execute the directory commands
        {
            chdir(token[1]);
            flag = true;
        }
	else if (token[0] == '\0')  // when enter is given it should continue the loop and display the msh
        {
            flag = true;
            break;

        }
	else if( strcmp(cmd,"showpids" ) == 0 ) // displaying the last 15 pids
        {
            int i;
            for( i = 0; i < NO_OF_PROCESS; i++ )
            {
             	printf("%d:  %d\n",i,showpids[i] );
            }
            }

	else if( strcmp(cmd,"history" ) == 0 )  // To show the last 15 process
            {
             	int i;
                for( i = 0; i < NO_OF_PROCESS; i++ )
                {
                printf("%d:  %s\n",i,history[i]);
                }
                }

            else
            {
                    child_pid = fork(); // invoking the fork function
                    if(child_pid == 0 )
                    {

                     	if(execvp(token[0],  token ) == -1 )

                        {
                            printf("Command not found \n");
                        }
                        flag = true;
                        exit( 0 );

                    }

                     else
                    {

                showpids[count]=child_pid;         // storing the pids which is there to showpids array
                waitpid( child_pid, &status, 0 );
                count++;                          // incrementing for the next process
                continue;
                break;
            }
	}
        break;
	}
	while (flag);

    free(working_root);
    continue;
 }

  return 0;
}




