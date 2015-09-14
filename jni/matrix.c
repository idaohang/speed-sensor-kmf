#include "matrix.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

double determinant(double a[MAT_SIZE][MAT_SIZE], int n);


void print_matrix(matrix a) {
	int i,j;
	for (i = 0; i < a.rows; i++)
	{
		  for (j = 0; j < a.cols; j++)
		  {
				printf("%2.5f ", a.mat[i][j]);
		  }
		  printf("\n");
	 }
	 printf("\n");
}

matrix multiply_by_number(matrix a, double b) {
	matrix res = {0,0,{0}};
	int row_a, col_a;
	for ( row_a = 0 ; row_a < a.rows ; row_a++ ) {
		for ( col_a = 0 ; col_a < a.cols ; col_a++ ) {

			res.mat[row_a][col_a] = a.mat[row_a][col_a]*b;
		}
	}
	res.rows = a.rows;
	res.cols = a.cols;
	return res;
}

matrix multiply(matrix a, matrix b) {
	matrix res = {0,0,{0}};
	double sum = 0;
	int row_a, col_b, index;
	for ( row_a = 0 ; row_a < a.rows ; row_a++ ) {
		for ( col_b = 0 ; col_b < b.cols ; col_b++ ) {
			for ( index = 0 ; index < a.cols ; index++ ) {
				sum = sum + a.mat[row_a][index]*b.mat[index][col_b];
			} 
			res.mat[row_a][col_b] = sum;
			sum = 0;
		}
	}
	res.rows = a.rows;
	res.cols = b.cols;
	return res;
}

matrix add(matrix a, matrix b) {
	matrix result = {a.rows, a.cols,{0}};
	int row, col;
	for ( row=0; row<a.rows; row++) {
		for ( col=0; col < a.cols; col++) {
			result.mat[row][col] = a.mat[row][col] + b.mat[row][col];
		}
	}
	return result;
}

matrix subtract(matrix a, matrix b) {
	matrix result = {a.rows, a.cols,{0}};
	int row, col;
	for ( row=0; row<a.rows; row++) {
		for ( col=0; col < a.cols; col++) {
			result.mat[row][col] = a.mat[row][col] - b.mat[row][col];
		}
	}
	return result;
}

matrix T(matrix a) {
	matrix result = {a.cols, a.rows,{0}};
	int row, col;
	for (row=0; row<a.rows; row++) {
		for (col=0; col < a.cols; col++) {
			result.mat[col][row] = a.mat[row][col];
		}
	}
	return result;
}

matrix divide(matrix a, matrix b) {
	return multiply(a, inverse(b));
}

double det(matrix a) {
	return determinant(a.mat, a.cols);
}

double determinant(double a[MAT_SIZE][MAT_SIZE], int n)
{
	int i,j,j1,j2;
	double det = 0;
	double m[MAT_SIZE][MAT_SIZE] = {0};

	if (n < 1) { /* Error */

	} else if (n == 1) { /* Shouldn't get used */
	  det = a[0][0];
	} else if (n == 2) {
	  det = a[0][0] * a[1][1] - a[1][0] * a[0][1];
	} else {
	  det = 0;
	  for (j1=0;j1<n;j1++) {
		 for (i=1;i<n;i++) {
			j2 = 0;
			for (j=0;j<n;j++) {
				if (j == j1)
				  continue;
				m[i-1][j2] = a[i][j];
				j2++;
			}
		 }
		 det += pow(-1.0,j1+2.0) * a[0][j1] * determinant(m,n-1);
		 //for (i=0;i<n-1;i++)
			//free(m[i]);
		 //free(m);
	  }
	}
	return det;
}

/*
	Find the cofactor matrix of a square matrix
*/
matrix inverse(matrix a)
{
	int i,j,ii,jj,i1,j1;
	double ldet;
	double deter;
	matrix b;
	double c[MAT_SIZE][MAT_SIZE];
	int n = a.rows;

	for (j=0;j<n;j++) {
	  for (i=0;i<n;i++) {

		 /* Form the adjoint a_ij */
		 i1 = 0;
		 for (ii=0;ii<n;ii++) {
			if (ii == i)
				continue;
			j1 = 0;
			for (jj=0;jj<n;jj++) {
				if (jj == j)
				  continue;
				c[i1][j1] = a.mat[ii][jj];
				j1++;
			}
			i1++;
		 }

		 /* Calculate the determinate */
		 ldet = determinant(c,n-1);

		 /* Fill in the elements of the cofactor */
		 b.mat[i][j] = pow(-1.0,i+j+2.0) * ldet;
	  }
	}
	//end of cofactor calculation
	b.rows = a.rows;
	b.cols = a.cols;
	b = T(b);
	deter = det(a);
	b = multiply_by_number(b,1/deter);
	return b;
}

