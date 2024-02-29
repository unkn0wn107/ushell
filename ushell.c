#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FATAL_ERR exit(ft_puterr("fatal", NULL))

int	ft_puterr(char *msg, char *arg)
{
	write(2, "error: ", 7);
	while(*msg)
		write(2, msg++, 1);
	if(arg)
		while(*arg)
			write(2, arg++, 1);
	write(2, "\n", 1);
	return (1);
}

int	ft_execute(char **argv, int i, char **envp)
{
	int	pid;
	int	status;
	int	fd[2];
	int	is_pipe;

	status = 0;
	is_pipe = argv[i] && strcmp(argv[i], "|") == 0;
	if (is_pipe && pipe(fd) == -1)
		FATAL_ERR;
	pid = fork();
	if (pid == -1)
		FATAL_ERR;
	else if (pid == 0)
	{
		argv[i] = NULL;
		if (is_pipe && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1] == -1)))
			FATAL_ERR;
		if (execve(*argv, argv, envp) == -1)
			ft_puterr("cannot execute ", *argv);
		return (WIFEXITED(status) && WEXITSTATUS(status));
	}

	if (waitpid(pid, &status, 0) == -1)
		FATAL_ERR;
	if (is_pipe && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
		FATAL_ERR;
	return (status);
}

int	main(int argc, char **argv, char **envp)
{
	int	ret;
	int	i;
       
	ret = 0;
	i = 0;
	while (argc > 1 && argv[i] && argv[++i])
	{
		argv += i;
		i = 0;
		while (argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"))
			i++;
		if (strcmp(*argv, "cd") == 0)
		{
			if (i != 2)
				ft_puterr("cd: bad arguments", NULL);
			else if (chdir(argv[1]) == -1)
				ft_puterr("cd: cannot change directory to ", argv[i]);
			ret = 0;
		}
		else
			ret = ft_execute(argv, i, envp);
	}
	return (ret);
}
