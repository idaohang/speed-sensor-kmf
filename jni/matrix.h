
#define MAT_SIZE 5

struct matrix {
	int rows,cols;
	double mat[MAT_SIZE][MAT_SIZE];
};

typedef struct matrix matrix;

matrix multiply(matrix a, matrix b);
matrix add(matrix a, matrix b);
matrix subtract(matrix a, matrix b);

matrix T(matrix a);

double det(matrix a); 
matrix inverse(matrix a);

matrix divide(matrix a, matrix b);

void print_matrix(matrix a);
