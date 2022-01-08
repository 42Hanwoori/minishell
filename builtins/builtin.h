#ifndef BUILTIN_H
# define BUILTIN_H

# include "./libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	int				print_check;
	struct s_env	*next;
}	t_env;

//builtin_tools.c
int					ft_strcmp(const char *s1, const char *s2);
void				ft_free_split(char **draw);
void				env_lstadd_back(t_env **lst, t_env *new,
						char *key, char *value);
t_env				*env_set(char	**envp);

//builtin1.c
void				cd_error(int	err, char *arg);
void				ft_cd(char **buf, t_env *env_list);
void				ft_env(t_env *env_list);
int					unset_key_syntax_check(char *s);
void				ft_unset(char **dbuf, t_env **env_list);

//builtin2.c
char				*read_value_of_key(t_env *env_list, char *key);
int					ft_echo(char **buf);
void				ft_pwd(char **dbuf);

//builtin_export.c
int					export_key_syntax_check(char *s);
void				reset_env_print_check(t_env *env_list);
void				print_env_in_order(t_env *env_list);
t_env				*env_dup_check(t_env *env_list, char *new_key);
void				ft_export(char **dbuf, t_env *env_list);

//builtin_exit.c
int					valid_exit_arg(char	*arg);
int					exit_numeric_arg_cal(char *arg);
void				exit_extra_cases(int c, char *s);
void				ft_exit(char **dbuf);

#endif
