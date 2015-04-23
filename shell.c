#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
int word_count=0;
void type_prompt(){
    char name[50],direct[50];
    struct passwd *my_info;
    my_info = getpwuid(getuid());
    gethostname(name,sizeof(name));
    getcwd(direct,sizeof(direct));
    printf("[MY_shell]%s@%s:%s$ ",my_info->pw_name,name,direct);
}

void read_command(char command[],char *parameters[]){
    char input[50],*left,*right;
    int let_count=0;
    fgets(input,sizeof(input),stdin);
    word_count = 0;
    
    left = input;right = input;
    //printf("%d",*right);
    //if(input[3]=='\n'){printf("yes");}
    
    while(*right!=10){
	//printf("%d",*right);
        let_count = 0;
        while(*right!=10&&*right!=32){
            right++;
		//printf("%ld ",(right-input));
        }
        while(left!=right){
            parameters[word_count][let_count] = *left;
            if(word_count==0){command[let_count]=*left;}
            let_count++;
            left++;
	    //printf("%ld",left-input);
        }
        if(word_count==0){command[let_count]='\0';}
        parameters[word_count][let_count]='\0';
        word_count++;
        if(*left==10||*right==10){break;}
        while(*right==32){right++;}
	if(*right ==10){break;}
        left = right;
	//printf("%d ",*right);
    }
    parameters[word_count] = NULL;
}

int main(){
    int i=0,pipe_posi,is_pipe=0,file_pipes[2];
    char command[50],*parameters[5];
    char p1[50],p2[50],p3[50],p4[50],p5[50];
    pid_t pid,pid2;
    while(1){
	is_pipe = 0;
        command[0]='0';
        parameters[0] = p1;parameters[1] = p2;
        parameters[2] = p3;parameters[3] = p4;
        parameters[4] = p5;
        type_prompt();
        read_command(command,parameters);
	//printf("\n%s\n",command);
	if(command[0]=='e'&&command[1]=='x'&&command[2]=='i'&&command[3]=='t'){
		return 0; 
	}
	if(command[0]=='c'&&command[1]=='d'){
		chdir(parameters[1]);
	}
	i=0;
	
	while(i<=word_count -1){
		if(parameters[i]==NULL){break;}	
		if(parameters[i][0]=='|'){
			pipe_posi = i;
			is_pipe = 1;
			parameters[i] = NULL;
			break;
		}
		i+=1;
		}
	if(is_pipe){
		pipe(file_pipes);
		pid = fork();
		if(pid!=0){
				waitpid(pid,0,NULL);
			}
			
			else{
				pipe(file_pipes);
				if(fork()!=0){
				//sprintf("I am read");
			
				close(file_pipes[1]);
				close(0);
				dup(file_pipes[0]);
				execvp(parameters[pipe_posi+1],parameters+pipe_posi+1);
				return 0;
			}
			else{
			//printf("i am writ");
			
			close(file_pipes[0]);
			close(1);
			dup(file_pipes[1]);
			execvp(parameters[0],parameters);
			return 0;}
			}
		
		
	}
	else{
	        if(fork()!=0){
            	wait(NULL);
        	}
        	else{
	    	//printf("hello");
            	execvp(command,parameters);
        	}
	}
    }

}	
