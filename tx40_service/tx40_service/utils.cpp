#include "utils.h"

/*  Push x, y, z, rx, ry, rz values into a vector
        :param x, y, z, rx, ry, rz  : double, 6 values of 6DOF coord
        :return: vect   : vector<double>, vector of 6DOF coord
    Usage:
        std::vector<double> pos = value2vector(x, y, z, rx, ry, rz);
*/
std::vector<double> value2vector(double x, double y, double z, double rx, double ry, double rz) {
    std::vector<double> vect = {x, y, z, rx, ry, rz};

    return vect;
}

/*  Extract a vector to x, y, z, rx, ry, rz values
        :param pos  : vector<double>, vector of 6DOF position
        :return: x, y, z, rx, ry, rz: double, 6 values of 6DOF coord
    Usage:
        double x, y, z, rx, ry, rz;
        std::tie(x, y, z, rx, ry, rz) = vector2value(vect);
*/
std::tuple<double, double, double, double, double, double> vector2value(std::vector<double> vect) {
    return std::make_tuple(vect[0], vect[1], vect[2], vect[3], vect[4], vect[5]);
}

/*  Show a vector
        :param vect : vector<double>, vector of 6 dimensions
        :return: void
    Usage:
        showVect(vect);
*/
void showVect(std::vector<double> vect) {
    std::cout << "Vector(";
    for (auto i: vect) std::cout << i << "; ";
    std::cout << "\b\b)";

    return ;
}

/*  Show values of x, y, y, rx, ry, rz
        :param x, y, y, rx, ry, rz: double,
        :return: void
    Usage:
        showVect(vect);
*/
void showVals(double x, double y, double z, double rx, double ry, double rz) {
    std::cout << "Values(";
    std::cout << x << "; " << y << "; " << z << "; " << rx << "; " << ry << "; " << rz;
    std::cout << ")";

    return ;
}

/*  Convert a vector in units (meters, radian) to (milimeters, radians)
        :param      pos         : vector<double>, position with (x, y, z) is m unit and (rx, ry, rz) is radian unit
        :return:    mmdeg_pos   : vector<double>, position with (x, y, z) is mm unit and (rx, ry, rz) is degree unit
    Usage:
        std::vector<double> mmdeg_pos = mrad2mmdeg(pos);
*/
std::vector<double> mrad2mmdeg(std::vector<double> pos) {
    std::vector<double> mmdeg_pos = value2vector(M2MM(pos[0]), M2MM(pos[1]), M2MM(pos[2]), R2D(pos[3]), R2D(pos[4]), R2D(pos[5])); // position with (x, y, z) is mm unit and (rx, ry, rz) is degree unit
    return mmdeg_pos;
}


/*  Check whether robot riched the values of position
        :param      pos         : vector<double>, position with (x, y, z) is mm unit and (rx, ry, rz) is degree unit
        :return:    mmdeg_pos   : vector<double>, position with (x, y, z) is m unit and (rx, ry, rz) is radian unit
    Usage:
        std::vector<double> mrad_pos = mmdeg2mrad(pos);
*/
std::vector<double> mmdeg2mrad(std::vector<double> pos) {
    std::vector<double> mmdeg_pos = value2vector(MM2M(pos[0]), MM2M(pos[1]), MM2M(pos[2]), D2R(pos[3]), D2R(pos[4]), D2R(pos[5])); // position with (x, y, z) is mm unit and (rx, ry, rz) is degree unit
    return mmdeg_pos;
}

bool IsVectEqual(std::vector<double> vect1, std::vector<double> vect2) {
	double epsilon = 0.001;
    bool equal = (abs(vect1[0] - vect2[0]) < epsilon) &&
                 (abs(vect1[1] - vect2[1]) < epsilon) &&
                 (abs(vect1[2] - vect2[2]) < epsilon) &&
                 (abs(vect1[3] - vect2[3]) < epsilon) &&
                 (abs(vect1[4] - vect2[4]) < epsilon) &&
                 (abs(vect1[5] - vect2[5]) < epsilon);
    return equal;
}
