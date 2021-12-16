#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef struct s_cmd{
    int pipe;
    char *cmd[1000];
    int size;
    int fd[2];
}               t_cmd;

size_t ft_strlen(char *str){
    size_t i = 0;
    while (str[i])
        i++;
    return i;
}

void    ft_free(t_cmd *cmds){
    free(cmds);
    exit(1);
}

int fatal(int ret){
    if (ret){
        write(2, "error: fatal\n", 13);
        return (1);
    }
    return (0);
}

void    *ft_memset(void *b, int c, size_t len){
    int i = 0;
    unsigned char *ptr = b;
    while (len--)
        ptr[i++] = (unsigned char)c;
    return b;
}

void    ft_cd(t_cmd *cmds, int index){
    if (cmds[index].size != 2)
        write(2, "error: cd: bad arguments\n", 25);
    else if (chdir(cmds[index].cmd[1]) == -1){
        write(2, "error: cd: cannot change directory to ", 38);
        write(2, cmds[index].cmd[1], ft_strlen(cmds[index].cmd[1]));
        write(2, "\n", 1);
        ft_free(cmds);
    }
}

int main(int argc, char **argv, char **envp){
    int i = 1;
    int j = 0;
    int index = 0;
    t_cmd *cmds = malloc(sizeof(t_cmd)*1000);
    ft_memset(cmds, 0, sizeof(t_cmd)*1000);
    while (argc && argv[i]){
        cmds[index].pipe = 0;
        cmds[index].fd[0] = -1;
        cmds[index].fd[1] = -1;
        if (strcmp(argv[i], "|") == 0 || strcmp(argv[i], ";") == 0){
            if (strcmp(argv[i], "|") == 0)
                cmds[index].pipe = 1;
            if (*cmds[index].cmd)
                index++;
            i++;
            j = 0;
        }
        else{
            cmds[index].size++;
            cmds[index].cmd[j++] = argv[i++];
        }
    }
    index = 0;
    int f = 0;
    int fd[2];
    while (*cmds[index].cmd){
        if (strcmp(*cmds[index].cmd, "cd") == 0)
            ft_cd(cmds, index);
        else{
            if (cmds[index].pipe){
                if (fatal(pipe(fd) == -1))
                    ft_free(cmds);
                cmds[index].fd[1] = fd[1];
                cmds[index+1].fd[0] = fd[0];
            }
            if (fatal((f = fork()) == -1))
                ft_free(cmds);
            if (f == 0){
                if (fatal(cmds[index].fd[1] != -1 && dup2(cmds[index].fd[1], 1) == -1))
                    ft_free(cmds);
                if (fatal(cmds[index].fd[0] != -1 && dup2(cmds[index].fd[0], 0) == -1))
                    ft_free(cmds);
                if (execve(*cmds[index].cmd, cmds[index].cmd, envp) == -1){
                    write(2, "error: cannot execute ", 22);
                    write(2, *cmds[index].cmd, ft_strlen(*cmds[index].cmd));
                    write(2, "\n", 1);
                }
                exit(0);
            }
            if (fatal(cmds[index].fd[1] != -1 && close(cmds[index].fd[1]) == -1))
                ft_free(cmds);
            if (fatal(cmds[index].fd[0] != -1 && close(cmds[index].fd[0]) == -1))
                ft_free(cmds);
        }
        if (!cmds[index].pipe)
            if (fatal(f != 0 && waitpid(f, NULL, 0) == -1))
                ft_free(cmds);
        index++;
    }
    free(cmds);
    return 0;
}