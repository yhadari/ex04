/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhadari <yhadari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 13:26:21 by yhadari           #+#    #+#             */
/*   Updated: 2021/12/08 22:48:57 by yhadari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

typedef struct s_cmd{
    
    int pipe;
    char *cmds[100];
}               t_cmd;

size_t ft_strlen(char *str){
    
    size_t i = 0;
    while (str[i])
        i++;
    return i;
}

int cd(int argc, char *arg){
    
    int ret = 0;
    
    if (argc != 3){
        write(2, "error: cd: bad arguments", 24);
        write(2, "\n", 1);
        return 0;
    }
    if ((ret = chdir(arg)) != 0){
        write(2, "error: cd: cannot change directory to ", 38);
        write(2, arg, ft_strlen(arg));
        write(2, "\n", 1);
        return 0;
    }
    return 1;
}

int main(int argc, char **argv, char **envp){
    
    if (strcmp(argv[1], "cd") == 0){
        cd(argc, argv[2]);
    }
    else{
        int i = 1;
        int j = 0;
        int f = 0;
        int index = 0;
        t_cmd *cmd = malloc(sizeof(t_cmd)*100);
        memset(cmd, 0, sizeof(t_cmd)*100);
        while (argv[i]){
            cmd[index].cmds[j++] = argv[i++];
            cmd[index].pipe = 0;
            if (argv[i] && (strcmp(argv[i], ";") == 0 || strcmp(argv[i], "|") == 0)){
                if (strcmp(argv[i], "|") == 0)
                    cmd[index].pipe = 1;
                i++;
                index++;
                j = 0;
            }
        }
        index = 0;
        int std[2];
        int fd[2];
        std[0] = dup(0);
        std[1] = dup(1);
        while (*cmd[index].cmds){
            f = fork();
            if (f == 0){
                if (cmd[index].pipe){
                    pipe(fd);
                    dup2(fd[0], 0);
                    dup2(fd[1], 1);
                }
                else{
                    dup2(std[0], 0);
                    dup2(std[1], 1);
                }
                execve(*cmd[index].cmds, cmd[index].cmds, envp);
            }
            index++;
            if (!cmd[index].pipe)
                waitpid(f, NULL, 0);
        }
    }        
    return 0; 
}