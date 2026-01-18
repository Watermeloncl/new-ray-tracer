#ifndef CONFIG_H_
#define CONFIG_H_

#define OUT_FILE_NAME "Resources\\Outputs\\test0.ppm"
#define IN_FILE_NAME "Resources\\Inputs\\scene1.txt"

const bool OUT_TO_FILE_FLAG = true;
const bool SHOW_ON_SCREEN_FLAG = true;

const int CLIENT_SCREEN_WIDTH = 1280;
const int CLIENT_SCREEN_HEIGHT = 720;

const int MIN_CLIENT_SCREEN_WIDTH = 256;
const int MIN_CLIENT_SCREEN_HEIGHT = 72;

const int NUM_WORKER_THREADS = 10; //keep in mind divisibility.

const int MAX_OBJECTS = 32;
const int MAX_LIGHTS = 4;


#endif