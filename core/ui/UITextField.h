/****************************************************************************
Copyright (c) 2013-2016 Chukong Technologies Inc.
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

#ifndef __UITEXTFIELD_H__
#define __UITEXTFIELD_H__

#include "ui/UIWidget.h"
#include "2d/TextFieldTTF.h"
#include "ui/GUIExport.h"

NS_AX_BEGIN
/**
 * @addtogroup ui
 * @{
 */

namespace ui
{

/**
 * @brief A helper class which inherit from @see `TextFieldTTF` and implements the @see `TextFieldDelegate` protocol.
 * It is mainly be used internally by @see `UITextField` class.
 * !!!DEPRECATED since axmol-2.1.3
 * @js NA
 * @lua NA
 */
class AX_GUI_DLL UICCTextField : public TextFieldTTF, public TextFieldDelegate
{
public:
    /**
     * @brief Create an empty UICCTextField.
     *
     * @return A UICCTextField instance.
     */
    static UICCTextField* create();

    /**
     * Default constructor
     */
    UICCTextField();

    /**
     * Default destructor
     */
    ~UICCTextField();

    virtual void onEnter() override;

    /**
     * Create a UICCTextField instance with a placeholder, a fontName and a fontSize.
     *@param placeholder Placeholder in string.
     *@param fontName Font name in string.
     *@param fontSize Font size in float.
     *@return A UICCTextField instance.
     */
    static UICCTextField* create(std::string_view placeholder, std::string_view fontName, float fontSize);

    // override functions
    virtual bool onTextFieldAttachWithIME(TextFieldTTF* pSender) override;
    virtual bool onTextFieldDetachWithIME(TextFieldTTF* pSender) override;
    virtual bool onTextFieldInsertText(TextFieldTTF* pSender, const char* text, size_t nLen) override;
    virtual bool onTextFieldDeleteBackward(TextFieldTTF* pSender, const char* delText, size_t nLen) override;
    void insertText(const char* text, size_t len) override;

    /**
     * Open up the IME.
     */
    void openIME();

    /**
     * Close the IME.
     */
    void closeIME();

    /**
     * Toggle enable max length limitation.
     *@param enable True to enable max length, false otherwise.
     */
    void setMaxLengthEnabled(bool enable);

    /**
     * Query max length enable state.
     *@return Whether max length is enabled or not.
     */
    bool isMaxLengthEnabled() const;

    /**
     * Set maximize length.
     *@param length  The maximize length in integer.
     */
    void setMaxLength(int length);

    /**
     * Get maximize length.
     *@return Maximize length.
     */
    int getMaxLength() const;

    /**
     * Return the total inputed characters.
     *@return Total inputed character count.
     */
    std::size_t getCharCount() const;

    /**
     * @brief Toggle password input mode.
     *
     * @param enable True if enable password input, false otherwise.
     */
    void setPasswordEnabled(bool enable);

    /**
     * @brief Query whether password input mode is enabled or not.
     *
     * @return True if password input is enabled, false otherwise.
     */
    bool isPasswordEnabled() const;

    /**
     * @brief Change password style text.
     *
     * @param styleText The styleText for password mask, the default value is "*".
     */
    void setPasswordStyleText(std::string_view styleText);

    /**
     * @brief Set the password text content.
     *
     * @param text The content of password.
     */
    void setPasswordText(std::string_view text);

    /**
     * @brief Toggle attach with IME.
     *
     * @param attach True if attach with IME, false otherwise.
     */
    void setAttachWithIME(bool attach);

    /**
     * @brief Query whether the IME is attached or not.
     *
     * @return True if IME is attached, false otherwise.
     */
    bool getAttachWithIME() const;

    /**
     * @brief Toggle detach with IME.
     *
     * @param detach True if detach with IME, false otherwise.
     */
    void setDetachWithIME(bool detach);

    /**
     * @brief Query whether IME is detached or not.
     *
     * @return True if IME is detached, false otherwise.
     */
    bool getDetachWithIME() const;

    /**
     * @brief Toggle enable text insert.
     *
     * @param insert True if enable insert text, false otherwise.
     */
    void setInsertText(bool insert);

    /**
     * @brief Query whether insert text is enabled or not.
     *
     * @return True if insert text is enabled, false otherwise.
     */
    bool getInsertText() const;

    /**
     * @brief Toggle enable delete backward.
     *
     * @param deleteBackward True if enable delete backward, false otherwise.
     */
    void setDeleteBackward(bool deleteBackward);

    /**
     * @brief Query whether delete backward is enabled   or not.
     *
     * @return True if delete backward is enabled, false otherwise.
     */
    bool getDeleteBackward() const;

protected:
    bool _maxLengthEnabled;
    int _maxLength;
    bool _attachWithIME;
    bool _detachWithIME;
    bool _insertText;
    bool _deleteBackward;
};

/**
 * @brief A widget which allows users to input text.
 * The rendering of the input text are based on @see `TextFieldTTF'.
 * If you want to use system control behavior, please use @see `EditBox` instead.
 * @js NA
 * @lua NA
 */
class AX_GUI_DLL TextField : public Widget
{

    DECLARE_CLASS_GUI_INFO

public:
    /**
     * TextField event type.
     */
    enum class EventType
    {
        ATTACH_WITH_IME,
        DETACH_WITH_IME,
        INSERT_TEXT,
        DELETE_BACKWARD,
    };
    /**
     * A callback which would be called when a TextField event happens.
     */
    typedef std::function<void(Object*, EventType)> ccTextFieldCallback;

    /**
     * @brief Default constructor.
     *
     */
    TextField();

    /**
     * @brief Default destructor.
     */
    virtual ~TextField();

    /**
     * @brief Create an empty TextField.
     *
     * @return A TextField instance.
     */
    static TextField* create();

    /**
     * @brief Create a TextField with a placeholder, a font name and a font size.
     *
     * @param placeholder The placeholder string.
     * @param fontName The font name.
     * @param fontSize The font size.
     * @return A TextField instance.
     */
    static TextField* create(std::string_view placeholder, std::string_view fontName, int fontSize);

    /**
     * @brief Set the touch size
     * The touch size is used for @see `hitTest`.
     * @param size A delimitation zone.
     */
    void setTouchSize(const Vec2& size);

    /**
     * @brief Get current touch size of TextField.
     *
     * @return The TextField's touch size.
     */
    Vec2 getTouchSize() const;

    /**
     * @brief Toggle enable touch area.
     *
     * @param enable True if enable touch area, false otherwise.
     */
    void setTouchAreaEnabled(bool enable);

    virtual bool hitTest(const Vec2& pt, const Camera* camera, Vec3* p) const override;

    /**
     * @brief Set placeholder of TextField.
     *
     * @param value The string value of placeholder.
     */
    void setPlaceHolder(std::string_view value);

    /**
     * @brief Get the placeholder of TextField.
     *
     * @return A placeholder string.
     */
    std::string_view getPlaceHolder() const;

    /**
     * @brief Query the placeholder string color.
     *
     * @return The color of placeholder.
     */
    const Color4B& getPlaceHolderColor() const;

    /**
     * @brief Change the placeholder color.
     *
     * @param color A color value in `Color3B`.
     */
    void setPlaceHolderColor(const Color3B& color);

    /**
     * @brief Change the placeholder color.
     *
     * @param color A color value in `Color4B`.
     */
    void setPlaceHolderColor(const Color4B& color);

    /**
     * @brief Query the text string color.
     *
     * @return The color of the text.
     */
    const Color4B& getTextColor() const;

    /**
     * @brief Change the text color.
     *
     * @param textColor The color value in `Color4B`.
     */
    void setTextColor(const Color4B& textColor);

    /**
     * @brief Change font size of TextField.
     *
     * @param size The integer font size.
     */
    void setFontSize(int size);

    /**
     * @brief Query the font size.
     *
     * @return The integer font size.
     */
    int getFontSize() const;

    /**
     * @brief Change the font name of TextField.
     *
     * @param name The font name string.
     */
    void setFontName(std::string_view name);

    /**
     * @brief Query the TextField's font name.
     *
     * @return The font name string.
     */
    std::string_view getFontName() const;

    /**
     * @brief Detach the IME.
     */
    virtual void didNotSelectSelf();

    /**
     *Change content of TextField.
     *@param text A string content.
     */
    void setString(std::string_view text);

    /**
     *Query the content of TextField.
     *@return The string value of TextField.
     */
    std::string_view getString() const;

    virtual bool onTouchBegan(Touch* touch, Event* unusedEvent) override;

    /**
     * @brief Toggle maximize length enable
     *
     * @param enable True if enable maximize length, false otherwise.
     */
    void setMaxLengthEnabled(bool enable);

    /**
     * @brief Query whether max length is enabled or not.
     *
     * @return True if maximize length is enabled, false otherwise.
     */
    bool isMaxLengthEnabled() const;

    /**
     * @brief Change maximize input length limitation.
     *
     * @param length A character count in integer.
     */
    void setMaxLength(int length);

    /**
     * @brief Query maximize input length of TextField.
     *
     * @return The integer value of maximize input length.
     */
    int getMaxLength() const;

    /**
     * @brief Query the input string length.
     *
     * @return A integer length value.
     */
    int getStringLength() const;

    /**
     * @brief Toggle enable password input mode.
     *
     * @param enable True if enable password input mode, false otherwise.
     */
    void setPasswordEnabled(bool enable);

    /**
     * @brief Query whether password is enabled or not.
     *
     * @return True if password is enabled, false otherwise.
     */
    bool isPasswordEnabled() const;

    /**
     * @brief Change password style text.
     *
     * @param styleText The styleText for password mask, the default value is "*".
     */
    void setPasswordStyleText(std::string_view styleText);
    /**
     * @brief Query the password style text.
     *
     * @return A password style text.
     */
    std::string_view getPasswordStyleText() const;

    virtual void update(float dt) override;

    /**
     * @brief Query whether the IME is attached or not.
     *
     * @return True if IME is attached, false otherwise.
     */
    bool getAttachWithIME() const;

    /**
     * @brief Toggle attach with IME.
     *
     * @param attach True if attach with IME, false otherwise.
     */
    void setAttachWithIME(bool attach);

    /**
     * @brief Query whether IME is detached or not.
     *
     * @return True if IME is detached, false otherwise.
     */
    bool getDetachWithIME() const;

    /**
     * @brief Toggle detach with IME.
     *
     * @param detach True if detach with IME, false otherwise.
     */
    void setDetachWithIME(bool detach);

    /**
     * @brief Whether it is ready to get the inserted text or not.
     *
     * @return True if the insert text is ready, false otherwise.
     */
    bool getInsertText() const;

    /**
     * @brief Toggle enable insert text mode
     *
     * @param insertText True if enable insert text, false otherwise.
     */
    void setInsertText(bool insertText);

    /**
     * @brief Whether it is ready to delete backward in TextField.
     *
     * @return True is the delete backward is enabled, false otherwise.
     */
    bool getDeleteBackward() const;

    /**
     * @brief Toggle enable delete backward mode.
     *
     * @param deleteBackward True is delete backward is enabled, false otherwise.
     */
    void setDeleteBackward(bool deleteBackward);

    /**
     * Add a event listener to TextField, when some predefined event happens, the callback will be called.
     *@param callback A callback function with type of `ccTextFieldCallback`.
     */
    void addEventListener(const ccTextFieldCallback& callback);

    /**
     * Returns the "class name" of widget.
     */
    virtual std::string getDescription() const override;

    /**
     * @brief Get the renderer size in auto mode.
     *
     * @return A delimitation zone.
     */
    virtual Vec2 getAutoRenderSize();
    // override functions.
    virtual Vec2 getVirtualRendererSize() const override;
    virtual Node* getVirtualRenderer() override;
    virtual void onEnter() override;

    /**
     * @brief Attach the IME for inputing.
     *
     */
    void attachWithIME();

    /**
     * @brief Detach the IME from inputing.
     *
     */
    void detachWithIME();

    /**
     * @brief Change the text area size.
     *
     * @param size A delimitation zone.
     */
    void setTextAreaSize(const Vec2& size);

    /**
     * @brief Change horizontal text alignment.
     *
     * @param alignment A alignment arguments in @see `TextHAlignment`.
     */
    void setTextHorizontalAlignment(TextHAlignment alignment);

    /**
     * @brief Inquire the horizontal alignment
     *
     * @return The horizontal alignment
     */
    TextHAlignment getTextHorizontalAlignment() const;

    /**
     * @brief Change the vertical text alignment.
     *
     * @param alignment A alignment arguments in @see `TextVAlignment`.
     */
    void setTextVerticalAlignment(TextVAlignment alignment);

    /**
     * @brief Inquire the horizontal alignment
     *
     * @return The horizontal alignment
     */
    TextVAlignment getTextVerticalAlignment() const;

    /**
     * Set enable cursor use.
     * @js NA
     */
    void setCursorEnabled(bool enabled);

    /**
     * Set char showing cursor.
     * @js NA
     */
    void setCursorChar(char cursor);

    /**
     * Set cursor position, if enabled
     * @js NA
     */
    void setCursorPosition(std::size_t cursorPosition);

    /**
     * Set cursor position to hit letter, if enabled
     * @js NA
     */
    void setCursorFromPoint(const Vec2& point, const Camera* camera);

    virtual bool init() override;

protected:
    virtual void initRenderer() override;
    void attachWithIMEEvent();
    void detachWithIMEEvent();
    void insertTextEvent();
    void deleteBackwardEvent();
    virtual void onSizeChanged() override;

    void textfieldRendererScaleChangedWithSize();

    virtual Widget* createCloneInstance() override;
    virtual void copySpecialProperties(Widget* model) override;
    virtual void adaptRenderers() override;

protected:
    UICCTextField* _textFieldRenderer;

    float _touchWidth;
    float _touchHeight;
    bool _useTouchArea;

    Object* _textFieldEventListener;
    ccTextFieldCallback _eventCallback;

    bool _textFieldRendererAdaptDirty;

protected:
    enum class FontType
    {
        SYSTEM,
        TTF,
        BMFONT
    };

    std::string _fontName;
    int _fontSize;
    FontType _fontType;
};

}  // namespace ui

// end of ui group
/// @}
NS_AX_END

#endif /* defined(__TextField__) */
