/****************************************************************************
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

#include "UIPageViewTest.h"
#include "axmol.h"

USING_NS_AX;
using namespace ax::ui;

UIPageViewTests::UIPageViewTests()
{
    ADD_TEST_CASE(UIPageViewTest);
    ADD_TEST_CASE(UIPageViewButtonTest);
    ADD_TEST_CASE(UIPageViewTouchPropagationTest);
    ADD_TEST_CASE(UIPageViewDynamicAddAndRemoveTest);
    ADD_TEST_CASE(UIPageViewJumpToPageTest);
    ADD_TEST_CASE(UIPageViewVerticalTest);
    ADD_TEST_CASE(UIPageViewDisableTouchTest);
    ADD_TEST_CASE(UIPageViewChildSizeTest);
    ADD_TEST_CASE(UIPageViewIndicatorTest);
}

// UIPageViewTest
UIPageViewTest::UIPageViewTest() : _displayValueLabel(nullptr) {}

UIPageViewTest::~UIPageViewTest() {}

bool UIPageViewTest::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the dragpanel events will be displayed
        _displayValueLabel = Text::create("Move by horizontal direction", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5));
        _uiLayer->addChild(_displayValueLabel);

        // Add the black background
        Text* alert = Text::create("PageView", "fonts/Marker Felt.ttf", 30);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = dynamic_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the page view
        Size size(240, 130);
        PageView* pageView = PageView::create();
        pageView->setDirection(PageView::Direction::HORIZONTAL);
        pageView->setContentSize(size);
        Size backgroundSize = background->getContentSize();
        pageView->setPosition((widgetSize - pageView->getContentSize()) / 2.0f);
        pageView->removeAllItems();
        pageView->setIndicatorEnabled(true);
        pageView->setGlobalZOrder(200);

        int pageCount = 4;
        for (int i = 0; i < pageCount; ++i)
        {
            Layout* layout = Layout::create();
            layout->setContentSize(size);

            ImageView* imageView = ImageView::create("cocosui/scrollviewbg.png");
            imageView->setScale9Enabled(true);
            imageView->setContentSize(size);
            imageView->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));
            layout->addChild(imageView);

            Text* label = Text::create(fmt::format("page {}", (i + 1)), "fonts/Marker Felt.ttf", 30);
            label->setColor(Color3B(192, 192, 192));
            label->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));
            layout->addChild(label);

            pageView->insertCustomItem(layout, i);
        }

        pageView->removeItem(0);
        pageView->scrollToItem(pageCount - 2);

        AXLOGD("TODO in {} {} {}", __FILE__, __FUNCTION__, __LINE__);

        pageView->setIndicatorIndexNodesOpacity(255);

        _uiLayer->addChild(pageView);

        return true;
    }
    return false;
}

void UIPageViewTest::pageViewEvent(Object* pSender, PageView::EventType type)
{
    switch (type)
    {
    case PageView::EventType::TURNING:
    {
        PageView* pageView = dynamic_cast<PageView*>(pSender);

        _displayValueLabel->setString(
            fmt::format("page = {}", static_cast<int32_t>(pageView->getCurrentPageIndex() + 1)));
    }
    break;

    default:
        break;
    }
}

// UIPageViewButtonTest
UIPageViewButtonTest::UIPageViewButtonTest() : _displayValueLabel(nullptr) {}

UIPageViewButtonTest::~UIPageViewButtonTest() {}

bool UIPageViewButtonTest::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the drag panel events will be displayed
        _displayValueLabel = Text::create("Move by horizontal direction", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5));
        _uiLayer->addChild(_displayValueLabel);

        // Add the black background
        Text* alert = Text::create("PageView with Buttons", "fonts/Marker Felt.ttf", 30);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = dynamic_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the page view
        PageView* pageView = PageView::create();
        pageView->setContentSize(Size(240.0f, 130.0f));
        Size backgroundSize = background->getContentSize();
        pageView->setPosition(Vec2((widgetSize.width - backgroundSize.width) / 2.0f +
                                       (backgroundSize.width - pageView->getContentSize().width) / 2.0f,
                                   (widgetSize.height - backgroundSize.height) / 2.0f +
                                       (backgroundSize.height - pageView->getContentSize().height) / 2.0f));

        pageView->removeAllItems();

        int pageCount = 4;
        for (int i = 0; i < pageCount; ++i)
        {
            HBox* outerBox = HBox::create();
            outerBox->setContentSize(Size(240.0f, 130.0f));

            for (int k = 0; k < 2; ++k)
            {
                VBox* innerBox = VBox::create();

                for (int j = 0; j < 3; j++)
                {
                    Button* btn =
                        Button::create("cocosui/animationbuttonnormal.png", "cocosui/animationbuttonpressed.png");
                    btn->setName(fmt::format("button {}", j));
                    btn->addTouchEventListener(AX_CALLBACK_2(UIPageViewButtonTest::onButtonClicked, this));

                    innerBox->addChild(btn);
                }

                LinearLayoutParameter* parameter = LinearLayoutParameter::create();
                parameter->setMargin(Margin(0.0f, 0.0f, 100.0f, 0.0f));
                innerBox->setLayoutParameter(parameter);

                outerBox->addChild(innerBox);
            }

            pageView->insertCustomItem(outerBox, i);
        }

        pageView->removeItem(0);

        pageView->addEventListener(
            (PageView::ccPageViewCallback)AX_CALLBACK_2(UIPageViewButtonTest::pageViewEvent, this));

        _uiLayer->addChild(pageView);

        return true;
    }
    return false;
}

void UIPageViewButtonTest::onButtonClicked(Object* sender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        AXLOGD("button {} clicked", static_cast<Button*>(sender)->getName().data());
    }
}

void UIPageViewButtonTest::pageViewEvent(Object* pSender, PageView::EventType type)
{
    switch (type)
    {
    case PageView::EventType::TURNING:
    {
        PageView* pageView = dynamic_cast<PageView*>(pSender);

        _displayValueLabel->setString(
            fmt::format("page = {}", static_cast<int32_t>(pageView->getCurrentPageIndex() + 1)));
    }
    break;

    default:
        break;
    }
}

// UIPageViewTouchPropagationTest
UIPageViewTouchPropagationTest::UIPageViewTouchPropagationTest() : _displayValueLabel(nullptr) {}

UIPageViewTouchPropagationTest::~UIPageViewTouchPropagationTest() {}

bool UIPageViewTouchPropagationTest::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the dragpanel events will be displayed
        _displayValueLabel = Text::create("Move by horizontal direction", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5));
        _uiLayer->addChild(_displayValueLabel);

        // Add the black background
        Text* alert = Text::create("PageView Touch Propagation", "fonts/Marker Felt.ttf", 30);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = dynamic_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the page view
        PageView* pageView = PageView::create();
        pageView->setContentSize(Size(240.0f, 130.0f));
        pageView->setAnchorPoint(Vec2(0.5f, 0.5f));
        Size backgroundSize = background->getContentSize();
        pageView->setPosition(Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f));
        pageView->setBackGroundColor(Color3B::GREEN);
        pageView->setBackGroundColorType(Layout::BackGroundColorType::SOLID);

        int pageCount = 4;
        for (int i = 0; i < pageCount; ++i)
        {
            HBox* outerBox = HBox::create();
            outerBox->setContentSize(Size(240.0f, 130.0f));

            for (int k = 0; k < 2; ++k)
            {
                VBox* innerBox = VBox::create();

                for (int j = 0; j < 3; j++)
                {
                    Button* btn =
                        Button::create("cocosui/animationbuttonnormal.png", "cocosui/animationbuttonpressed.png");
                    btn->setName(fmt::format("button {}", j));
                    btn->addTouchEventListener(AX_CALLBACK_2(UIPageViewTouchPropagationTest::onButtonClicked, this));

                    innerBox->addChild(btn);
                }

                LinearLayoutParameter* parameter = LinearLayoutParameter::create();
                parameter->setMargin(Margin(0.0f, 0.0f, 100.0f, 0.0f));
                innerBox->setLayoutParameter(parameter);

                outerBox->addChild(innerBox);
            }

            pageView->insertCustomItem(outerBox, i);
        }

        pageView->addEventListener(
            (PageView::ccPageViewCallback)AX_CALLBACK_2(UIPageViewTouchPropagationTest::pageViewEvent, this));
        pageView->setName("pageView");
        pageView->addTouchEventListener([](Object* sender, Widget::TouchEventType type) {
            if (type == Widget::TouchEventType::BEGAN)
            {
                AXLOGD("page view touch began");
            }
            else if (type == Widget::TouchEventType::MOVED)
            {
                AXLOGD("page view touch moved");
            }
            else if (type == Widget::TouchEventType::ENDED)
            {
                AXLOGD("page view touch ended");
            }
            else
            {
                AXLOGD("page view touch cancelled");
            }
        });
        _uiLayer->addChild(pageView);

        Text* propagationText = Text::create("Allow Propagation", "Arial", 10);
        propagationText->setAnchorPoint(Vec2(0.0f, 0.5f));
        propagationText->setTextColor(Color4B::RED);
        propagationText->setPosition(Vec2(0.0f, pageView->getPosition().y + 50));
        _uiLayer->addChild(propagationText);

        Text* swallowTouchText = Text::create("Swallow Touches", "Arial", 10);
        swallowTouchText->setAnchorPoint(Vec2(0.f, 0.5f));
        swallowTouchText->setTextColor(Color4B::RED);
        swallowTouchText->setPosition(Vec2(0.0f, pageView->getPosition().y));
        _uiLayer->addChild(swallowTouchText);

        // Create the checkbox
        CheckBox* checkBox1 = CheckBox::create("cocosui/check_box_normal.png", "cocosui/check_box_normal_press.png",
                                               "cocosui/check_box_active.png", "cocosui/check_box_normal_disable.png",
                                               "cocosui/check_box_active_disable.png");
        checkBox1->setPosition(propagationText->getPosition() +
                               Vec2(propagationText->getContentSize().width / 2, -20.0f));

        checkBox1->setName("propagation");
        _uiLayer->addChild(checkBox1);

        // Create the checkbox
        CheckBox* checkBox2 = CheckBox::create("cocosui/check_box_normal.png", "cocosui/check_box_normal_press.png",
                                               "cocosui/check_box_active.png", "cocosui/check_box_normal_disable.png",
                                               "cocosui/check_box_active_disable.png");
        checkBox2->setPosition(swallowTouchText->getPosition() +
                               Vec2(swallowTouchText->getContentSize().width / 2, -20.0f));

        checkBox2->setName("swallow");
        _uiLayer->addChild(checkBox2);

        auto eventListener          = EventListenerTouchOneByOne::create();
        eventListener->onTouchBegan = [](Touch* touch, Event* event) -> bool {
            AXLOGD("layout receives touches");
            return true;
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

        return true;
    }
    return false;
}

void UIPageViewTouchPropagationTest::onButtonClicked(Object* pSender, Widget::TouchEventType type)
{
    Button* btn   = (Button*)pSender;
    CheckBox* ck1 = (CheckBox*)_uiLayer->getChildByName("propagation");
    CheckBox* ck2 = (CheckBox*)_uiLayer->getChildByName("swallow");
    auto pageView = (PageView*)_uiLayer->getChildByName("pageView");

    if (type == Widget::TouchEventType::BEGAN)
    {
        if (ck1->isSelected())
        {
            btn->setPropagateTouchEvents(true);
            pageView->setPropagateTouchEvents(true);
        }
        else
        {
            btn->setPropagateTouchEvents(false);
            pageView->setPropagateTouchEvents(false);
        }

        if (ck2->isSelected())
        {
            btn->setSwallowTouches(true);
            pageView->setSwallowTouches(true);
        }
        else
        {
            btn->setSwallowTouches(false);
            pageView->setSwallowTouches(false);
        }
    }
    if (type == Widget::TouchEventType::ENDED)
    {
        AXLOGD("button clicked");
    }
}

void UIPageViewTouchPropagationTest::pageViewEvent(Object* pSender, PageView::EventType type)
{
    switch (type)
    {
    case PageView::EventType::TURNING:
    {
        PageView* pageView = dynamic_cast<PageView*>(pSender);

        _displayValueLabel->setString(
            fmt::format("page = {}", static_cast<int32_t>(pageView->getCurrentPageIndex() + 1)));
    }
    break;

    default:
        break;
    }
}

// UIPageViewDynamicAddAndRemoveTest
UIPageViewDynamicAddAndRemoveTest::UIPageViewDynamicAddAndRemoveTest() : _displayValueLabel(nullptr) {}

UIPageViewDynamicAddAndRemoveTest::~UIPageViewDynamicAddAndRemoveTest() {}

bool UIPageViewDynamicAddAndRemoveTest::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the dragpanel events will be displayed
        _displayValueLabel = Text::create("Click Buttons on the Left", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5));
        _uiLayer->addChild(_displayValueLabel);

        // Add the black background
        Text* alert = Text::create("PageView Dynamic Modification", "fonts/Marker Felt.ttf", 30);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = dynamic_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the page view
        PageView* pageView = PageView::create();
        pageView->setDirection(ui::PageView::Direction::VERTICAL);
        pageView->setContentSize(Size(240.0f, 130.0f));
        pageView->setAnchorPoint(Vec2(0.5f, 0.5f));
        Size backgroundSize = background->getContentSize();
        pageView->setPosition(Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f));
        pageView->setBackGroundColor(Color3B::GREEN);
        pageView->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
        pageView->setIndicatorEnabled(true);
        pageView->setIndicatorSpaceBetweenIndexNodes(10);

        int pageCount = 4;
        for (int i = 0; i < pageCount; ++i)
        {
            HBox* outerBox = HBox::create();
            outerBox->setContentSize(Size(240.0f, 130.0f));

            for (int k = 0; k < 2; ++k)
            {
                VBox* innerBox = VBox::create();

                for (int j = 0; j < 3; j++)
                {
                    Button* btn =
                        Button::create("cocosui/animationbuttonnormal.png", "cocosui/animationbuttonpressed.png");
                    btn->setName(fmt::format("button {}", j));

                    innerBox->addChild(btn);
                }

                LinearLayoutParameter* parameter = LinearLayoutParameter::create();
                parameter->setMargin(Margin(0.0f, 0.0f, 100.0f, 0.0f));
                innerBox->setLayoutParameter(parameter);

                outerBox->addChild(innerBox);
            }

            pageView->insertCustomItem(outerBox, i);
        }

        pageView->addEventListener(
            (PageView::ccPageViewCallback)AX_CALLBACK_2(UIPageViewDynamicAddAndRemoveTest::pageViewEvent, this));
        pageView->setName("pageView");
        _uiLayer->addChild(pageView);

        // add buttons
        auto button = Button::create();
        button->setPositionNormalized(Vec2(0.12f, 0.7f));
        button->setTitleText("Add A Page");
        button->setZoomScale(0.3f);
        button->setPressedActionEnabled(true);
        button->setTitleColor(Color3B::RED);
        button->addClickEventListener([=](Object* sender) {
            HBox* outerBox = HBox::create();
            outerBox->setContentSize(Size(240.0f, 130.0f));

            for (int k = 0; k < 2; ++k)
            {
                VBox* innerBox = VBox::create();
                for (int j = 0; j < 3; j++)
                {
                    Button* btn =
                        Button::create("cocosui/animationbuttonnormal.png", "cocosui/animationbuttonpressed.png");
                    btn->setName(fmt::format("button {}", j));
                    innerBox->addChild(btn);
                }

                LinearLayoutParameter* parameter = LinearLayoutParameter::create();
                parameter->setMargin(Margin(0.0f, 0.0f, 100.0f, 0.0f));
                innerBox->setLayoutParameter(parameter);
                outerBox->addChild(innerBox);
            }

            pageView->pushBackCustomItem(outerBox);
            _displayValueLabel->setString(
                fmt::format("page count = {}", static_cast<int32_t>(pageView->getItems().size())));
            AXLOGD("current page index = {}", pageView->getCurrentPageIndex());
        });
        _uiLayer->addChild(button);

        auto button2 = Button::create();
        button2->setPositionNormalized(Vec2(0.12f, 0.5f));
        button2->setTitleText("Remove A Page");
        button2->setZoomScale(0.3f);
        button2->setPressedActionEnabled(true);
        button2->setTitleColor(Color3B::RED);
        button2->addClickEventListener([=](Object* sender) {
            if (pageView->getItems().size() > 0)
            {
                pageView->removeItem(pageView->getItems().size() - 1);
            }
            else
            {
                AXLOGD("There is no page to remove!");
            }
            _displayValueLabel->setString(
                fmt::format("page count = {}", static_cast<int32_t>(pageView->getItems().size())));
            AXLOGD("current page index = {}", pageView->getCurrentPageIndex());
        });
        _uiLayer->addChild(button2);

        auto button3 = Button::create();
        button3->setPositionNormalized(Vec2(0.12f, 0.3f));
        button3->setTitleText("Remove All Pages");
        button3->setZoomScale(0.3f);
        button3->setPressedActionEnabled(true);
        button3->setTitleColor(Color3B::RED);
        button3->addClickEventListener([=](Object* sender) {
            pageView->removeAllItems();
            _displayValueLabel->setString(
                fmt::format("page count = {}", static_cast<int32_t>(pageView->getItems().size())));
            AXLOGD("current page index = {}", pageView->getCurrentPageIndex());
        });
        _uiLayer->addChild(button3);

        auto button4 = (ui::Button*)button3->clone();
        button4->setTitleText("Scroll to Page4");
        button4->setPositionNormalized(Vec2(0.85f, 0.5f));
        button4->addClickEventListener([=](Object* sender) {
            pageView->scrollToItem(3);
            AXLOGD("current page index = {}", pageView->getCurrentPageIndex());
        });
        _uiLayer->addChild(button4);

        return true;
    }
    return false;
}

void UIPageViewDynamicAddAndRemoveTest::pageViewEvent(Object* pSender, PageView::EventType type)
{
    switch (type)
    {
    case PageView::EventType::TURNING:
    {
        PageView* pageView = dynamic_cast<PageView*>(pSender);

        _displayValueLabel->setString(
            fmt::format("page = {}", static_cast<int32_t>((pageView->getCurrentPageIndex() + 1))));
    }
    break;

    default:
        break;
    }
}

// UIPageViewJumpToPageTest
UIPageViewJumpToPageTest::UIPageViewJumpToPageTest() : _displayValueLabel(nullptr) {}

UIPageViewJumpToPageTest::~UIPageViewJumpToPageTest() {}

bool UIPageViewJumpToPageTest::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the dragpanel events will be displayed
        _displayValueLabel = Text::create("setCurrentPageIndex API Test", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5));
        _uiLayer->addChild(_displayValueLabel);

        // Add the black background
        Text* alert = Text::create("PageView", "fonts/Marker Felt.ttf", 30);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = dynamic_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the page view
        PageView* pageView = PageView::create();
        pageView->setContentSize(Size(240.0f, 130.0f));
        Size backgroundSize = background->getContentSize();
        pageView->setPosition(Vec2((widgetSize.width - backgroundSize.width) / 2.0f +
                                       (backgroundSize.width - pageView->getContentSize().width) / 2.0f,
                                   (widgetSize.height - backgroundSize.height) / 2.0f +
                                       (backgroundSize.height - pageView->getContentSize().height) / 2.0f));
        pageView->setIndicatorEnabled(true);
        pageView->removeAllItems();

        int pageCount = 4;
        for (int i = 0; i < pageCount; ++i)
        {
            Layout* layout = Layout::create();
            layout->setContentSize(Size(240.0f, 130.0f));

            ImageView* imageView = ImageView::create("cocosui/scrollviewbg.png");
            imageView->setScale9Enabled(true);
            imageView->setContentSize(Size(240.0f, 130.0f));
            imageView->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));
            layout->addChild(imageView);

            Text* label = Text::create(fmt::format("page {}", (i + 1)), "fonts/Marker Felt.ttf", 30);
            label->setColor(Color3B(192, 192, 192));
            label->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));
            layout->addChild(label);

            pageView->insertCustomItem(layout, i);
        }

        pageView->setCurrentPageIndex(1);

        // add buttons to jump to specific page
        auto button1 = ui::Button::create();
        button1->setPositionNormalized(Vec2(0.1f, 0.75f));
        button1->setTitleText("Jump to Page1");
        AXLOGD("button1 content Size = {}, {}", button1->getContentSize().width, button1->getContentSize().height);
        button1->addClickEventListener([=](Object*) { pageView->setCurrentPageIndex(0); });
        _uiLayer->addChild(button1);

        auto button2 = static_cast<ui::Button*>(button1->clone());
        button2->setTitleText("Jump to Page2");
        button2->setPositionNormalized(Vec2(0.1f, 0.65f));
        AXLOGD("button2 content Size = {}, {}", button2->getContentSize().width, button2->getContentSize().height);
        button2->addClickEventListener([=](Object*) { pageView->setCurrentPageIndex(1); });
        _uiLayer->addChild(button2);

        auto button3 = static_cast<ui::Button*>(button2->clone());
        button3->setTitleText("Jump to Page3");
        button3->setPositionNormalized(Vec2(0.9f, 0.75f));
        button3->addClickEventListener([=](Object*) { pageView->setCurrentPageIndex(2); });
        _uiLayer->addChild(button3);

        auto button4 = static_cast<ui::Button*>(button2->clone());
        button4->setTitleText("Jump to Page4");
        button4->setPositionNormalized(Vec2(0.9f, 0.65f));
        button4->addClickEventListener([=](Object*) { pageView->setCurrentPageIndex(3); });
        _uiLayer->addChild(button4);
        _uiLayer->addChild(pageView);

        return true;
    }
    return false;
}

// UIPageViewVerticalTest
UIPageViewVerticalTest::UIPageViewVerticalTest() : _displayValueLabel(nullptr) {}

UIPageViewVerticalTest::~UIPageViewVerticalTest() {}

bool UIPageViewVerticalTest::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the dragpanel events will be displayed
        _displayValueLabel = Text::create("Move by Vertical direction", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5));
        _uiLayer->addChild(_displayValueLabel);

        // Add the black background
        Text* alert = Text::create("PageView", "fonts/Marker Felt.ttf", 30);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = dynamic_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the page view
        PageView* pageView = PageView::create();
        pageView->setIndicatorEnabled(true);
        pageView->setDirection(ui::PageView::Direction::VERTICAL);
        pageView->setContentSize(Size(240.0f, 130.0f));
        Size backgroundSize = background->getContentSize();
        pageView->setPosition((widgetSize - pageView->getContentSize()) / 2.0f);
        pageView->removeAllItems();

        int pageCount = 4;
        for (int i = 0; i < pageCount; ++i)
        {
            Layout* layout = Layout::create();
            layout->setContentSize(Size(240.0f, 130.0f));

            ImageView* imageView = ImageView::create("cocosui/scrollviewbg.png");
            imageView->setScale9Enabled(true);
            imageView->setContentSize(Size(240.0f, 130.0f));
            imageView->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));
            layout->addChild(imageView);

            Text* label = Text::create(fmt::format("page {}", (i + 1)), "fonts/Marker Felt.ttf", 30);
            label->setColor(Color3B(192, 192, 192));
            label->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));
            layout->addChild(label);

            pageView->insertCustomItem(layout, i);
        }

        pageView->addEventListener(
            (PageView::ccPageViewCallback)AX_CALLBACK_2(UIPageViewVerticalTest::pageViewEvent, this));

        _uiLayer->addChild(pageView);

        return true;
    }
    return false;
}

void UIPageViewVerticalTest::pageViewEvent(Object* pSender, PageView::EventType type)
{
    switch (type)
    {
    case PageView::EventType::TURNING:
    {
        PageView* pageView = dynamic_cast<PageView*>(pSender);

        _displayValueLabel->setString(
            fmt::format("page = {}", static_cast<int32_t>(pageView->getCurrentPageIndex() + 1)));
    }
    break;

    default:
        break;
    }
}

// UIPageViewDisableTouchTest
UIPageViewDisableTouchTest::UIPageViewDisableTouchTest() : _displayValueLabel(nullptr) {}

UIPageViewDisableTouchTest::~UIPageViewDisableTouchTest() {}

bool UIPageViewDisableTouchTest::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the dragpanel events will be displayed
        _displayValueLabel = Text::create("PageView disable touch", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5));
        _uiLayer->addChild(_displayValueLabel);

        // Add the black background
        Text* alert = Text::create("PageView", "fonts/Marker Felt.ttf", 30);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = dynamic_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the page view
        PageView* pageView = PageView::create();
        pageView->setContentSize(Size(240.0f, 130.0f));
        Size backgroundSize = background->getContentSize();
        pageView->setPosition(Vec2((widgetSize.width - backgroundSize.width) / 2.0f +
                                       (backgroundSize.width - pageView->getContentSize().width) / 2.0f,
                                   (widgetSize.height - backgroundSize.height) / 2.0f +
                                       (backgroundSize.height - pageView->getContentSize().height) / 2.0f));
        pageView->setDirection(ui::PageView::Direction::VERTICAL);
        pageView->setTouchEnabled(false);
        pageView->removeAllItems();

        int pageCount = 4;
        for (int i = 0; i < pageCount; ++i)
        {
            Layout* layout = Layout::create();
            layout->setContentSize(Size(240.0f, 130.0f));

            ImageView* imageView = ImageView::create("cocosui/scrollviewbg.png");
            imageView->setScale9Enabled(true);
            imageView->setContentSize(Size(240.0f, 130.0f));
            imageView->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));
            layout->addChild(imageView);

            Text* label = Text::create(fmt::format("page {}", (i + 1)), "fonts/Marker Felt.ttf", 30);
            label->setColor(Color3B(192, 192, 192));
            label->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));
            layout->addChild(label);

            pageView->insertCustomItem(layout, i);
        }

        _uiLayer->addChild(pageView);

        return true;
    }
    return false;
}

// UIPageViewTest
UIPageViewChildSizeTest::UIPageViewChildSizeTest() : _displayValueLabel(nullptr) {}

UIPageViewChildSizeTest::~UIPageViewChildSizeTest() {}

bool UIPageViewChildSizeTest::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the dragpanel events will be displayed
        _displayValueLabel = Text::create("Move by horizontal direction", "fonts/Marker Felt.ttf", 32);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(Vec2(
            widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height * 1.5));
        _uiLayer->addChild(_displayValueLabel);

        // Add the black background
        Text* alert = Text::create("PageView", "fonts/Marker Felt.ttf", 30);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = dynamic_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the page view
        Size size(240, 130);
        PageView* pageView = PageView::create();
        pageView->setDirection(PageView::Direction::HORIZONTAL);
        pageView->setContentSize(size);
        Size backgroundSize = background->getContentSize();
        pageView->setPosition((widgetSize - pageView->getContentSize()) / 2.0f);
        pageView->removeAllItems();
        pageView->setIndicatorEnabled(true);

        int pageCount = 4;
        for (int i = 0; i < pageCount; ++i)
        {
            ImageView* imageView = ImageView::create("cocosui/scrollviewbg.png");
            imageView->setScale9Enabled(true);

            Text* label = Text::create(fmt::format("page {}", (i + 1)), "fonts/Marker Felt.ttf", 30);
            label->setColor(Color3B(192, 192, 192));
            label->setAnchorPoint(Vec2::ZERO);
            imageView->addChild(label);

            pageView->insertCustomItem(imageView, i);
        }

        pageView->addEventListener(
            (PageView::ccPageViewCallback)AX_CALLBACK_2(UIPageViewChildSizeTest::pageViewEvent, this));

        _uiLayer->addChild(pageView);

        return true;
    }
    return false;
}

void UIPageViewChildSizeTest::pageViewEvent(Object* pSender, PageView::EventType type)
{
    switch (type)
    {
    case PageView::EventType::TURNING:
    {
        PageView* pageView = dynamic_cast<PageView*>(pSender);

        _displayValueLabel->setString(
            fmt::format("page = {}", static_cast<int32_t>(pageView->getCurrentPageIndex() + 1)));
    }
    break;

    default:
        break;
    }
}

// UIPageViewIndicatorTest
UIPageViewIndicatorTest::UIPageViewIndicatorTest() : _displayValueLabel(nullptr) {}

UIPageViewIndicatorTest::~UIPageViewIndicatorTest() {}

bool UIPageViewIndicatorTest::init()
{
    if (UIScene::init())
    {
        Size widgetSize = _widget->getContentSize();

        // Add a label in which the dragpanel events will be displayed
        _displayValueLabel = Text::create("PageView indicator custom texture\nscale : 0.5, index color: RED",
                                          "fonts/Marker Felt.ttf", 16);
        _displayValueLabel->setAnchorPoint(Vec2(0.5f, -1.0f));
        _displayValueLabel->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f + _displayValueLabel->getContentSize().height));
        _uiLayer->addChild(_displayValueLabel);

        // Add the black background
        Text* alert = Text::create("PageView", "fonts/Marker Felt.ttf", 30);
        alert->setColor(Color3B(159, 168, 176));
        alert->setPosition(
            Vec2(widgetSize.width / 2.0f, widgetSize.height / 2.0f - alert->getContentSize().height * 3.075f));
        _uiLayer->addChild(alert);

        Layout* root = static_cast<Layout*>(_uiLayer->getChildByTag(81));

        Layout* background = dynamic_cast<Layout*>(root->getChildByName("background_Panel"));

        // Create the page view
        PageView* pageView = PageView::create();
        pageView->setContentSize(Size(240.0f, 130.0f));
        Size backgroundSize = background->getContentSize();
        pageView->setPosition(Vec2((widgetSize.width - backgroundSize.width) / 2.0f +
                                       (backgroundSize.width - pageView->getContentSize().width) / 2.0f,
                                   (widgetSize.height - backgroundSize.height) / 2.0f +
                                       (backgroundSize.height - pageView->getContentSize().height) / 2.0f));
        pageView->setDirection(ui::PageView::Direction::VERTICAL);
        pageView->removeAllItems();

        pageView->setIndicatorEnabled(true);
        pageView->setIndicatorSpaceBetweenIndexNodes(5);
        pageView->setIndicatorIndexNodesScale(0.5);
        pageView->setIndicatorIndexNodesTexture("cocosui/green_edit.png");
        pageView->setIndicatorIndexNodesColor(Color3B::RED);

        int pageCount = 4;
        for (int i = 0; i < pageCount; ++i)
        {
            Layout* layout = Layout::create();
            layout->setContentSize(Size(240.0f, 130.0f));

            ImageView* imageView = ImageView::create("cocosui/scrollviewbg.png");
            imageView->setScale9Enabled(true);
            imageView->setContentSize(Size(240.0f, 130.0f));
            imageView->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));
            layout->addChild(imageView);

            Text* label = Text::create(fmt::format("page {}", (i + 1)), "fonts/Marker Felt.ttf", 30);
            label->setColor(Color3B(192, 192, 192));
            label->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));
            layout->addChild(label);

            pageView->insertCustomItem(layout, i);
        }

        _uiLayer->addChild(pageView);

        return true;
    }
    return false;
}
