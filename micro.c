/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhadari <yhadari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 13:26:21 by yhadari           #+#    #+#             */
/*   Updated: 2021/12/07 20:49:05 by yhadari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
        int index = 0;
        char *path = NULL;
        char *cmd[100][100];
        while (argv[i]){
            cmd[index][j++] = argv[i++];
            if (argv[i] && strcmp(argv[i], ";") == 0){
                i++;
                index++;
                j = 0;
            }
        }
        index = 0;
        int f;
        while (*cmd[index]){
            path = *cmd[index];
            f = fork();
            if (f == 0){
                execve(path, cmd[index], envp);
                exit(0);
            }
            index++;
        }
    }        
    return 0; 
}