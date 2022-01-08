#include "./libft/libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	int				print_check;
	struct s_env	*next;
}	t_env;

int		main(int argc, char **argv, char **envp)
{
	int		MAX_PATH;
	char	*buf;
	t_env	*env_list;
	char	**dbuf;

	env_list = env_set(envp);
	while (1)
	{
		rl_on_new_line();
		buf = readline("minishell$ ");
		dbuf = ft_split(buf, ' ');
		if (ft_strncmp("echo", buf, 4) == 0)
			ft_echo(dbuf);
		else if (ft_strncmp("pwd", buf, 3) == 0)
			ft_pwd(dbuf);
		else if (ft_strncmp("cd", buf, 2) == 0)
			ft_cd(dbuf, env_list);
		else if (ft_strncmp("env", buf, 3) == 0)
			ft_env(env_list);
		else if (ft_strncmp("export", buf, 6) == 0)
			ft_export(dbuf, env_list);
		else if (ft_strncmp("unset", buf, 5) == 0)
			ft_unset(dbuf, &env_list);
		else if (ft_strncmp("exit", buf, 4) == 0)
			ft_exit(dbuf);
		else
			printf("else : %s\n", buf);
		add_history(buf);
		free(buf);
	}
}
