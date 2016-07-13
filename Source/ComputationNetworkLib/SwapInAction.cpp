//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//

#define _CRT_SECURE_NO_WARNINGS // "secure" CRT not available on all platforms  --add this at the top of all CPP files that give "function or variable may be unsafe" warnings

#include "SwapInAction.h"

#ifndef CPUONLY
    #include <cuda.h>
#endif



namespace Microsoft { namespace MSR { namespace CNTK {


void SwapInAction::BeginAction()
{
    size_t cols = m_bufferGPU->GetNumCols();
    size_t rows = m_bufferGPU->GetNumRows();
    size_t bytes = cols*rows*sizeof(float);

    float *cpu = (float*)malloc(bytes);
    CUDA_CALL(cudaMemcpyAsync(m_bufferGPU->Data(), m_bufferCPU, bytes, cudaMemcpyDefault, m_swapInStream));
}

void SwapInAction::EndAction()
{
    CUDA_CALL(cudaStreamSynchronize(m_swapInStream));
    m_isSwappingToGPU = false;
}

}}}
