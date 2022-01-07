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

void	ft_free_split(char **draw)
{
	int	i;
	
	i = 0;
	while(draw[i])
		free(draw[i++]);
	free(draw);
}

int		echo(char *buf)
{
	int	i;
	char **dp;

	i = 0;
	dp = ft_split(buf, ' ');
	while (dp[1] != NULL && ft_strncmp(dp[i + 1], "-n", strlen(dp[i + 1])) == 0)
	{
		i++;
		if (dp[i + 1] == NULL)
			break;
	}
	while(dp[++i])
	{
		printf("%s", dp[i]);
		if (dp[i + 1] != 0)
			printf(" ");
	}
	if ((i == 1) || (i > 1 && ft_strncmp(dp[1], "-n", strlen(dp[1])) != 0))
		printf("\n");
	ft_free_split(dp);
	return(0);
}

void	pwd(char *pbuf)
{
	getcwd(pbuf, 4096);
	printf("%s\n", pbuf);
}

void	cd_error(int	err, char *arg)
{
	if (err == EACCES)
		printf("minishell: cd: %s: Permission denied\n", arg);
	else if (err == EFAULT)
		printf("minishell: cd: %s: Path points lie outside the accessible address space\n", arg);
	else if (err == EIO)
		printf("minishell: cd: %s: An I/O error occurred\n", arg);
	else if (err == ELOOP)
		printf("minishell: cd: %s: Too many symbolic links were encountered in the resolving path\n", arg);
	else if (err == ENAMETOOLONG)
		printf("minishell: cd: %s: Path is too long\n", arg);
	else if (err == ENOENT)
		printf("minishell: cd: %s: No such file or directory\n", arg);
	else if (err == ENOTDIR)
		printf("minishell: cd: %s: Not a directory\n", arg);
}

void	cd(char *buf)
{
	char	**args;
	int		err;

	args = ft_split(buf, ' ');
	if (chdir(args[1]) < 0)
	{
		err = errno;
		cd_error(err, args[1]);
	}
	ft_free_split(args);
}

void	env(t_env *env_list)
{
	t_env	*one;

	one = env_list;
	while (one)
	{
		printf("%s=%s\n",one->key, one->value);
		one = one->next;
	}
}

void	env_lstadd_back(t_env **lst, t_env *new)
{
	t_env **phead;

	if (lst == NULL || new == NULL)
		return ;
	phead = lst;
	while (*phead)
		phead = &(*phead)->next;
	new->next = *phead;
	*phead = new;
}

t_env	*env_set(char	**envp)
{
	char	**splits;
	t_env	*temp;
	t_env	*env_list;
	
	env_list = 0;
	while(*envp)
	{
		temp = (t_env *)malloc(sizeof(t_env));
		splits = ft_split(*(envp++), '=');
		temp->key = splits[0];
		temp->value = splits[1];
		temp->print_check = 0;
		env_lstadd_back(&env_list, temp);
	}
	return(env_list);
}

int		env_syntax_check(char *s)
{
	int i;

	i = -1;
	if (s[0] > 47 && s[0] < 58)
	{
		printf("export: not an identifier: %s\n", s);
		return (0);
	}
	while (s[++i])
	{
		if (s[i] == '_')
			continue;
		if (!ft_isalnum(s[i]))
		{
			printf("export: not valid in this context: %s\n", s);
			return (0);
		}
	}
	return (1);
}

// t_env	*find_env_key(t_env *env_list, char *key)
// {

// }

void	reset_env_print_check(t_env *env_list)
{
	t_env	*i;

	i = env_list;
	while (i)
	{
		i->print_check = 0;
		i = i->next;
	}
}

// int		my_strcmp(const char *s1, const char *s2)
// {
// 	if
	
// 	return (ft_strcmp(s1, s2));
// }

void	env_in_order(t_env *env_list)
{
	t_env	*i;
	t_env	*j;

	i = env_list;
	while (i)
	{
		if (i->print_check == 1)
		{
			i = i->next;
			continue ;
		}
		j = i -> next;
		while (j)
		{
			if (my_strcmp(i->key , j->key) > 0 && j->print_check == 0)
				i = j;
			j = j->next;
		}
		printf("%s=%s\n", i->key, i->value);
		i->print_check = 1;
		i = env_list;
	}
	i = env_list;
	reset_env_print_check(env_list);
}

void	export(char *buf, t_env *env_list)
{
	int 	i;
	char 	**splits;
	char	**splits2;
	t_env	*temp;

	i = 0;
	splits = ft_split(buf, ' ');
	if (!splits[1])
		return (env_in_order(env_list));
	while(splits[++i])
	{
		splits2 = ft_split(splits[i], '=');
		if (!env_syntax_check(splits2[0]))
			continue ;
		// if (!env_dup_check(env_list, splits2[0]))
		// {
			

		// 	continue ;
		// }
		temp = (t_env *)malloc(sizeof(t_env));
		temp->key = splits2[0];
		temp->value = splits2[1];
		if (!splits2[1])
			temp->value = ft_strdup("\'\'");
		env_lstadd_back(&env_list, temp);
	}
}

int		main(int argc, char **argv, char **envp)
{
	int		MAX_PATH;
	char	*buf;
	char	pbuf[4096];
	t_env	*env_list;

	env_list = env_set(envp);
	while (1)
	{
		rl_on_new_line();
		buf = readline("minishell$ ");
		if (ft_strncmp("echo", buf, 4) == 0)
			echo(buf);
		else if (ft_strncmp("pwd", buf, 3) == 0)
			pwd(pbuf);
		else if (ft_strncmp("cd", buf, 2) == 0)
			cd(buf);
		else if (ft_strncmp("env", buf, 3) == 0)
			env(env_list);
		else if (ft_strncmp("export", buf, 6) == 0)
			export(buf, env_list);
		else
		{
			printf("else : %s", buf);
			printf("\n");
		}
		add_history(buf);
		free(buf);
	}
}
