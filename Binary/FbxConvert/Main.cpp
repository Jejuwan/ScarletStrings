#include "Main.h"

void main()
{
    Main main;
    main.SaveData();
}

struct BONEDESC {
    unsigned int ParentIndex = { 0xffffffff };
    unsigned int NameLength = { 0 };
    char* Name = { nullptr };
    float TransformMat[16] = {};
};
vector<BONEDESC>   BoneData;


HRESULT SaveBones(aiNode* pAINode, unsigned int Parentindex) {

    BONEDESC BoneDesc;
    ZeroMemory(&BoneDesc, sizeof(BONEDESC));

    BoneDesc.ParentIndex = Parentindex;
    BoneDesc.NameLength = strlen(pAINode->mName.data) + 1;
    BoneDesc.Name = new char[BoneDesc.NameLength];

    strcpy_s(BoneDesc.Name, BoneDesc.NameLength, pAINode->mName.data);

    memcpy(&BoneDesc.TransformMat, &pAINode->mTransformation, sizeof(float) * 16);

    BoneData.push_back(BoneDesc);

    unsigned int MyBoneIndex = 0;

    for (auto& iter : BoneData)
    {
        if (!strcmp(iter.Name, BoneDesc.Name))
            break;
        ++MyBoneIndex;
    }

    for (size_t i = 0; i < pAINode->mNumChildren; ++i)
    {
        if (FAILED(SaveBones(pAINode->mChildren[i], MyBoneIndex)))
            return E_FAIL;
    }
    
    return S_OK;
}

Main::Main() {

}
Main::~Main() {
    /*for (auto& Char : boneName)
        delete[] Char;*/

    for (auto& Desc : BoneData)
        delete[] Desc.Name;
}

HRESULT Main::SaveData()
{

    char filepath[MAX_PATH];
    cout << "file path (if you want out, type 'shutdown'): \t";
    cin >> filepath;

    if (!strcmp(filepath, "shutdown"))
        return 0;

    unsigned int AIFlag = aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded;

    bool isAnim = false;
    cout << "Is file with Anim? : \t";
    cin >> isAnim;

    if (!isAnim)
        AIFlag |= aiProcess_PreTransformVertices;

    m_pAIScene = m_Importer.ReadFile(filepath, AIFlag);
    if (!m_pAIScene)
    {
        cout << "file is not exist!!" << endl;
        return 0;
    }
    wchar_t wFilePath[MAX_PATH];

    cout << "output FilePath : \t";
    wcin >> wFilePath;

    HANDLE hFile = CreateFile(wFilePath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (0 == hFile)
    {
        cout << "Somethine is Wrong, Check output FilePath" << endl;
        return 0;
    }

    unsigned long dwByte = 0;
    // 뼈 상속관계 부르기

    unsigned int ParentIndex = 0xffffffff;

    if (FAILED(SaveBones(m_pAIScene->mRootNode, ParentIndex)))
        return 0;
    unsigned int BoneNum = BoneData.size();
    WriteFile(hFile, &isAnim, sizeof(bool), &dwByte, nullptr);
    WriteFile(hFile, &BoneNum, sizeof(int), &dwByte, nullptr);

    for (auto& Data : BoneData)
    {
        WriteFile(hFile, &Data.ParentIndex, sizeof(int), &dwByte, nullptr);
        WriteFile(hFile, &Data.NameLength, sizeof(int), &dwByte, nullptr);

        for (size_t i = 0; i < Data.NameLength; ++i)
        {
            WriteFile(hFile, &Data.Name[i], 1, &dwByte, nullptr);
        }
        WriteFile(hFile, &Data.TransformMat, sizeof(float) * 16, &dwByte, nullptr);
    }

    //cout << "file path (if you want out, type 'shutdown'): \t";
    //cin >> filepath;
    //
    //if (!strcmp(filepath, "shutdown"))
    //    return 0;
    //
    //AIFlag = aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded;
    //
    //
    //if (!isAnim)
    //    AIFlag |= aiProcess_PreTransformVertices;
    //
    //m_pAIScene = m_Importer.ReadFile(filepath, AIFlag);
    if (!m_pAIScene)
    {
        cout << "file is not exist!!" << endl;
        system("pause");
    }
    // 매쉬 부르기
    m_iNumMeshes = m_pAIScene->mNumMeshes;

    WriteFile(hFile, &m_iNumMeshes, sizeof(int), &dwByte, nullptr);

    for (size_t i = 0; i < m_iNumMeshes; ++i)
    {
        aiMesh* pAIMesh = m_pAIScene->mMeshes[i];



        unsigned int MaterialIndex = pAIMesh->mMaterialIndex;

        unsigned int VertexNum = pAIMesh->mNumVertices;

        WriteFile(hFile, &MaterialIndex, sizeof(int), &dwByte, nullptr);    // 마테리얼 인덱스
        WriteFile(hFile, &VertexNum, sizeof(int), &dwByte, nullptr);        // 버텍스 갯수

        if (!isAnim)
        {
            for (size_t j = 0; j < VertexNum; ++j)
            {
                float vPos[3] = {};
                float vNormal[3] = {};
                float vTex[2] = {};
                float vTan[3] = {};

                memcpy(&vPos, &pAIMesh->mVertices[j], sizeof(float) * 3);
                memcpy(&vNormal, &pAIMesh->mNormals[j], sizeof(float) * 3);
                memcpy(&vTex, &pAIMesh->mTextureCoords[0][j], sizeof(float) * 2);
                memcpy(&vTan, &pAIMesh->mTangents[j], sizeof(float) * 3);

                WriteFile(hFile, &vPos, sizeof(float) * 3, &dwByte, nullptr);
                WriteFile(hFile, &vNormal, sizeof(float) * 3, &dwByte, nullptr);
                WriteFile(hFile, &vTex, sizeof(float) * 2, &dwByte, nullptr);
                WriteFile(hFile, &vTan, sizeof(float) * 3, &dwByte, nullptr);
            }
        }
        else
        {
            for (size_t j = 0; j < VertexNum; ++j)
            {
                float vPos[3] = {};
                float vNormal[3] = {};
                float vTex[2] = {};
                float vTan[3] = {};

                memcpy(&vPos, &pAIMesh->mVertices[j], sizeof(float) * 3);
                memcpy(&vNormal, &pAIMesh->mNormals[j], sizeof(float) * 3);
                memcpy(&vTex, &pAIMesh->mTextureCoords[0][j], sizeof(float) * 2);
                memcpy(&vTan, &pAIMesh->mTangents[j], sizeof(float) * 3);

                WriteFile(hFile, &vPos, sizeof(float) * 3, &dwByte, nullptr);
                WriteFile(hFile, &vNormal, sizeof(float) * 3, &dwByte, nullptr);
                WriteFile(hFile, &vTex, sizeof(float) * 2, &dwByte, nullptr);
                WriteFile(hFile, &vTan, sizeof(float) * 3, &dwByte, nullptr);
            }
            unsigned int BonesNum = pAIMesh->mNumBones;

            if (!BonesNum)
            {
                BonesNum = 1;
                WriteFile(hFile, &BonesNum, sizeof(int), &dwByte, nullptr);  // 매쉬내 뼈 갯수
                float OffsetMat[16] = {
                    1.f,0.f,0.f,0.f,
                    0.f,1.f,0.f,0.f,
                    0.f,0.f,1.f,0.f,
                    0.f,0.f,0.f,1.f
                };
                WriteFile(hFile, &OffsetMat, sizeof(float) * 16, &dwByte, nullptr);    //오프셋 매트릭스
                unsigned int index = 0;
                for (auto& iter : BoneData)
                {
                    if (!strcmp(iter.Name, pAIMesh->mName.data))
                        break;
                    ++index;
                }
                WriteFile(hFile, &index, sizeof(int), &dwByte, nullptr);    //뼈 인덱스
                unsigned int WeightNum = 0;
                WriteFile(hFile, &WeightNum, sizeof(int), &dwByte, nullptr);    //웨이트 정보 갯수
                goto noBones;
            }


            WriteFile(hFile, &BonesNum, sizeof(int), &dwByte, nullptr);  // 매쉬내 뼈 갯수

            for (size_t j = 0; j < BonesNum; ++j)
            {
                aiBone* pAIBone = pAIMesh->mBones[j];
                int index = 0;
                float OffsetMat[16] = {};
                memcpy(&OffsetMat, &pAIBone->mOffsetMatrix, sizeof(float) * 16);
                WriteFile(hFile, &OffsetMat, sizeof(float) * 16, &dwByte, nullptr);    //오프셋 매트릭스


                for (auto& iter : BoneData)
                {
                    if (!strcmp(iter.Name, pAIBone->mName.data))
                        break;
                    ++index;
                }
                WriteFile(hFile, &index, sizeof(int), &dwByte, nullptr);    //뼈 인덱스

                unsigned int WeightNum = pAIBone->mNumWeights;
                WriteFile(hFile, &WeightNum, sizeof(int), &dwByte, nullptr);    //웨이트 정보 갯수


                for (size_t k = 0; k < WeightNum; ++k)
                {
                    unsigned int Vertexid = pAIBone->mWeights[k].mVertexId;
                    unsigned int BlendIndex = j;
                    float BlendWeight = pAIBone->mWeights[k].mWeight;

                    WriteFile(hFile, &Vertexid, sizeof(int), &dwByte, nullptr);
                    WriteFile(hFile, &BlendIndex, sizeof(int), &dwByte, nullptr);
                    WriteFile(hFile, &BlendWeight, sizeof(float), &dwByte, nullptr);
                }
            }
        }
    noBones:

        // 인덱스 정보
        unsigned int IndexNum = pAIMesh->mNumFaces * 3;
        unsigned long* Indices = new unsigned long[IndexNum];
        ZeroMemory(Indices, sizeof(long) * IndexNum);

        unsigned int iIndex = { 0 };
        for (size_t j = 0; j < pAIMesh->mNumFaces; ++j)
        {
            Indices[iIndex++] = pAIMesh->mFaces[j].mIndices[0];
            Indices[iIndex++] = pAIMesh->mFaces[j].mIndices[1];
            Indices[iIndex++] = pAIMesh->mFaces[j].mIndices[2];
        }
        WriteFile(hFile, &IndexNum, sizeof(int), &dwByte, nullptr);
        WriteFile(hFile, Indices, sizeof(long) * IndexNum, &dwByte, nullptr);
        delete[] Indices;
    }

    // 마테리얼 부르기

    unsigned int MaterialNum = m_pAIScene->mNumMaterials;

    WriteFile(hFile, &MaterialNum, sizeof(int), &dwByte, nullptr);

    for (size_t i = 0; i < MaterialNum; ++i)
    {
        aiMaterial* paiMaterial = m_pAIScene->mMaterials[i];

        unsigned int FilePathLength[AI_TEXTURE_TYPE_MAX] = {};
        char MaterialFilePath[AI_TEXTURE_TYPE_MAX][MAX_PATH];
      
        for (size_t j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
        {
            aiString strPath;
            strPath = "";

            if (FAILED(paiMaterial->GetTexture((aiTextureType)j, 0, &strPath)))
                cout << j << endl;
            else
                cout << strPath.data << endl;

            strcpy_s(MaterialFilePath[j], strPath.data);
            FilePathLength[j] = strlen(MaterialFilePath[j]);

            WriteFile(hFile, &FilePathLength[j], sizeof(int), &dwByte, nullptr);

            for (size_t k = 0; k < FilePathLength[j]; ++k)
            {
                WriteFile(hFile, &MaterialFilePath[j][k], 1, &dwByte, nullptr);
            }
        }
    }

    // 애니메이션 정보!!

    unsigned int AnimationNum = m_pAIScene->mNumAnimations;
    /*cout << "How many Animation? \t :";
    unsigned int AnimationNum = 0;
    cin >> AnimationNum;*/

    WriteFile(hFile, &AnimationNum, sizeof(int), &dwByte, nullptr);



    for (size_t i = 0; i < AnimationNum; ++i)
    {

        /*char AnimationFilePath[MAX_PATH] = "";
        cout << "Animation fbxFile Path? \t :";
        cin >> AnimationFilePath;
        AIFlag = aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded;

        m_pAIScene = nullptr;
        m_pAIScene = m_Importer.ReadFile(AnimationFilePath, AIFlag);

        if (!m_pAIScene)
            continue;*/

        aiAnimation* Animation = m_pAIScene->mAnimations[i];
        float Duration = Animation->mDuration;
        float TickPerSecond = Animation->mTicksPerSecond;
        unsigned int ChannelNum = Animation->mNumChannels;

        WriteFile(hFile, &Duration, sizeof(int), &dwByte, nullptr);
        WriteFile(hFile, &TickPerSecond, sizeof(int), &dwByte, nullptr);
        WriteFile(hFile, &ChannelNum, sizeof(int), &dwByte, nullptr);

        for (size_t j = 0; j < ChannelNum; ++j)
        {
            aiNodeAnim* NodeAnim = Animation->mChannels[j];
            unsigned int MyBoneIndex = 0;

            for (auto& iter : BoneData)
            {
                if (!strcmp(iter.Name, NodeAnim->mNodeName.data))
                    break;
                ++MyBoneIndex;
            }

            unsigned int KeyFrameNum = max(max(NodeAnim->mNumPositionKeys, NodeAnim->mNumRotationKeys), NodeAnim->mNumScalingKeys);

            WriteFile(hFile, &MyBoneIndex, sizeof(int), &dwByte, nullptr);
            WriteFile(hFile, &KeyFrameNum, sizeof(int), &dwByte, nullptr);

            float vScale[4] = {};
            float vRotation[4] = {};
            float vPosition[4] = {};
            float Time = { 0 };
            for (size_t k = 0; k < KeyFrameNum; ++k)
            {
                if (k < NodeAnim->mNumScalingKeys)
                {
                    memcpy(&vScale, &NodeAnim->mScalingKeys[k].mValue, sizeof(float) * 3);
                    Time = NodeAnim->mScalingKeys[k].mTime;
                }
                if (k < NodeAnim->mNumRotationKeys)
                {
                    vRotation[0] = NodeAnim->mRotationKeys[k].mValue.x;
                    vRotation[1] = NodeAnim->mRotationKeys[k].mValue.y;
                    vRotation[2] = NodeAnim->mRotationKeys[k].mValue.z;
                    vRotation[3] = NodeAnim->mRotationKeys[k].mValue.w;

                    Time = NodeAnim->mRotationKeys[k].mTime;
                }
                if (k < NodeAnim->mNumPositionKeys)
                {
                    memcpy(&vPosition, &NodeAnim->mPositionKeys[k].mValue, sizeof(float) * 3);
                    Time = NodeAnim->mPositionKeys[k].mTime;
                }
                vScale[3] = 0.f;
                vPosition[3] = 1.f;

                WriteFile(hFile, &vScale, sizeof(float) * 4, &dwByte, nullptr);
                WriteFile(hFile, &vRotation, sizeof(float) * 4, &dwByte, nullptr);
                WriteFile(hFile, &vPosition, sizeof(float) * 4, &dwByte, nullptr);
                WriteFile(hFile, &Time, sizeof(float), &dwByte, nullptr);
            }
        }
    }



    CloseHandle(hFile);


    return S_OK;
}