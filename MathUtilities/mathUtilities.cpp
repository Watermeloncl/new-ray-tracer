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