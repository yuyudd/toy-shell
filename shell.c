#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#define MAX_LEN_LINE    100
#define LEN_HOSTNAME     30
#define LEN_PWD  50
int main(void)
{
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    
    char hostname[LEN_HOSTNAME +1];
    memset(hostname, 0x00, sizeof(hostname));
   
    gethostname(hostname, LEN_HOSTNAME);
    char pwd[LEN_PWD +1];
    getcwd(pwd,sizeof(pwd));
    pid_t pid, cpid;
    char buf[80];
    
    while (true) {
        char *s;
	char buf[80];
        int len;
       	char *s1 = hostname;
	printf(" \x1b[33m 유저이름 : \x1b[0m %s\n", getpwuid(getuid())->pw_name);	
	printf( "\x1b[33m 호스트의 이름 :\x1b[0m%s\n",  s1);
	printf( "\x1b[31m %s\n \x1b[0m ", pwd );
        printf("\x1b[32m  MyShell$ \x1b[0m  ");
        s = fgets(command, MAX_LEN_LINE, stdin);
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }
	


        len = strlen(command);
        printf("%d\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }
        
        printf("[%s]\n", command);
      
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");        
            }
            printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }
        else {  /* child */
            ret = execve(args[0], args, NULL);
            if (ret < 0) {
                fprintf(stderr, "execve failed\n");   
                return 1;
            }
	    
        }
	printf( " \x1b[31m   종료시에는 exit를 입력!, 계속진행시에는 아무키를 누르시오 ! \x1b[0m  ");

	if (fgets(buf, 80, stdin)!= NULL)
	{
		buf[strlen(buf)-1] = 0x00;
		if(strcmp(buf,"exit")==0)
			exit(0);
	}

	
    }



    return 0;
}
