all: main

main: main.cpp
	g++ -Wall -Wconversion -o main.exe main.cpp Display\display.cpp MainFunctions\mainFunctions.cpp MainFunctions\worker.cpp MathUtilities\mathUtilities.cpp Objects\Lights\directionalLight.cpp Objects\Lights\genericLight.cpp Objects\SceneObjects\genericObject.cpp Objects\SceneObjects\material.cpp Objects\SceneObjects\sphere.cpp Objects\Special\sceneInfo.cpp Objects\Special\threadBuffer.cpp Resources\parser.cpp Scheduler\scheduler.cpp -ld2d1 -lgdi32 -luser32 -lm