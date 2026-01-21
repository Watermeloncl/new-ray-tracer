#ifndef CONFIG_H_
#define CONFIG_H_

#define OUT_FILE_NAME "Resources\\Outputs\\turnin1c.ppm"
#define IN_FILE_NAME "Resources\\Inputs\\scene4.txt"

const bool OUT_TO_FILE_FLAG = true;
const bool SHOW_ON_SCREEN_FLAG = true;

const int CLIENT_SCREEN_WIDTH = 1280;
const int CLIENT_SCREEN_HEIGHT = 720;

const int CLIENT_WORLD_RATIO_WIDTH = 16;
const int CLIENT_WORLD_RATIO_HEIGHT = 9;

const int MIN_CLIENT_SCREEN_WIDTH = 256;
const int MIN_CLIENT_SCREEN_HEIGHT = 72;

const int NUM_WORKER_THREADS = 10; //keep in mind divisibility.
const int SUPER_SAMPLING_AMP = 5; // 1 = 1x1 (1 ray); 4 = 4x4 (16 ray) etc

const int MAX_OBJECTS = 32;
const int MAX_LIGHTS = 4;

const int MAX_COLOR = 255;
const double RAY_EPSILON = 0.000001;
const double MAX_RAY_DEPTH = 12;

enum class ObjectType {
    SPHERE,
    POLYGON,
};

enum class LightType {
    DIRECTIONAL,
    POINT,
};

#endif
