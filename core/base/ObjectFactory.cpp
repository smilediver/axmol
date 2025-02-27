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

#include <functional>
#include "base/ObjectFactory.h"

NS_AX_BEGIN

ObjectFactory::TInfo::TInfo() : _class(""), _fun(nullptr), _func(nullptr) {}

ObjectFactory::TInfo::TInfo(std::string_view type, Instance ins) : _class(type), _fun(ins), _func(nullptr)
{
    ObjectFactory::getInstance()->registerType(*this);
}

ObjectFactory::TInfo::TInfo(std::string_view type, InstanceFunc ins) : _class(type), _fun(nullptr), _func(ins)
{
    ObjectFactory::getInstance()->registerType(*this);
}

ObjectFactory::TInfo::TInfo(const TInfo& t)
{
    _class = t._class;
    _fun   = t._fun;
    _func  = t._func;
}

ObjectFactory::TInfo::~TInfo()
{
    _class = "";
    _fun   = nullptr;
    _func  = nullptr;
}

ObjectFactory::TInfo& ObjectFactory::TInfo::operator=(const TInfo& t)
{
    _class = t._class;
    _fun   = t._fun;
    _func  = t._func;
    return *this;
}

ObjectFactory* ObjectFactory::_sharedFactory = nullptr;

ObjectFactory::ObjectFactory() {}

ObjectFactory::~ObjectFactory()
{
    _typeMap.clear();
}

ObjectFactory* ObjectFactory::getInstance()
{
    if (nullptr == _sharedFactory)
    {
        _sharedFactory = new ObjectFactory();
    }
    return _sharedFactory;
}

void ObjectFactory::destroyInstance()
{
    AX_SAFE_DELETE(_sharedFactory);
}

Object* ObjectFactory::createObject(std::string_view name)
{
    Object* o = nullptr;
    do
    {
        // const TInfo t = _typeMap[name.data];
        auto it = _typeMap.find(name);
        if (it == _typeMap.end())
            break;
        auto& t = it->second;
        if (t._fun != nullptr)
        {
            o = t._fun();
        }
        else if (t._func != nullptr)
        {
            o = t._func();
        }
    } while (0);

    return o;
}

void ObjectFactory::registerType(const TInfo& t)
{
    _typeMap.emplace(t._class, t);
}

NS_AX_END
