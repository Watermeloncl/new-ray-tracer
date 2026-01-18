#include <cmath>

#include "mathUtilities.h"
#include "..\config.h"

unsigned char MathUtilities::ColorAmp(double percent) {
    if(percent > 1) {
        percent = 1;
    } else if(percent < 0) {
        percent = 0;
    }

    return (unsigned char)(percent * MAX_COLOR);
}

void MathUtilities::Normalize(double& dx, double& dy, double& dz) {
    double mag = 1.0 / std::sqrt(dx*dx + dy*dy + dz*dz);
    dx *= mag;
    dy *= mag;
    dz *= mag;
}

double MathUtilities::DotProduct(double x1, double y1, double z1, double x2, double y2, double z2) {
    return (x1 * x2) + (y1 * y2) + (z1 * z2);
}
