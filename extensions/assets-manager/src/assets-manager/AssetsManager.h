/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
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

#ifndef __AssetsManager__
#define __AssetsManager__

#include <string>

#include <mutex>

#include "2d/Node.h"
#include "extensions/ExtensionMacros.h"
#include "extensions/ExtensionExport.h"

struct zlib_filefunc_def_s;

NS_AX_BEGIN

namespace network
{
class Downloader;
}

NS_AX_END  // namespace ax

NS_AX_EXT_BEGIN

class AssetsManagerDelegateProtocol;

/*
 *  This class is used to auto update resources, such as pictures or scripts.
 *  The updated package should be a zip file. And there should be a file named
 *  version in the server, which contains version code.
 */
class AX_EX_DLL AssetsManager : public Node
{
public:
    enum class ErrorCode
    {
        // Error caused by creating a file to store downloaded data
        CREATE_FILE,
        /** Error caused by network
         -- network unavailable
         -- timeout
         -- ...
         */
        NETWORK,
        /** There is not a new version
         */
        NO_NEW_VERSION,
        /** Error caused in uncompressing stage
         -- can not open zip file
         -- can not read file global information
         -- can not read file information
         -- can not create a directory
         -- ...
         */
        UNCOMPRESS,
    };

    /* @brief Creates a AssetsManager with new package url, version code url and storage path.
     *
     * @param packageUrl URL of new package, the package should be a zip file.
     * @param versionFileUrl URL of version file. It should contain version code of new package.
     * @param storagePath The path to store downloaded resources.
     * @js NA
     * @lua new
     */
    AssetsManager(const char* packageUrl = NULL, const char* versionFileUrl = NULL, const char* storagePath = NULL);
    /**
     * @js NA
     * @lua NA
     */
    virtual ~AssetsManager();

    typedef std::function<void(int)> ErrorCallback;
    typedef std::function<void(int)> ProgressCallback;
    typedef std::function<void()> SuccessCallback;

    /* @brief To access within scripting environment
     */
    static AssetsManager* create(const char* packageUrl,
                                 const char* versionFileUrl,
                                 const char* storagePath,
                                 ErrorCallback errorCallback,
                                 ProgressCallback progressCallback,
                                 SuccessCallback successCallback);

    /* @brief Check out if there is a new version resource.
     *        You may use this method before updating, then let user determine whether
     *        he wants to update resources.
     */
    virtual bool checkUpdate();

    using Node::update;
    /* @brief Download new package if there is a new version, and uncompress downloaded zip file.
     *        Ofcourse it will set search path that stores downloaded files.
     */
    virtual void update();

    /* @brief Gets url of package.
     */
    const char* getPackageUrl() const;

    /* @brief Sets package url.
     */
    void setPackageUrl(const char* packageUrl);

    /* @brief Gets version file url.
     */
    const char* getVersionFileUrl() const;

    /* @brief Gets version file url.
     */
    void setVersionFileUrl(const char* versionFileUrl);

    /* @brief Gets current version code.
     */
    std::string_view getVersion();

    /* @brief Deletes recorded version code.
     */
    void deleteVersion();

    /* @brief Gets storage path.
     */
    const char* getStoragePath() const;

    /* @brief Sets storage path.
     *
     * @param storagePath The path to store downloaded resources.
     * @warning The path should be a valid path.
     */
    void setStoragePath(const char* storagePath);

    /** @brief Sets delegate, the delegate will receive messages
     * @js NA
     * @lua NA
     */
    void setDelegate(AssetsManagerDelegateProtocol* delegate);

    /**
     * @js NA
     * @lua NA
     */
    AssetsManagerDelegateProtocol* getDelegate() const { return _delegate; }

    /** @brief Sets connection time out in seconds
     */
    void setConnectionTimeout(unsigned int timeout);

    /** @brief Gets connection time out in seconds
     */
    unsigned int getConnectionTimeout();

protected:
    void checkStoragePath();
    bool uncompress();
    void setSearchPath();
    void downloadAndUncompress();
    void fillZipFunctionOverrides(zlib_filefunc_def_s& zipFunctionOverrides);

private:
    //! The path to store downloaded resources.
    std::string _storagePath;

    //! The version of downloaded resources.
    std::string _version;

    std::string _packageUrl;
    std::string _versionFileUrl;

    std::string _downloadedVersion;

    ax::network::Downloader* _downloader;

    unsigned int _connectionTimeout;

    AssetsManagerDelegateProtocol* _delegate;

    bool _isDownloading;
    bool _shouldDeleteDelegateWhenExit;

    std::string keyOfVersion() const;
    std::string keyOfDownloadedVersion() const;
};

class AssetsManagerDelegateProtocol
{
public:
    virtual ~AssetsManagerDelegateProtocol(){};

public:
    /* @brief Call back function for error
       @param errorCode Type of error
     * @js NA
     * @lua NA
     */
    virtual void onError(AssetsManager::ErrorCode errorCode) {}
    /** @brief Call back function for recording downloading percent
        @param percent How much percent downloaded
        @warning    This call back function just for recording downloading percent.
              AssetsManager will do some other thing after downloading, you should
              write code in onSuccess() after downloading.
     * @js NA
     * @lua NA
     */
    virtual void onProgress(int percent) {}
    /** @brief Call back function for success
     * @js NA
     * @lua NA
     */
    virtual void onSuccess() {}
};

NS_AX_EXT_END

#endif /* defined(__AssetsManager__) */
