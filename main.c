#include <stdio.h>
#include <inttypes.h>

static const unsigned char BitsSetTable256[256] =
{
#   define B2(n) n,     n+1,     n+1,     n+2
#   define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#   define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
	B6(0), B6(1), B6(1), B6(2)
};

#define baisset(ba, i) ((ba)[(i)>>6] & (1ull << ((i) & 63))) != 0
#define baisclear(ba, i) ((ba)[(i)>>6] & (1ull << ((i) & 63))) == 0
#define baset(ba, i) (ba)[(i)>>6] |= (1ull << ((i) & 63))

#define swap(a, b) temp = (a); (a) = (b); (b) = temp;

typedef struct charbinarr {
	uint64_t arr[2];
}charbinarr;

charbinarr pred[128];
charbinarr succ[128];

int present[128];
char passchars[128];
int nsucc[128];
int nprec[128];
int lenpass;

char threechars[4];
char solution[128];

char validchars[128][128];
int nvalidchars[128];


int searchsolution()
{
	charbinarr insol = { { 0, 0 } };

	for (int i = 0; i < lenpass; i++)
	{
		int found = 0;
		for (int j = 0; !found && j < nvalidchars[i]; j++)
		{
			char c = validchars[i][j];
			if (baisset(insol.arr, c)) continue;
			if ((pred[c].arr[0] & insol.arr[0]) ^ pred[c].arr[0]) continue;
			if ((pred[c].arr[1] & insol.arr[1]) ^ pred[c].arr[1]) continue;
			baset(insol.arr, c);
			solution[i] = c;
			found = 1;
		}
		if (!found) return 0;
	}

	return 1;
}

int init()
{
	int count;
	int c, q;

	for (int z = 0; z < lenpass; z++)
	{
		for (int i = 0; i < lenpass; i++)
		{
			c = passchars[i];
			for (int j = 0; j < lenpass; j++)
			{
				q = passchars[j];
				if (baisset(succ[q].arr, c))
				{
					succ[q].arr[0] |= succ[c].arr[0];
					succ[q].arr[1] |= succ[c].arr[1];
				}
				if (baisset(pred[q].arr, c))
				{
					pred[q].arr[0] |= pred[c].arr[0];
					pred[q].arr[1] |= pred[c].arr[1];
				}
			}
		}
	}

	for (int i = 0; i < lenpass; i++)
	{
		c = passchars[i];

		unsigned char * p = (unsigned char *)(pred[c].arr);
		count = 0;
		for (int j = 0; j < 16; j++)
			count += BitsSetTable256[p[j]];

		nprec[i] = count;


		p = (unsigned char *)(succ[c].arr);
		count = 0;
		for (int j = 0; j < 16; j++)
			count += BitsSetTable256[p[j]];

		nsucc[i] = count;
	}

	for (int i = 0; i < lenpass; i++)
	{
		c = passchars[i];
		int limit = lenpass - nsucc[i];
		for (int j = nprec[i]; j < limit; j++)
		{
			validchars[j][nvalidchars[j]++] = c;
		}
	}


	for (int i = 0; i < lenpass; i++)
	{
		if (nvalidchars[i] == 0) return 1;
		c = passchars[i];
		if (baisset(succ[c].arr, c)) return 1;
		if (baisset(pred[c].arr, c)) return 1;
		if (pred[c].arr[0] & succ[c].arr[0]) return 1;
		if (pred[c].arr[1] & succ[c].arr[1]) return 1;
	}

	return 0;
}


void readandsave()
{
	int t;

	scanf("%d", &t);

	while (t-->0)
	{
		scanf("%s", threechars);
		present[threechars[0]] = 1;
		present[threechars[1]] = 1;
		present[threechars[2]] = 1;
		baset(pred[threechars[1]].arr, threechars[0]);
		baset(pred[threechars[2]].arr, threechars[0]);
		baset(pred[threechars[2]].arr, threechars[1]);
		baset(succ[threechars[0]].arr, threechars[1]);
		baset(succ[threechars[0]].arr, threechars[2]);
		baset(succ[threechars[1]].arr, threechars[2]);
	}

	lenpass = 0;
	for (int c = 33; c < 128; c++)
	{
		if (present[c])
		{
			passchars[lenpass] = c;
			lenpass++;
		}
	}

}

int main()
{
	readandsave();
	int found;
	if (init())
	{
		found = 0;
	}
	else
	{
		found = searchsolution();
	}

	puts(found ? solution : "SMTH WRONG");


	return 0;
}
