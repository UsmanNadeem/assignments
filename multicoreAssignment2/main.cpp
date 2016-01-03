#include <omp.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <limits>  
#include <thread>
#include <stdlib.h>
using namespace std;

class Body;
const long double G = 6.67408e-11;  // 6.67408 × 10-11 m3 kg-1 s-2
const long double COLLISION_DISTANCE = 1;

double dt, errorThreshold;
int numIterations, numObjects;
vector<Body*> bodies;

#include "classes.cpp"
#include "one-two.cpp"
#include "three.cpp"
#include "four.cpp"

void RANDOM_TEST(char const *argv[]) {
	srand(1);
	dt = 10;
	errorThreshold = 0;
	numObjects = stoi(argv[1]);
	numIterations = stoi(argv[2]);
	
	for (int i = 0; i < numObjects; ++i) {
		double vx,vy,px,py,m;
	
		// vx = 0;
		// vy = 0;
		// px = 100*i;
		// py = 100*i;
		// m = 100000000000;

		vx = rand() % 300 -100;
		vy = rand() % 300 -100;
		px = rand() % 20000 +1000;
		py = rand() % 20000 +1000;
		m = rand() % 2000000 +1;
		bodies.push_back(new Body(vx,vy,px,py,m));
	}
}

void REAL_INPUT() {
	cout << "Enter time interval (dt): ";
	cin >> dt;
	cout << "Enter number of iterations to simulate: ";
	cin >> numIterations;
	cout << "Enter the error threshold: ";
	cin >> errorThreshold;
	cout << "Enter the number of objects to simulate: ";
	cin >> numObjects;

	for (int i = 0; i < numObjects; ++i) {
		double vx,vy,px,py,m;
		cout << "Enter the initial Xvelocity of Body " << i+1 << ": ";
		cin >> vx;
		cout << "Enter the initial Yvelocity of Body " << i+1 << ": ";
		cin >> vy;
		cout << "Enter the initial Xposition of Body " << i+1 << ": ";
		cin >> px;
		cout << "Enter the initial Yposition of Body " << i+1 << ": ";
		cin >> py;
		cout << "Enter the mass of Body " << i+1 << ": ";
		cin >> m;
		bodies.push_back(new Body(vx,vy,px,py,m));
	}
}
int main(int argc, char const *argv[]) {
	
	// REAL_INPUT();
	RANDOM_TEST(argv);

	switch(stoi(argv[3])) {
		case 1: {
			FIRSTPART();
			break;
		}
		case 2: {
			SECONDPART();
			break;
		}
		case 3: {
			THIRDPART();
			break;
		}
		case 4: {
			FOURTHPART();
			break;
		}
	}
	
	

	// FIRSTPART();
	// SECONDPART();
	// THIRDPART();
	// FOURTHPART();


	return 0;
}
