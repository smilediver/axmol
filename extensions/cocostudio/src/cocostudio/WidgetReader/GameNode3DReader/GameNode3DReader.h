/****************************************************************************
 Copyright (c) 2014 cocos2d-x.org

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

#ifndef __cocos2d_libs__GameNode3DReader__
#define __cocos2d_libs__GameNode3DReader__

#include "2d/CameraBackgroundBrush.h"
#include "cocostudio/CocosStudioExport.h"
#include "cocostudio/WidgetReader/NodeReaderProtocol.h"
#include "cocostudio/WidgetReader/NodeReaderDefine.h"

namespace cocostudio
{
class CCS_DLL GameNode3DReader : public ax::Object, public NodeReaderProtocol
{
    DECLARE_CLASS_NODE_READER_INFO

public:
    GameNode3DReader();
    ~GameNode3DReader();

    static GameNode3DReader* getInstance();
    static ax::CameraBackgroundBrush* getSceneBrushInstance();
    /** @deprecated Use method destroyInstance() instead */
    AX_DEPRECATED_ATTRIBUTE static void purge();
    static void destroyInstance();

    flatbuffers::Offset<flatbuffers::Table> createOptionsWithFlatBuffers(pugi::xml_node objectData,
                                                                         flatbuffers::FlatBufferBuilder* builder);
    void setPropsWithFlatBuffers(ax::Node* node, const flatbuffers::Table* node3DOptions);
    ax::Node* createNodeWithFlatBuffers(const flatbuffers::Table* node3DOptions);

protected:
    int getResourceType(std::string_view key);
};
}  // namespace cocostudio

#endif /* defined(__cocos2d_libs__Node3DReader__) */
