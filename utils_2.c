#include <stdlib.h>

void	set_range_int(int *to_set, int floor, int ceil)
{
	if (*to_set < floor)
		*to_set = floor;
	else if (*to_set > ceil)
		*to_set = ceil;
}

void	set_range_double(double *to_set, double floor, double ceil)
{
	if (*to_set < floor)
		*to_set = floor;
	else if (*to_set > ceil)
		*to_set = ceil;
}

int	exit_game(int exit_status)
{
	exit(exit_status);
}
