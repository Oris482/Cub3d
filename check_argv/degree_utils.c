#define PI 3.141592

double	deg2rad(double degree)
{
	return ((degree) * PI / 180.0); 
}

double	adjust_degree(double base_degree, double offset_degree)
{
	double	ret_degree;

	ret_degree = base_degree + offset_degree;
	if (ret_degree >= 360.0)
		return (ret_degree - 360.0);
	if (ret_degree < 0)
		return (ret_degree + 360.0);
	return (ret_degree);
}