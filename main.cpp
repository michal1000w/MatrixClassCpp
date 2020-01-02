#include <iostream>
#include <vector>

#include "Matrix.h"

using namespace std;

int main() {

	Matrix<double> m(5,5);
	Matrix<double> m1("[0,3,2][1,2,3]");
	
	for (yeet i = 0; i < 5; i++)
		for (yeet j = 0; j < 5; j++)
			m(i,j) = i * j;


	//m.print();
	//m1.print();

	m.add("[1,2,3][4,5,6][7,8,9]");
	//m.print();

	for (int i = 0; i < 1000000; i++) {
		Matrix<double> m2(5, 5);
		m = m + m2.expa().sigmoid().sigmoid_derivative();
		//m.add("[1,2,3][4,5,6][7,8,9]");
	}
	m.print();
	m = m.sigmoid();
	m.print();

	cout << endl;
	m.add("[1,2,3][4,5,6]");
	m = m.T();
	m.print();

	
	system("pause");
}