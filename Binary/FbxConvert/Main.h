#pragma once


#include <d3d11.h>

#include <process.h>
#include <iostream>
#include <vector>

#ifdef min
#undef min
#undef max
#endif

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


using namespace std;

class Main
{
public:
    Main();
    ~Main();

    const aiScene* m_pAIScene = { nullptr };
    Assimp::Importer m_Importer;

    unsigned int m_iNumMeshes = { 0 };

    HRESULT SaveData();
};

