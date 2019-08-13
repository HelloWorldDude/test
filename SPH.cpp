#include <stdio.h>
#include <cstdlib>
#include <vector>

#define MAX_PARTICLES 5

struct Particle
{
	int a; // Acceleration

	int x;
	int y;

	int v1;
	int v2;
};

std::array<Particle, MAX_PARTICLES> particles;

void init_particles()
{
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		
	}
}

void dump_particles()
{
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		printf("a - %d, x - (%d, %d), v - (%d, %d)\n",
			particles[i].a,
			particles[i].x, particles[i].y,
			particles[i].v1, particles[i].v2);
	}
}

char *find_neighbors(Particle p)
{
	int x = p.x + 1;
	int y = p.y + 1;
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		if ((particles[i].x > (x - 1) && particles[i].x <= x)
			&& particles[i].y > (y - 1) && particles[i].y <= y)
		{

		}
	}
	return nullptr;
}


int main()
{
	init_particles();
	dump_particles();
}