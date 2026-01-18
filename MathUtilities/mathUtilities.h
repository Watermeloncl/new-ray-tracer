#ifndef MATH_UTILITIES_H_
#define MATH_UTILITIES_H_

class MathUtilities {
public:
    static unsigned char ColorAmp(double percent);
    static void Normalize(double& dx, double& dy, double& dz);
private:
    MathUtilities() = default;
};

#endif