#include "runtime/resource/res_type/data/blend_state.h"

namespace Piccolo
{
    float BlendState::getLength() const
    {
        float length = 0;
        for (int i = 0; i < m_clip_count; i++)
        {
            auto curweight = m_blend_weight[i];
            length += curweight * m_blend_clip_file_length[i];
        }
        return length;
    }
}