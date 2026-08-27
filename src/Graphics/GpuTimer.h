//***************************************************************************************
// GameObject.h by X_Jun(MKXJun) (C) 2018-2022 All Rights Reserved.
// Licensed under the MIT License.
//
// Retrieve the interval between two timestamps of the GPU.
//***************************************************************************************

#pragma once

#ifndef GPU_TIMER_H
#define GPU_TIMER_H

#include <cassert>
#include <cstdint>
#include <deque>
#include <wrl/client.h>
#include <d3d11.h>

struct GpuTimerInfo
{
    D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjointData{}; 
    uint64_t startData = 0;                             
    uint64_t stopData = 0;                              
    Microsoft::WRL::ComPtr<ID3D11Query> disjointQuery;  
    Microsoft::WRL::ComPtr<ID3D11Query> startQuery;     
    Microsoft::WRL::ComPtr<ID3D11Query> stopQuery;      
    bool isStopped = false;                             
};

class GpuTimer
{
public:
    GpuTimer() = default;
    

    void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, size_t recentCount = 0);
    

    void Reset(ID3D11DeviceContext* deviceContext, size_t recentCount = 0);

    HRESULT Start();

    void Stop();

    bool TryGetTime(double* pOut);

    double GetTime();

    double AverageTime()
    {
        if (m_RecentCount)
            return m_AccumTime / m_DeltaTimes.size();
        else
            return m_AccumTime / m_AccumCount;
    }

private:
    
    static bool GetQueryDataHelper(ID3D11DeviceContext* pContext, bool loopUntilDone, ID3D11Query* query, void* data, uint32_t dataSize);
    

    std::deque<double> m_DeltaTimes;   
    double m_AccumTime = 0.0;          
    size_t m_AccumCount = 0;           
    size_t m_RecentCount = 0;          

    std::deque<GpuTimerInfo> m_Queries; 
    Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pImmediateContext;
};

#endif