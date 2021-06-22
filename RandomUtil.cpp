#include <stdlib.h> 

#include "RandomUtil.h"

double unitRandom()
{
	return ((double)rand()/((double)(RAND_MAX)+(double)(1)));
}

float randomFloat(float lower, float upper)
{
	return lower + (upper - lower) * unitRandom();
}

int randomInt(int lower, int upper)
{
	return lower + (upper - lower + 1) * unitRandom();
}