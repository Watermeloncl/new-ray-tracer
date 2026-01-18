#ifndef RESOURCES_PARSER_H_
#define RESOURCES_PARSER_H_

#include "..\Objects\Special\sceneInfo.h"

class Parser {
public:
    Parser();
    ~Parser();

    void ParseInput(SceneInfo* sceneInfo);
};

#endif