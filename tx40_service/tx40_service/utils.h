#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <vector>
#include <tuple>

#define PI 3.14156
#define TWOPI PI * 2
#define EPS 0.00001
#define D2R(X) (X * PI / 180)
#define R2D(X) (X * 180 / PI)
#define M2MM(X) (X * 1000)
#define MM2M(X) (X * 0.001)
#define SQR(x) ((x) * (x))

using namespace std;

void showVect(std::vector<double> vect);
void showVals(double x, double y, double z, double rx, double ry, double rz);
std::vector<double> mrad2mmdeg(std::vector<double> pos);
std::vector<double> mmdeg2mrad(std::vector<double> pos);
std::vector<double> value2vector(double x, double y, double z, double rx, double ry, double rz);
std::tuple<double, double, double, double, double, double> vector2value(std::vector<double> vect);
bool IsVectEqual(std::vector<double> vect1, std::vector<double> vect2);

#endif
