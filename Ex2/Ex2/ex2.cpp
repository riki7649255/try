#include<stdio.h>
#include<malloc.h>

#define N 4
#define M 6

//			x1	x2	x3	x4	x5	b
//	maxZ	-1	2	0	0	0	0
//	x3		-1	-1	1	0	0	-1
//	x4		1	0	0	1	0	2
//	x5		0	1	0	0	1	2
int findMinZcolumn(float st[N][M],int numCol)
{
	int minCol = st[0][0], colIndex = 0;
	for (int i = 1; i < numCol; i++) //find min maxZ column
	{
		if (st[0][i] < minCol)
		{
			minCol = st[0][i];
			colIndex = i;
		}
	}
	return colIndex;
}

void simplex(float st[N][M],int *b, int numVars, int numST)
{
	int maxZ[2];
	maxZ[0] = st[0][0];
	maxZ[1] = st[0][1];
	int colVars[3];
	colVars[0] = 3;
	colVars[1] = 4;
	colVars[2] = 5;
	//Adding idle variables
	for (int i = 0; i < numST+1; i++)//allocate for idle vars more memory
	{
		//initialize all new cells with zero
		for (int j = numVars; j < numVars+numST; j++)
		{
			st[i][j] = 0;
		}
	}
	for (int i = 0; i < numST+1; i++)
	{//set the idle var
		if(i!=0)
			st[i][numVars + i-1] = 1;
	}
	//adding the b column
	for (int i = 0; i < numST+1; i++)
	{
		st[i][M-1] = b[i];
	}
	int numCol = M;
	int numRow = N;
	int colIndex = findMinZcolumn(st, numCol);

	while(st[0][colIndex] < 0) //the min in maxZ line has2b negative number
	{
		int minRow = 1000000;
		int rowIndex;
		for (int i = 1; i < numRow; i++)//find min result
		//from dividing b column with min maxZ column row
		{
			if (st[i][colIndex] > 0)
			{
				if ((st[i][numCol - 1] / st[i][colIndex]) < minRow)
				{
					minRow = (st[i][numCol - 1] / st[i][colIndex]);
					rowIndex = i;
				}
			}
		}
		//switch between row&column
		colVars[rowIndex-1] = colIndex + 1;
		//set all column cells in chosen row-column to zero excepr itself
		float div;
		for (int i = 0; i < numRow; i++)
		{
			if (i != rowIndex)
			{
				div = st[i][colIndex] / (-1 * (st[rowIndex][colIndex]));
				for (int j = 0; j < numCol; j++)
				{
					st[i][j] += div * st[rowIndex][j];
				}
			}
		}
		colIndex = findMinZcolumn(st, numCol);
	}
	for (int i = 0; i < numRow; i++)
	{
		for (int j = 0; j < numCol; j++)
		{
			printf("%1f	 |",st[i][j]);
		}
		printf("\n");
	}
	float opt = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 1; j <= numVars; j++)
		{
			if (colVars[i] == j)
			{
				printf("x%d: %f,", j, st[i + 1][numCol - 1] / st[i + 1][j - 1]);
				opt += (maxZ[colVars[i]-1]*-1) * (st[i + 1][numCol - 1] / st[i + 1][j - 1]);
			}
		}
	}
	printf("\noptimum solution: %f\n",opt);

}

void main()
{
	float mat[N][M];
	mat[0][0] = -3;
	mat[0][1] = -5;
	mat[1][0] = 1;
	mat[1][1] = 0;
	mat[2][0] = 0;
	mat[2][1] = 2; 
	mat[3][0] = 3;
	mat[3][1] = 2;
	int *b = (int*)malloc(sizeof(int) * 4);
	b[0] = 0;
	b[1] = 4;
	b[2] = 12;
	b[3] = 18;
	simplex(mat, b, 2, 3);
}