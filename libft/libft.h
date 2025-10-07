/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 14:52:53 by aheisch           #+#    #+#             */
/*   Updated: 2025/03/09 01:43:36 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>

int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
int					ft_isascii(int c);
int					ft_isprint(int c);
int					ft_toupper(int c);
int					ft_tolower(int c);

size_t				ft_strlen(const char *s);
char				*ft_strncpy(char *dest, const char *src, int n);
size_t				ft_strlcpy(char *dst, const char *src, size_t dsize);
size_t				ft_strlcat(char *dst, const char *src, size_t dsize);
char				*ft_strchr(const char *s, int c);
char				*ft_strnchr(const char *s, int c, size_t n);
char				*ft_strrchr(const char *s, int c);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strnstr(const char *big, const char *little,
						size_t len);
char				*ft_strdup(const char *s);
char				*ft_strndup(const char *src, unsigned int n);

void				*ft_memset(void *s, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
void				ft_bzero(void *s, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
void				*ft_memchr(const void *s, int c, size_t n);
void				*ft_memrchr(const void *s, int c, size_t n);

int					ft_atoi(const char *nptr);
long				ft_atol(const char *nptr);
unsigned long		ft_atoul(const char *nptr);
void				*ft_calloc(size_t nmemb, size_t size);

char				*ft_substr(const char *s, unsigned int start, size_t len);
char				*ft_strjoin(const char *s1, const char *s2);
char				*ft_strtrim(const char *s1, const char *set);
char				**ft_split(const char *s, char c);
char				*ft_itoa(int n);
char				*ft_ntoa(long long n, const char *base);
char				*ft_untoa(unsigned long long n, const char *base);
char				*ft_strmapi(const char *s, char (*f)(unsigned int, char));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
void				ft_putchar_fd(char c, int fd);
long				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *el);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *el);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstremove(t_list **lst, void *el, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));
t_list				*ft_lstpop_back(t_list **lst);
t_list				*ft_lstpop_front(t_list **lst);
t_list				*ft_lstget_nth(t_list *lst, size_t n);
t_list				*ft_lstcopy_shallow(t_list *src);

typedef struct s_string
{
	char			*content;
	size_t			length;
	size_t			capacity;
}					t_string;

t_string			ft_string_new(void);
t_string			ft_string_from(char *s);
t_string			*ft_string_new_ptr(void);
void				ft_string_move(t_string *from, t_string *to);
t_string			ft_string_destroy(t_string *str);
void				*ft_string_delete(t_string **str);
char				*ft_string_into(t_string *str);
char				*ft_string_get(t_string *str);
void				ft_string_set(t_string *str, const char *content);
t_string			*ft_string_ncat(t_string *str, const char *cat, size_t n);
t_string			*ft_string_cat(t_string *str, const char *cat);
t_string			*ft_string_cats(t_string *str, const char **cats);
t_string			*ft_string_cat_free(t_string *str, char *cat);

int					ft_printf(const char *fmt, ...);

t_string			ft_read_file(int fd);
char				*ft_get_next_line(int fd);

#endif // !LIBFT_H
