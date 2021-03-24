#pragma once

#include <DirectXMath.h>

using vec4 = DirectX::XMVECTOR;
using float4 = DirectX::XMFLOAT4;
using float3 = DirectX::XMFLOAT3;
using float2 = DirectX::XMFLOAT2;
using mat4 = DirectX::XMMATRIX;

inline auto Vec4 = DirectX::XMVectorSet;

inline auto QuatRotMat = DirectX::XMMatrixRotationQuaternion;
inline auto TransFromVec = DirectX::XMMatrixTranslationFromVector;

inline auto PerspectiveFov = DirectX::XMMatrixPerspectiveFovLH;

inline auto Inverse = DirectX::XMMatrixInverse;

inline auto Vec4Trans = DirectX::XMVector4Transform;

inline auto VecAdd = DirectX::XMVectorAdd;
inline auto VecScale = DirectX::XMVectorScale;

inline auto VecMul = DirectX::XMVectorMultiply;

inline auto QuatMul = DirectX::XMQuaternionMultiply;

inline auto QuatRotAxis = DirectX::XMQuaternionRotationAxis;

inline auto Vec2Norm = DirectX::XMVector2Normalize;

inline auto Vec3Norm = DirectX::XMVector3Normalize;

inline auto Vec4Norm = DirectX::XMVector4Normalize;


