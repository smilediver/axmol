/****************************************************************************
Copyright (c) 2015 Neo Kim (neo.kim@neofect.com)
Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

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

#include "ui/UIPageViewIndicator.h"
#include "2d/Sprite.h"
#include "base/Utils.h"

static const char* CIRCLE_IMAGE =
    "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAQAAADZc7J/AAAA8ElEQVRIx62VyRGCQBBF+6gWRCEmYDIQkhiBCgHhSclC8YqWzOV5oVzKAYZp3r1/"
    "9fpbxAIBMTsKrjx5cqVgR0wgLhCRUWOjJiPqD56xoaGPhpRZV/iSEy6crHmw5oIrF9b/lVeMofrJgjlnxlIy/"
    "wik+JB+mme8BExbBhm+5CJC2LE2LtSEQoyGWDioBA5CoRIohJtK4CYDxzNEM4GAugR1E9VjVC+"
    "SZpXvhCJCrjomESLvc17pDGX7bWmlh6UtpjPVCWy9zaJ0TD7qfm3pwERMz2trRVZk3K3BD/L34AY+dEDCniMVBkPFkT2J/b2/"
    "AIV+dRpFLOYoAAAAAElFTkSuQmCC";
static const char* CIRCLE_IMAGE_KEY = "/__circleImage";

NS_AX_BEGIN

namespace
{
static const float SPACE_BETWEEN_INDEX_NODES_DEFAULT = 23;
static const uint8_t INDEX_NODES_OPACITY_DEFAULT     = 0.3 * 255;
}  // namespace

namespace ui
{

PageViewIndicator* PageViewIndicator::create()
{
    PageViewIndicator* node = new PageViewIndicator();
    if (node->init())
    {
        node->autorelease();
        return node;
    }
    AX_SAFE_DELETE(node);
    return nullptr;
}

PageViewIndicator::PageViewIndicator()
    : _direction(PageView::Direction::HORIZONTAL)
    , _currentIndexNode(nullptr)
    , _currentOverlappingIndexNode(nullptr)
    , _spaceBetweenIndexNodes(SPACE_BETWEEN_INDEX_NODES_DEFAULT)
    , _indexNodesScale(1.0f)
    , _indexNodesColor(Color3B::WHITE)
    , _indexNodesOpacity(INDEX_NODES_OPACITY_DEFAULT)
    , _useDefaultTexture(true)
    , _indexNodesTextureFile("")
    , _indexNodesTexType(Widget::TextureResType::LOCAL)
{}

PageViewIndicator::~PageViewIndicator() {}

bool PageViewIndicator::init()
{
    _currentIndexNode = utils::createSpriteFromBase64Cached(CIRCLE_IMAGE, CIRCLE_IMAGE_KEY);
    _currentIndexNode->setVisible(false);
    addProtectedChild(_currentIndexNode, 1);
    return true;
}

void PageViewIndicator::setDirection(PageView::Direction direction)
{
    _direction = direction;
    rearrange();
}

void PageViewIndicator::reset(ssize_t numberOfTotalPages)
{
    while (_indexNodes.size() < numberOfTotalPages)
    {
        increaseNumberOfPages();
    }
    while (_indexNodes.size() > numberOfTotalPages)
    {
        decreaseNumberOfPages();
    }
    rearrange();
    _currentIndexNode->setVisible(!_indexNodes.empty());
}

void PageViewIndicator::indicate(ssize_t index)
{
    if (index < 0 || index >= _indexNodes.size())
    {
        return;
    }
    Sprite* oldOverlappingNode   = _currentOverlappingIndexNode;
    _currentOverlappingIndexNode = _indexNodes.at(index);
    if (oldOverlappingNode != _currentOverlappingIndexNode)
    {
        if (oldOverlappingNode)
            oldOverlappingNode->setVisible(true);
        _currentOverlappingIndexNode->setVisible(false);
        _currentIndexNode->setPosition(_currentOverlappingIndexNode->getPosition());
    }
}

void PageViewIndicator::rearrange()
{
    if (_indexNodes.empty())
    {
        return;
    }

    bool horizontal = (_direction == PageView::Direction::HORIZONTAL);

    // Calculate total size
    Vec2 indexNodeSize = _indexNodes.at(0)->getContentSize();
    float sizeValue    = (horizontal ? indexNodeSize.width : indexNodeSize.height);

    ssize_t numberOfItems = _indexNodes.size();
    float totalSizeValue  = sizeValue * numberOfItems + _spaceBetweenIndexNodes * (numberOfItems - 1);

    float posValue = -(totalSizeValue / 2) + (sizeValue / 2);
    for (auto&& indexNode : _indexNodes)
    {
        Vec2 position;
        if (horizontal)
        {
            position = Vec2(posValue, indexNodeSize.height / 2.0f);
        }
        else
        {
            position = Vec2(indexNodeSize.width / 2.0f, -posValue);
        }
        indexNode->setPosition(position);
        posValue += sizeValue + _spaceBetweenIndexNodes;
    }
}

void PageViewIndicator::setSpaceBetweenIndexNodes(float spaceBetweenIndexNodes)
{
    if (_spaceBetweenIndexNodes == spaceBetweenIndexNodes)
    {
        return;
    }
    _spaceBetweenIndexNodes = spaceBetweenIndexNodes;
    rearrange();
}

void PageViewIndicator::setIndexNodesColor(const Color3B& indexNodesColor)
{
    _indexNodesColor = indexNodesColor;

    for (auto&& indexNode : _indexNodes)
    {
        indexNode->setColor(indexNodesColor);
    }
}

void PageViewIndicator::setIndexNodesOpacity(uint8_t opacity)
{
    _indexNodesOpacity = opacity;
    for (auto&& indexNode : _indexNodes)
        indexNode->setOpacity(opacity);
}

void PageViewIndicator::setIndexNodesScale(float indexNodesScale)
{
    if (_indexNodesScale == indexNodesScale)
    {
        return;
    }
    _indexNodesScale = indexNodesScale;

    _currentIndexNode->setScale(indexNodesScale);
    for (auto&& indexNode : _indexNodes)
    {
        indexNode->setScale(_indexNodesScale);
    }

    rearrange();
}

void PageViewIndicator::setIndexNodesTexture(std::string_view texName, Widget::TextureResType texType)
{
    _useDefaultTexture     = false;
    _indexNodesTextureFile = texName;
    _indexNodesTexType     = texType;

    switch (texType)
    {
    case Widget::TextureResType::LOCAL:
        _currentIndexNode->setTexture(texName);
        for (auto&& indexNode : _indexNodes)
        {
            indexNode->setTexture(texName);
        }
        break;
    case Widget::TextureResType::PLIST:
        _currentIndexNode->setSpriteFrame(texName);
        for (auto&& indexNode : _indexNodes)
        {
            indexNode->setSpriteFrame(texName);
        }
        break;
    default:
        break;
    }

    rearrange();
}

void PageViewIndicator::increaseNumberOfPages()
{
    if (_currentOverlappingIndexNode)
    {
        _currentOverlappingIndexNode->setVisible(true);
        _currentOverlappingIndexNode = nullptr;
    }
    Sprite* indexNode;

    if (_useDefaultTexture)
    {
        indexNode = utils::createSpriteFromBase64(CIRCLE_IMAGE);
    }
    else
    {
        switch (_indexNodesTexType)
        {
        case Widget::TextureResType::LOCAL:
            indexNode = Sprite::create(_indexNodesTextureFile);
            break;
        case Widget::TextureResType::PLIST:
            indexNode = Sprite::createWithSpriteFrameName(_indexNodesTextureFile);
            break;
        default:
            return;
        }
    }

    indexNode->setColor(_indexNodesColor);
    indexNode->setScale(_indexNodesScale);
    indexNode->setOpacity(_indexNodesOpacity);
    addProtectedChild(indexNode);
    _indexNodes.pushBack(indexNode);
}

void PageViewIndicator::decreaseNumberOfPages()
{
    if (_currentOverlappingIndexNode)
    {
        _currentOverlappingIndexNode->setVisible(true);
        _currentOverlappingIndexNode = nullptr;
    }
    if (_indexNodes.empty())
    {
        return;
    }
    removeProtectedChild(*_indexNodes.begin());
    _indexNodes.erase(_indexNodes.begin());
}

void PageViewIndicator::clear()
{
    if (_currentOverlappingIndexNode)
    {
        _currentOverlappingIndexNode->setVisible(true);
        _currentOverlappingIndexNode = nullptr;
    }
    for (auto&& indexNode : _indexNodes)
    {
        removeProtectedChild(indexNode);
    }
    _indexNodes.clear();
    _currentIndexNode->setVisible(false);
}

}  // namespace ui

NS_AX_END
