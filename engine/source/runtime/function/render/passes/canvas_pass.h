#pragma once

#include "runtime/function/render/render_pass.h"

#include "runtime/core/math/math_headers.h"

namespace Piccolo
{
    struct LineSegment
    {
        Vector3 start;
        Vector3 end;
    };

    class CanvasPass : public RenderPass
    {
    public:
        void addLineSegment(const LineSegment& segment);
        void addLineSegment(const std::vector<LineSegment>& segments);

        void initialize(const RenderPassInitInfo* init_info) override final;
        void draw() override final;

        void updateAfterFramebufferRecreate(VkImageView input_attachment);

    private:
        std::vector<LineSegment> m_segments;
    };
} // namespace Piccolo
