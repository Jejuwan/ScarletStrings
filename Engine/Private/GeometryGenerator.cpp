#include "EnginePCH.h"
#include "GeometryGenerator.h"

std::tuple<std::vector<VTXPOSNORTEX>, std::vector<_ushort>> CGeometryGenerator::CreateCube_8()
{
    //vertices
    std::vector<VTXPOSNORTEX> vecVertices;
    vecVertices.reserve(8);
    vecVertices.emplace_back(VTXPOSNORTEX{ XMFLOAT3(0.5f, 0.5f, 0.5f),   XMFLOAT3(3, 1, 2), XMFLOAT2(0, 0) });
    vecVertices.emplace_back(VTXPOSNORTEX{ XMFLOAT3(-0.5f, 0.5f, 0.5f),  XMFLOAT3(-3, 1, 2), XMFLOAT2(1, 0) });
    vecVertices.emplace_back(VTXPOSNORTEX{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(-3, -1, 2), XMFLOAT2(1, 1) });
    vecVertices.emplace_back(VTXPOSNORTEX{ XMFLOAT3(0.5f, -0.5f, 0.5f),  XMFLOAT3(3, -1, 2), XMFLOAT2(0, 1) });
    vecVertices.emplace_back(VTXPOSNORTEX{ XMFLOAT3(0.5f, 0.5f, -0.5f),  XMFLOAT3(3, 1, -2), XMFLOAT2(0, 0) });
    vecVertices.emplace_back(VTXPOSNORTEX{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(3, -1, -2), XMFLOAT2(1, 0) });
    vecVertices.emplace_back(VTXPOSNORTEX{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(-3, -1, -2), XMFLOAT2(1, 1) });
    vecVertices.emplace_back(VTXPOSNORTEX{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(-3, 1, -2), XMFLOAT2(0, 1) });
    
    for (auto& vertex : vecVertices)
    {
        _vector vNormal = XMVector3Normalize(XMLoadFloat3(&vertex.vNormal));
        XMStoreFloat3(&vertex.vNormal, vNormal);
    }

    std::vector<_ushort> vecIndices;
    vecIndices.reserve(12 * 3);
    vecIndices.insert(vecIndices.end(),
        {
            0, 1, 2,
            0, 2, 3,
            4, 5, 6,
            4, 6, 7,
            4, 7, 1,
            4, 1, 0,
            7, 6, 2,
            7, 2, 1,
            6, 5, 3,
            6, 3, 2,
            5, 4, 0,
            5, 0, 3
        });

    return std::tuple{ vecVertices, vecIndices };
}

std::tuple<std::vector<VTXPOSNORTEX>, std::vector<_ushort>> CGeometryGenerator::CreateCube_24()
{
    std::vector<XMFLOAT3> vecPositions;
    std::vector<XMFLOAT3> vecNormals;
    std::vector<XMFLOAT2> vecTexcoord; // ÅØ½ºÃç ÁÂÇ¥

    // À­¸é
    vecPositions.push_back(XMFLOAT3(-0.5f, 0.5f, -0.5f));
    vecPositions.push_back(XMFLOAT3(-0.5f, 0.5f, 0.5f));
    vecPositions.push_back(XMFLOAT3(0.5f, 0.5f, 0.5f));
    vecPositions.push_back(XMFLOAT3(0.5f, 0.5f, -0.5f));
    vecNormals.push_back(XMFLOAT3(0.0f, 1.0f, 0.0f));
    vecNormals.push_back(XMFLOAT3(0.0f, 1.0f, 0.0f));
    vecNormals.push_back(XMFLOAT3(0.0f, 1.0f, 0.0f));
    vecNormals.push_back(XMFLOAT3(0.0f, 1.0f, 0.0f));
    vecTexcoord.push_back(XMFLOAT2(0.0f, 1.0f)); // ÁÂÇÏ´Ü
    vecTexcoord.push_back(XMFLOAT2(0.0f, 0.0f)); // ÁÂ»ó´Ü
    vecTexcoord.push_back(XMFLOAT2(1.0f, 0.0f)); // ¿ì»ó´Ü
    vecTexcoord.push_back(XMFLOAT2(1.0f, 1.0f)); // ¿ìÇÏ´Ü

    // ¾Æ·§¸é
    vecPositions.push_back(XMFLOAT3(-0.5f, -0.5f, -0.5f));
    vecPositions.push_back(XMFLOAT3(0.5f, -0.5f, -0.5f));
    vecPositions.push_back(XMFLOAT3(0.5f, -0.5f, 0.5f));
    vecPositions.push_back(XMFLOAT3(-0.5f, -0.5f, 0.5f));
    vecNormals.push_back(XMFLOAT3(0.0f, -1.0f, 0.0f));
    vecNormals.push_back(XMFLOAT3(0.0f, -1.0f, 0.0f));
    vecNormals.push_back(XMFLOAT3(0.0f, -1.0f, 0.0f));
    vecNormals.push_back(XMFLOAT3(0.0f, -1.0f, 0.0f));
    vecTexcoord.push_back(XMFLOAT2(0.0f, 1.0f)); // ÁÂÇÏ´Ü
    vecTexcoord.push_back(XMFLOAT2(0.0f, 0.0f)); // ÁÂ»ó´Ü
    vecTexcoord.push_back(XMFLOAT2(1.0f, 0.0f)); // ¿ì»ó´Ü
    vecTexcoord.push_back(XMFLOAT2(1.0f, 1.0f)); // ¿ìÇÏ´Ü

    // ¾Õ¸é
    vecPositions.push_back(XMFLOAT3(-0.5f, -0.5f, -0.5f));
    vecPositions.push_back(XMFLOAT3(-0.5f, 0.5f, -0.5f));
    vecPositions.push_back(XMFLOAT3(0.5f, 0.5f, -0.5f));
    vecPositions.push_back(XMFLOAT3(0.5f, -0.5f, -0.5f));
    vecNormals.push_back(XMFLOAT3(0.0f, 0.0f, -1.0f));
    vecNormals.push_back(XMFLOAT3(0.0f, 0.0f, -1.0f));
    vecNormals.push_back(XMFLOAT3(0.0f, 0.0f, -1.0f));
    vecNormals.push_back(XMFLOAT3(0.0f, 0.0f, -1.0f));
    vecTexcoord.push_back(XMFLOAT2(0.0f, 1.0f)); // ÁÂÇÏ´Ü
    vecTexcoord.push_back(XMFLOAT2(0.0f, 0.0f)); // ÁÂ»ó´Ü
    vecTexcoord.push_back(XMFLOAT2(1.0f, 0.0f)); // ¿ì»ó´Ü
    vecTexcoord.push_back(XMFLOAT2(1.0f, 1.0f)); // ¿ìÇÏ´Ü

    // µÞ¸é
    vecPositions.push_back(XMFLOAT3(-0.5f, -0.5f, 0.5f));
    vecPositions.push_back(XMFLOAT3(0.5f, -0.5f, 0.5f));
    vecPositions.push_back(XMFLOAT3(0.5f, 0.5f, 0.5f));
    vecPositions.push_back(XMFLOAT3(-0.5f, 0.5f, 0.5f));
    vecNormals.push_back(XMFLOAT3(0.0f, 0.0f, 1.0f));
    vecNormals.push_back(XMFLOAT3(0.0f, 0.0f, 1.0f));
    vecNormals.push_back(XMFLOAT3(0.0f, 0.0f, 1.0f));
    vecNormals.push_back(XMFLOAT3(0.0f, 0.0f, 1.0f));
    vecTexcoord.push_back(XMFLOAT2(0.0f, 1.0f)); // ÁÂÇÏ´Ü
    vecTexcoord.push_back(XMFLOAT2(0.0f, 0.0f)); // ÁÂ»ó´Ü
    vecTexcoord.push_back(XMFLOAT2(1.0f, 0.0f)); // ¿ì»ó´Ü
    vecTexcoord.push_back(XMFLOAT2(1.0f, 1.0f)); // ¿ìÇÏ´Ü

    // ¿ÞÂÊ
    vecPositions.push_back(XMFLOAT3(-0.5f, -0.5f, 0.5f));
    vecPositions.push_back(XMFLOAT3(-0.5f, 0.5f, 0.5f));
    vecPositions.push_back(XMFLOAT3(-0.5f, 0.5f, -0.5f));
    vecPositions.push_back(XMFLOAT3(-0.5f, -0.5f, -0.5f));
    vecNormals.push_back(XMFLOAT3(-1.0f, 0.0f, 0.0f));
    vecNormals.push_back(XMFLOAT3(-1.0f, 0.0f, 0.0f));
    vecNormals.push_back(XMFLOAT3(-1.0f, 0.0f, 0.0f));
    vecNormals.push_back(XMFLOAT3(-1.0f, 0.0f, 0.0f));
    vecTexcoord.push_back(XMFLOAT2(0.0f, 1.0f)); // ÁÂÇÏ´Ü
    vecTexcoord.push_back(XMFLOAT2(0.0f, 0.0f)); // ÁÂ»ó´Ü
    vecTexcoord.push_back(XMFLOAT2(1.0f, 0.0f)); // ¿ì»ó´Ü
    vecTexcoord.push_back(XMFLOAT2(1.0f, 1.0f)); // ¿ìÇÏ´Ü

    // ¿À¸¥ÂÊ
    vecPositions.push_back(XMFLOAT3(0.5f, -0.5f, 0.5f));
    vecPositions.push_back(XMFLOAT3(0.5f, -0.5f, -0.5f));
    vecPositions.push_back(XMFLOAT3(0.5f, 0.5f, -0.5f));
    vecPositions.push_back(XMFLOAT3(0.5f, 0.5f, 0.5f));
    vecNormals.push_back(XMFLOAT3(1.0f, 0.0f, 0.0f));
    vecNormals.push_back(XMFLOAT3(1.0f, 0.0f, 0.0f));
    vecNormals.push_back(XMFLOAT3(1.0f, 0.0f, 0.0f));
    vecNormals.push_back(XMFLOAT3(1.0f, 0.0f, 0.0f));
    vecTexcoord.push_back(XMFLOAT2(0.0f, 1.0f)); // ÁÂÇÏ´Ü
    vecTexcoord.push_back(XMFLOAT2(0.0f, 0.0f)); // ÁÂ»ó´Ü
    vecTexcoord.push_back(XMFLOAT2(1.0f, 0.0f)); // ¿ì»ó´Ü
    vecTexcoord.push_back(XMFLOAT2(1.0f, 1.0f)); // ¿ìÇÏ´Ü

    std::vector<VTXPOSNORTEX> vecVertices;
    for (size_t i = 0; i < vecPositions.size(); i++) {
        VTXPOSNORTEX v;
        v.vPosition = vecPositions[i];
        v.vNormal = vecNormals[i];
        v.vTexCoord = vecTexcoord[i];
        vecVertices.push_back(v);
    }

    std::vector<_ushort> vecIndices = {
        0,  1,  2,  0,  2,  3,  // À­¸é
        4,  5,  6,  4,  6,  7,  // ¾Æ·§¸é
        8,  9,  10, 8,  10, 11, // ¾Õ¸é
        12, 13, 14, 12, 14, 15, // µÞ¸é
        16, 17, 18, 16, 18, 19, // ¿ÞÂÊ
        20, 21, 22, 20, 22, 23  // ¿À¸¥ÂÊ
    };

    return std::tuple{ vecVertices, vecIndices };
}

std::tuple<std::vector<VTXPOSNORTEX>, std::vector<_ulong>> CGeometryGenerator::Create_Sphere(const _uint _iNumSlices, const _uint _iNumStacks)
{
    const _float fTheta = XM_2PI / float(_iNumSlices);
    const _float fPhi = XM_PI / float(_iNumStacks);

    std::vector<VTXPOSNORTEX> vecVertices;

    for (_uint j = 0; j <= _iNumStacks; j++)
    {
        XMVECTOR vStackStartPoint = XMVector3TransformCoord(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMMatrixRotationZ(fPhi * j));

        for (_uint i = 0; i <= _iNumSlices; i++)
        {
            VTXPOSNORTEX vVertex;

            XMVECTOR vPosition = XMVector3TransformCoord(vStackStartPoint, XMMatrixRotationY(fTheta * _float(i)));

            XMStoreFloat3(&vVertex.vPosition, vPosition);
            XMStoreFloat3(&vVertex.vNormal, XMVector3Normalize(vPosition));

            vVertex.vTexCoord = _float2(_float(i) / _iNumSlices, 1.0f - _float(j) / _iNumStacks);

            vecVertices.push_back(vVertex);
        }
    }

    std::vector<_ulong> vecIndices;

    for (_uint j = 0; j < _iNumStacks; j++) {

        const _uint offset = (_iNumSlices + 1) * j;

        for (_uint i = 0; i < _iNumSlices; i++) {

            vecIndices.push_back(offset + i);
            vecIndices.push_back(offset + i + _iNumSlices + 1);
            vecIndices.push_back(offset + i + 1 + _iNumSlices + 1);

            vecIndices.push_back(offset + i);
            vecIndices.push_back(offset + i + 1 + _iNumSlices + 1);
            vecIndices.push_back(offset + i + 1);
        }
    }

    return std::make_tuple(std::move(vecVertices), std::move(vecIndices));
}

std::tuple<std::vector<VTXPOSTEX>, std::vector<_ushort>> CGeometryGenerator::Create_Rect(const float _fScale, const _float2 _vTexScale)
{
    vector<_float3> vecPositions;
    vector<_float2> vecTexcoords;

    // ¾Õ¸é
    vecPositions.push_back(_float3(-1.0f * _fScale, 1.0f * _fScale, 0.0f));
    vecPositions.push_back(_float3(1.0f * _fScale, 1.0f * _fScale, 0.0f));
    vecPositions.push_back(_float3(1.0f * _fScale, -1.0f * _fScale, 0.0f));
    vecPositions.push_back(_float3(-1.0f * _fScale, -1.0f * _fScale, 0.0f));
    vecTexcoords.push_back(_float2(0.0f, 0.0f));
    vecTexcoords.push_back(_float2(1.0f, 0.0f));
    vecTexcoords.push_back(_float2(1.0f, 1.0f));
    vecTexcoords.push_back(_float2(0.0f, 1.0f));

    std::vector<VTXPOSTEX> vecVertices;
    for (size_t i = 0; i < vecPositions.size(); ++i) {
        VTXPOSTEX v;
        v.vPosition = vecPositions[i];
        v.vTexCoord = _float2(vecTexcoords[i].x * _vTexScale.x, vecTexcoords[i].y * _vTexScale.y);

        vecVertices.push_back(v);
    }

    std::vector<_ushort> vecIndices = {
        0,  1,  2,  0,  2,  3
    };

    return std::make_tuple(std::move(vecVertices), std::move(vecIndices));
}
