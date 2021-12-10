#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

typedef struct s_cmd{
    
    int fd[2];
    int pipe;
    int size;
    char *cmds[100];
}               t_cmd;

size_t ft_strlen(char *str){
    
    size_t i = 0;
    while (str[i])
        i++;
    return i;
}

void    cd(t_cmd cmd){
    
    int ret = 0;
    
    if (cmd.size != 2){
        write(2, "error: cd: bad arguments", 24);
        write(2, "\n", 1);
    }
    else if ((ret = chdir(cmd.cmds[1])) != 0){
        write(2, "error: cd: cannot change directory to ", 38);
        write(2, cmd.cmds[1], ft_strlen(cmd.cmds[1]));
        write(2, "\n", 1);
    }
}

int main(int argc, char **argv, char **envp){
    
    int i = 1;
    int j = 0;
    int index = 0;
    t_cmd *cmd = malloc(sizeof(t_cmd)*100);
    memset(cmd, 0, sizeof(t_cmd)*100);
    while (argc && argv[i]){
        cmd[index].pipe = 0;
        cmd[index].fd[0] = -1;
        cmd[index].fd[1] = -1;
        if (strcmp(argv[i], ";") == 0 || strcmp(argv[i], "|") == 0){
            if (strcmp(argv[i], "|") == 0)
                cmd[index].pipe = 1;
            if(*cmd[index].cmds)
                index++;
            i++;
            j = 0;
        }
        else{
            cmd[index].size++;
            cmd[index].cmds[j++] = argv[i++];
        }
    }
    index = 0;
    int ret = 0;
    int f = 0;
    int fd[2];
    while (*cmd[index].cmds){
        if (strcmp(*cmd[index].cmds, "cd") == 0)
            cd(cmd[index]);
        else
        {
            if (cmd[index].pipe){
                pipe(fd);
                cmd[index].fd[1] = fd[1];
                cmd[index+1].fd[0] = fd[0];
            }
            f = fork();
            if (f == 0){
                dup2(cmd[index].fd[1], 1);
                dup2(cmd[index].fd[0], 0);
                if ((ret = execve(*cmd[index].cmds, cmd[index].cmds, envp)) == -1){
                    write(2, "error: cannot execute ", 22);
                    write(2, *cmd[index].cmds, ft_strlen(*cmd[index].cmds));
                    write(2, "\n", 1);
                }
                exit(0);
            }
            close(cmd[index].fd[1]);
            close(cmd[index].fd[0]);
        }
        index++;
        if (!cmd[index].pipe)
            waitpid(f, NULL, 0);
    }    
    return 0; 
}