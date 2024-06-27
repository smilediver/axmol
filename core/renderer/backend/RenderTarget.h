#pragma once

#include "base/Object.h"
#include "Texture.h"
#include <assert.h>

NS_AX_BACKEND_BEGIN

class RenderTarget : public ax::Object
{
public:
    struct RenderBuffer
    {
        TextureBackend* texture = nullptr;
        uint8_t level           = 0;  // level when attached to a texture
        explicit operator bool() const { return texture != nullptr; }
    };
    typedef RenderBuffer ColorAttachment[MAX_COLOR_ATTCHMENT];

    RenderTarget(bool defaultRenderTarget) : _defaultRenderTarget(defaultRenderTarget) {}
    virtual ~RenderTarget()
    {
        for (auto colorItem : _color)
            AX_SAFE_RELEASE(colorItem.texture);
        AX_SAFE_RELEASE(_depth.texture);
        AX_SAFE_RELEASE(_stencil.texture);
    }

    bool isDefaultRenderTarget() const { return _defaultRenderTarget; }

    void setColorAttachment(ColorAttachment attachment)
    {
        for (int i = 0; i < MAX_COLOR_ATTCHMENT; ++i)
        {
            auto colorItem = _color[i];
            if (colorItem.texture != attachment[i].texture || colorItem.level != attachment[i].level)
                _dirtyFlags |= getMRTColorFlag(i);
            AX_SAFE_RELEASE(colorItem.texture);
        }
        memcpy(_color, attachment, sizeof(ColorAttachment));
        for (auto colorItem : _color)
            AX_SAFE_RETAIN(colorItem.texture);
    };

    void setColorAttachment(TextureBackend* attachment, int level = 0, int index = 0)
    {
        if (_color[index].texture != attachment || _color[index].level != level)
        {
            _dirtyFlags |= getMRTColorFlag(index);
            AX_SAFE_RELEASE(_color[index].texture);
            _color[index].texture = attachment;
            _color[index].level   = level;
            AX_SAFE_RETAIN(_color[index].texture);
        }
    }

    void setDepthAttachment(TextureBackend* attachment, int level = 0)
    {
        if (_depth.texture != attachment || _depth.level != level)
        {
            _dirtyFlags |= TargetBufferFlags::DEPTH;
            AX_SAFE_RELEASE(_depth.texture);
            _depth.texture = attachment;
            _depth.level   = level;
            AX_SAFE_RETAIN(_depth.texture);
        }
    };
    void setStencilAttachment(TextureBackend* attachment, int level = 0)
    {
        if (_stencil.texture != attachment || _depth.level != level)
        {
            _dirtyFlags |= TargetBufferFlags::STENCIL;
            AX_SAFE_RELEASE(_stencil.texture);
            _stencil.texture = attachment;
            _stencil.level   = level;
            AX_SAFE_RETAIN(_stencil.texture);
        }
    };

    bool isDirty() const { return !!_dirtyFlags; }

    ColorAttachment _color{};
    RenderBuffer _depth{};
    RenderBuffer _stencil{};

protected:
    bool _defaultRenderTarget = false;
    mutable TargetBufferFlags _dirtyFlags{};
};

NS_AX_BACKEND_END
