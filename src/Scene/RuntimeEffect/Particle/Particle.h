#pragma once
#include <DirectXMath.h>

namespace Destiny
{
    struct Particle
    {
        DirectX::XMFLOAT3 position;    
        DirectX::XMFLOAT3 velocity;    
        DirectX::XMFLOAT3 acceleration;
        float lifetime;       
        float maxLifetime;    
        DirectX::XMFLOAT3 color;
        float size;           
    };

}