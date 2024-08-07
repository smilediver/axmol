/****************************************************************************
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 https://axmol.dev/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "2d/NodeGrid.h"
#include "2d/Grid.h"
#include "renderer/Renderer.h"

NS_AX_BEGIN

NodeGrid* NodeGrid::create()
{
    NodeGrid* ret = new NodeGrid();
    if (ret->init())
    {
        ret->autorelease();
    }
    else
    {
        AX_SAFE_DELETE(ret);
    }
    return ret;
}

NodeGrid* NodeGrid::create(const ax::Rect& rect)
{
    NodeGrid* ret = NodeGrid::create();
    if (ret)
    {
        ret->setGridRect(rect);
    }
    return ret;
}

NodeGrid::NodeGrid() {}

void NodeGrid::setTarget(Node* target)
{
#if AX_ENABLE_GC_FOR_NATIVE_OBJECTS
    auto sEngine = ScriptEngineManager::getInstance()->getScriptEngine();
    if (sEngine)
    {
        if (_gridTarget)
            sEngine->releaseScriptObject(this, _gridTarget);
        if (target)
            sEngine->retainScriptObject(this, target);
    }
#endif  // AX_ENABLE_GC_FOR_NATIVE_OBJECTS
    AX_SAFE_RELEASE(_gridTarget);
    AX_SAFE_RETAIN(target);
    _gridTarget = target;
}

NodeGrid::~NodeGrid()
{
    AX_SAFE_RELEASE(_nodeGrid);
    AX_SAFE_RELEASE(_gridTarget);
}

void NodeGrid::onGridBeginDraw()
{
    if (_nodeGrid && _nodeGrid->isActive())
    {
        _nodeGrid->beforeDraw();
    }
}

void NodeGrid::onGridEndDraw()
{
    if (_nodeGrid && _nodeGrid->isActive())
    {
        _nodeGrid->afterDraw(this);
    }
}

void NodeGrid::visit(const RenderContext& rctx, RenderArg1& rctx1)
{
    // quick return if not visible. children won't be drawn.
    if (!_visible)
    {
        return;
    }

    // TODO: rctx optimize
    _modelViewTransform = this->transform(getParentWorldTransform());
    _transformUpdated = false;

    Director::Projection beforeProjectionType = Director::Projection::DEFAULT;
    if (_nodeGrid && _nodeGrid->isActive())
    {
        beforeProjectionType = _director->getProjection();
        _nodeGrid->set2DProjection();
    }

    onGridBeginDraw();

    if (_gridTarget)
    {
        _gridTarget->visit(rctx, rctx1);
    }

    int i                = 0;
    bool visibleByCamera = isVisitableByVisitingCamera();

    if (!_children.empty())
    {
        sortAllChildren();
        // draw children zOrder < 0
        for (auto size = _children.size(); i < size; ++i)
        {
            auto node = _children.at(i);

            if (node && node->getLocalZOrder() < 0)
                node->visit(rctx, rctx1);
            else
                break;
        }
        // self draw,currently we have nothing to draw on NodeGrid, so there is no need to add render command
        if (visibleByCamera)
            this->draw(rctx, rctx1);

        for (auto it = _children.cbegin() + i, itCend = _children.cend(); it != itCend; ++it)
        {
            (*it)->visit(rctx, rctx1);
        }
    }
    else if (visibleByCamera)
    {
        this->draw(rctx, rctx1);
    }

    // FIX ME: Why need to set _orderOfArrival to 0??
    // Please refer to https://github.com/cocos2d/cocos2d-x/pull/6920
    // setOrderOfArrival(0);

    if (_nodeGrid && _nodeGrid->isActive())
    {
        // restore projection
        _director->setProjection(beforeProjectionType);
    }

    onGridEndDraw();
}

void NodeGrid::setGrid(GridBase* grid)
{
    AX_SAFE_RELEASE(_nodeGrid);
    AX_SAFE_RETAIN(grid);
    _nodeGrid = grid;
}

NS_AX_END
