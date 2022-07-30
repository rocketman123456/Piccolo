#include "runtime/function/render/passes/canvas_pass.h"

namespace Piccolo
{
    void CanvasPass::addLineSegment(const LineSegment& segment) { m_segments.push_back(segment); }

    void CanvasPass::addLineSegment(const std::vector<LineSegment>& segments)
    {
        m_segments.insert(m_segments.end(), segments.begin(), segments.end());
    }

    void CanvasPass::initialize(const RenderPassInitInfo* init_info) {}

    void CanvasPass::draw() {}

    void CanvasPass::updateAfterFramebufferRecreate(VkImageView input_attachment) {}
} // namespace Piccolo
