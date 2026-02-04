#pragma once
#include <functional>

namespace Destiny
{
    class SwapChain
    {
    public:
        virtual ~SwapChain() = default;
        virtual void present(std::function<void(void*)> callback = nullptr) = 0;
        virtual void resize(int width, int height) = 0;
    };
}
