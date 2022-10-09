#include "../cub3d.h"

void	exit_with_err(char *custom_msg, int exit_code)
{
	printf("ERROR\n:");
	if (custom_msg != NULL)
		printf("%s\n", custom_msg);
	else
		perror(NULL);
	exit(exit_code);
}
