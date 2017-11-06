/*This program is multiplying 2 matrixes by algorithm of standard multiplying:
A[i, j] = (-1) * (i + j);
B[i, j] = i - j;
i, j = 1..100;
Also, this program is calculating labor of alforithm
*/
#include <iostream>

using namespace std;

int T = 0;

int **SMM(int **A, int **B, int n);

int main(int argc, char const *argv[])
{
	int n = 100;
	cout << "n: ";
	cin >> n;

	int i, j;
	int **A = new int *[n];
	int **B = new int *[n];

	for(i = 0; i < n; ++i) {
		A[i] = new int [n];
		B[i] = new int [n];
		for(j = 0; j < n; ++j) {
			A[i][j] = -1 * (i + j);
			B[i][j] = i - j;
		}
	}
	// printing of matrixes
	/*for(i = 0; i < n; ++i) {
		for(j = 0; j < n; ++j) {
			cout.width(5);
			cout << A[i][j];
		}
		cout << endl;
	}
	cout << endl;

	for(i = 0; i < n; ++i) {
		for(j = 0; j < n; ++j) {
			cout.width(5);
			cout << B[i][j];
		}
		cout << endl;
	}
	cout << endl;*/

	int **C = SMM(A, B, n);
	cout << "T = " << T << endl;
	cout << "C" << endl;
	if(n < 16) {
		for(i = 0; i < n; ++i) {
			for(j = 0; j < n; ++j) {
				cout.width(5);
				cout << C[i][j];
			}
			cout << endl;
		}
	} else {
		for(i = 0; i < 4; ++i) {
			for(j = 0; j < 4; ++j) {
				cout.width(8);
				cout << C[i][j];
			}
			cout << "     ...";
			cout.width(8);
			cout << C[i][n-1];
			cout << endl;
		}

		for(i = 0; i < 6; ++i) {
			cout << "     ...";
		}
		cout << endl;

		for(j = 0; j < 4; ++j) {
			cout.width(8);
			cout << C[n-1][j];
		}
		cout << "     ...";
		cout.width(8);
		cout << C[n-1][n-1];
		cout << endl;
	}

	cout << endl;

	system("PAUSE");
	return 0;
}

int **SMM(int **A, int **B, int n) {
	int i, j, k;

	int **C = new int *[n];
	for(i = 0; i < n; ++i) {
		C[i] = new int [n];
		for(j = 0; j < n; ++j) {
			C[i][j] = 0;
		}
	}

	for(i = 0; i < n; ++i) {
		for(j = 0; j < n; ++j) {
			for(k = 0; k < n; ++k) {
				C[i][j] += A[i][k] * B[k][j];
				T += 3;
			}
		}
	}

	return C;
}
