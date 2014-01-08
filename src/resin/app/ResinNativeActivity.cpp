#include "resin/app/ResinNativeActivity.h"

#include <errno.h>

#include <android/asset_manager.h>
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include <map>

using std::vector;
using std::map;
// using ci::app::TouchEvent;
// using ci::app::Orientation_t;

namespace RESIN {

namespace {

const char* actionNames[] = {
    "AMOTION_EVENT_ACTION_DOWN",
    "AMOTION_EVENT_ACTION_UP",
    "AMOTION_EVENT_ACTION_MOVE",
    "AMOTION_EVENT_ACTION_CANCEL",
    "AMOTION_EVENT_ACTION_OUTSIDE",
    "AMOTION_EVENT_ACTION_POINTER_DOWN",
    "AMOTION_EVENT_ACTION_POINTER_UP",
};

struct InputState
{
    ResinNativeActivity*               cinderApp;
    vector<TouchEvent::Touch> touchesBegan;
    vector<TouchEvent::Touch> touchesMoved;
    vector<TouchEvent::Touch> touchesEnded;

    map<int32_t, TouchEvent::Touch> activeTouches;

    int32_t handleInput(AInputEvent* event)
    {
        int32_t eventType = AInputEvent_getType(event);

        if (eventType == AINPUT_EVENT_TYPE_MOTION) {
            int32_t actionCode = AMotionEvent_getAction(event);
            int action = actionCode & AMOTION_EVENT_ACTION_MASK;
            int index  = (actionCode & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
            const char* actionName = (action >= 0 && action <= 6) ? actionNames[action] : "UNKNOWN";
            // CI_LOGI("Received touch action %s pointer index %d", actionName, index);

            double timestamp = cinderApp->getElapsedSeconds();
            if (action == AMOTION_EVENT_ACTION_DOWN || action == AMOTION_EVENT_ACTION_POINTER_DOWN) {
                int pointerId = AMotionEvent_getPointerId(event, index);
                float x = AMotionEvent_getX(event, index);
                float y = AMotionEvent_getY(event, index);
                TouchEvent::Touch touch(ci::Vec2f(x, y), ci::Vec2f(x, y), pointerId, timestamp, NULL);
                touchesBegan.push_back(touch);
                activeTouches.insert(std::make_pair(pointerId, touch));

                // CI_LOGI("Pointer id %d down x %f y %f", pointerId, x, y);
            }
            else if (action == AMOTION_EVENT_ACTION_MOVE) {
                int pointerCount = AMotionEvent_getPointerCount(event);

                for (int i=0; i < pointerCount; ++i) {
                    int pointerId = AMotionEvent_getPointerId(event, i);
                    float x = AMotionEvent_getX(event, i);
                    float y = AMotionEvent_getY(event, i);
                    map<int, TouchEvent::Touch>::iterator it = activeTouches.find(pointerId);
                    if (it != activeTouches.end()) {
                        TouchEvent::Touch& prevTouch = it->second;
                        TouchEvent::Touch touch(ci::Vec2f(x, y), prevTouch.getPos(), pointerId, timestamp, NULL);
                        touchesMoved.push_back(touch);
                        activeTouches.erase(pointerId);
                        activeTouches.insert(std::make_pair(pointerId, touch));
                        // CI_LOGI("Pointer id %d move x %f y %f", pointerId, x, y);
                    }
                }
            }
            else if (action == AMOTION_EVENT_ACTION_UP || action == AMOTION_EVENT_ACTION_POINTER_UP) {
                int pointerId = AMotionEvent_getPointerId(event, index);
                float x = AMotionEvent_getX(event, index);
                float y = AMotionEvent_getY(event, index);
                touchesEnded.push_back(TouchEvent::Touch(ci::Vec2f(x, y), ci::Vec2f(x, y), pointerId, timestamp, NULL));
                activeTouches.erase(pointerId);
                // CI_LOGI("Pointer id %d up x %f y %f", pointerId, x, y);
            }

            return 1;
        }
        else if (eventType == AINPUT_EVENT_TYPE_KEY) {
            int32_t actionCode     = AKeyEvent_getAction(event);
            int32_t keyCode        = AKeyEvent_getKeyCode(event);
            int32_t metaState      = AKeyEvent_getMetaState(event);
            unsigned int modifiers = 0;

            if (metaState & AMETA_SHIFT_ON)
                modifiers |= KeyEvent::SHIFT_DOWN;
            if (metaState & AMETA_SYM_ON) {
                modifiers |= KeyEvent::CTRL_DOWN;
                modifiers |= KeyEvent::META_DOWN;
            }
            if (metaState & AMETA_ALT_ON) {
                modifiers |= KeyEvent::ALT_DOWN;
            }

            WindowRef window = cinderApp->getWindow();
            KeyEvent kevent(window, keyCode, keyCode, (char) keyCode, modifiers, 0);

            if (actionCode == AKEY_EVENT_ACTION_DOWN) {
                window->emitKeyDown(&kevent);
            }
            else if (actionCode == AKEY_STATE_UP) {
                window->emitKeyUp(&kevent);
            }
        }

        return 0;
    }

    void updateInput() 
    {
        if (cinderApp->getSettings().isMultiTouchEnabled()) {
            updateMultiTouches();
        }
        else {
            updateSingleTouch();
        }
    }

    void updateMultiTouches() 
    {
        ResinNativeActivity& app = *cinderApp;
        WindowRef window = app.getWindow();

        if ( ! touchesBegan.empty() ) {
            TouchEvent event(window, touchesBegan);
            window->emitTouchesBegan(&event);
            touchesBegan.clear();
        }
        if ( ! touchesMoved.empty() ) {
            TouchEvent event(window, touchesMoved);
            window->emitTouchesMoved(&event);
            touchesMoved.clear();
        }
        if ( ! touchesEnded.empty() ) {
            TouchEvent event(window, touchesEnded);
            window->emitTouchesEnded(&event);
            touchesEnded.clear();
        }

        //  set active touches
        vector<ci::app::TouchEvent::Touch> activeList;
        auto& activeMap = activeTouches;
        for (auto it = activeMap.begin(); it != activeMap.end(); ++it) {
           activeList.push_back(it->second);
        }
        app.privateSetActiveTouches__(activeList);
    }

    void updateSingleTouch() 
    {
        const float contentScale = 1.0f;

        using cinder::app::MouseEvent;

        //  Mouse emulation if multi-touch is disabled
        ResinNativeActivity& app = *cinderApp;
        for ( TouchEvent::Touch& touch : touchesBegan ) {
            ci::Vec2f pt = touch.getPos();
            uint32_t mods = 0;
            mods |= cinder::app::MouseEvent::LEFT_DOWN;
            MouseEvent event( app.getWindow(), MouseEvent::LEFT_DOWN, int(pt.x * contentScale), int(pt.y * contentScale), mods, 0.0f, 0 );
            app.getWindow()->emitMouseDown( &event );
        }
        touchesBegan.clear();
        for ( TouchEvent::Touch& touch : touchesMoved ) {
            ci::Vec2f pt = touch.getPos();
            uint32_t mods = 0;
            mods |= cinder::app::MouseEvent::LEFT_DOWN;
            MouseEvent event( app.getWindow(), 0, int(pt.x * contentScale), int(pt.y * contentScale), mods, 0.0f, 0 );
            app.getWindow()->emitMouseDrag( &event );
        }
        touchesMoved.clear();
        for ( TouchEvent::Touch& touch : touchesEnded ) {
            ci::Vec2f pt = touch.getPos();
            int mods = 0;
            mods |= cinder::app::MouseEvent::LEFT_DOWN;
            MouseEvent event( app.getWindow(), MouseEvent::LEFT_DOWN, int(pt.x * contentScale), int(pt.y * contentScale), mods, 0.0f, 0 );
            app.getWindow()->emitMouseUp( &event );
        }
        touchesEnded.clear();
    }
};

}  // anonymous namespace


/**
 * Shared state for our app, platform specifics
 */
class ResinNativeActivityImpl
{
  public:
    void* savedState;

    //  orientation
    Orientation_t orientation;

    //  JNI access helpers
    JavaVM* vm;

  protected:
    enum ActivityState {
        ACTIVITY_START = 0,
        ACTIVITY_RESUME,
        ACTIVITY_PAUSE,
        ACTIVITY_STOP,
        ACTIVITY_DESTROY
    };

    ci::app::ResinNativeActivity* cinderApp;
    struct android_app*  androidApp;
    InputState           inputState;

    // ASensorManager*    sensorManager;
    // ASensorEventQueue* sensorEventQueue;

    int animating;

    ActivityState activityState;

    bool renewContext;
    bool setupCompleted;
    bool resumed;

  public:
    static int32_t onInput(struct android_app* app, AInputEvent* event)
    {
        ResinNativeActivityImpl *impl = static_cast<ResinNativeActivityImpl *>(app->userData);
        return impl->inputState.handleInput(event);
    }

    static void onCmd(struct android_app* app, int32_t cmd)
    {
        ResinNativeActivityImpl *impl = static_cast<ResinNativeActivityImpl *>(app->userData);
        impl->handleCmd(cmd);
    }

    static void onThreadEnded(void* ptr)
    {
        if (ptr) {
            // CI_LOGD("XXX Detach native thread");
            JavaVM* vm = (JavaVM*) ptr;
            vm->DetachCurrentThread();
        }
    }

  public:
    ResinNativeActivityImpl(ci::app::ResinNativeActivity* cinderApp, struct android_app* androidApp)
        : cinderApp(cinderApp), androidApp(androidApp),
          renewContext(true), setupCompleted(false), resumed(false)
    {
        inputState.cinderApp = cinderApp;

        vm             = androidApp->activity->vm;
        savedState     = NULL;

        JNIEnv* env;
        vm->AttachCurrentThread(&env, NULL);

        pthread_key_t key;
        pthread_key_create(&key, ResinNativeActivityImpl::onThreadEnded);
        pthread_setspecific(key, vm);

        androidApp->userData     = this;
        androidApp->onAppCmd     = ResinNativeActivityImpl::onCmd;
        androidApp->onInputEvent = ResinNativeActivityImpl::onInput;

        if (androidApp->savedState != NULL) {
            // We are starting with a previous saved state; restore from it.
            savedState = androidApp->savedState;
            // XXX currently restores via setup(), possibly better to use resume()?
            // engine.resumed = true;
        }

        animating = 0;
    }

    bool eventLoop()
    {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(animating ? 0 : -1, NULL, &events, (void**)&source)) >= 0) {
            // Process this event.
            if (source != NULL) {
                source->process(androidApp, source);
            }

            // XXX Emit event signal 

            // Check if we are exiting.
            if (androidApp->destroyRequested != 0) {
                animating = 0;
                if (auto renderer = cinderApp->getRenderer()) {
                    renderer->teardown();
                }
                return false;
            }
        }

        //  Update engine touch state
        inputState.updateInput();

        if (animating) {
            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            drawFrame();
        }

        return true;
    }

    void drawFrame()
    {
        ci::app::ResinNativeActivity& app      = *cinderApp;
        ci::app::RendererRef renderer = cinderApp->getRenderer();

        if (!renderer || !renderer->isValidDisplay()) {
            CI_LOGW("No valid display, skipping render");
            // No display.
            return;
        }

        //  XXX handles delayed window size updates from orientation changes
        app.updateWindowSizes();

        // renderer->startDraw();
        app.privateUpdate__();
        app.draw();
        renderer->finishDraw();
    }

#if defined( NDEBUG )
    #define LOG_STATE 
#else
    #define LOG_STATE logState();
#endif

    void handleCmd(int32_t cmd)
    {
        switch (cmd) {

        case APP_CMD_SAVE_STATE:
            LOG_STATE
            cinderApp->setSavedState(&(androidApp->savedState), &(androidApp->savedStateSize));
            break;

        case APP_CMD_INIT_WINDOW:
            LOG_STATE
            // The window is being shown, get it ready.
            if (androidApp->window != NULL) {
                //  Create default window & renderer
                cinderApp->preSetup();
                animating = 0;
                orientation = cinderApp->orientationFromConfig();
                cinderApp->getRenderer()->setup(cinderApp, androidApp, &(cinderApp->mWidth), &(cinderApp->mHeight));
                cinderApp->updateWindowSizes();
                cinderApp->privatePrepareSettings__();

                //  New GL context, trigger app initialization
                setupCompleted = false;
                renewContext = true;
            }
            break;

        case APP_CMD_TERM_WINDOW:
            LOG_STATE
            // The window is being hidden or closed, clean it up.
            animating = 0;
            if (auto renderer = cinderApp->getRenderer()) {
                renderer->teardown();
            }
            break;

        case APP_CMD_GAINED_FOCUS:
            LOG_STATE

            // Emit gained focus

            if (!setupCompleted) {
                if (resumed) {
                    // CI_LOGD("XXXXXX RESUMING privateResume__ renew context %s", renewContext ? "true" : "false");
                    cinderApp->privateResume__(renewContext);
                }
                else {
                    // CI_LOGD("XXXXXX SETUP privateSetup__");
                    cinderApp->privateSetup__();
                }
                // cinderApp->privateResize__(ci::Vec2i( cinderApp->mWidth, cinderApp->mHeight ));
                cinderApp->getWindow()->emitResize();
                setupCompleted = true;
                renewContext   = false;
                resumed        = false;

                drawFrame();
            }

            animating = 1;
            break;

        case APP_CMD_LOST_FOCUS:
            LOG_STATE
            animating = 0;
            drawFrame();
            break;

        case APP_CMD_RESUME:
            activityState = ACTIVITY_RESUME;
            LOG_STATE
            break;
        
        case APP_CMD_START:
            activityState = ACTIVITY_START;
            LOG_STATE
            break;

        case APP_CMD_PAUSE:
            activityState = ACTIVITY_PAUSE;
            cinderApp->privatePause__();
            animating = 0;
            resumed = true;
            drawFrame();
            LOG_STATE
            break;

        case APP_CMD_STOP:
            activityState = ACTIVITY_STOP;
            LOG_STATE
            break;

        case APP_CMD_DESTROY:
            //  app has been destroyed, will crash if we attempt to do anything else
            activityState = ACTIVITY_DESTROY;
            cinderApp->privateDestroy__();
            LOG_STATE
            break;

        case APP_CMD_CONFIG_CHANGED:
            orientation = cinderApp->orientationFromConfig();
            break;
        }
    }

    void logState()
    {
        static const char* activityStates[] = {
            "Start",
            "Resume",
            "Pause",
            "Stop",
            "Destroy"
        };
        CI_LOGD("Engine activity state: %s", activityStates[activityState]);
    }
};

ResinNativeActivity* ResinNativeActivity::sInstance;

///////////////////////////////////////////////////////////////////////////////
// WindowImplAndroid

WindowImplAndroid::WindowImplAndroid( const Window::Format &format, RendererRef sharedRenderer, ResinNativeActivity *appImpl )
    : mAppImpl( appImpl ), mNativeWindow( NULL ), mWindowWidth(0), mWindowHeight(0)
{
    mFullScreen   = format.isFullScreen();
    mDisplay      = format.getDisplay();
    mRenderer     = format.getRenderer();
    mResizable    = format.isResizable();
    mAlwaysOnTop  = format.isAlwaysOnTop();
    mBorderless   = format.isBorderless();
    // mWindowedSize = format.getSize();
    mWindowRef    = Window::privateCreate__( this, mAppImpl );
    mNativeWindow = appImpl->mAndroidApp->window;
}

void WindowImplAndroid::updateWindowSize()
{
    if (mNativeWindow) {
        int32_t newWidth  = ANativeWindow_getWidth(mNativeWindow);
        int32_t newHeight = ANativeWindow_getHeight(mNativeWindow);
        if ( newWidth != mWindowWidth || newHeight != mWindowHeight ) {
            mAppImpl->setWindowSize(newWidth, newHeight);
            // mAppImpl->privateResize__(ci::Vec2i(winWidth, winHeight));
            mWindowWidth  = newWidth;
            mWindowHeight = newHeight;
            mWindowRef->emitResize();
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// ResinNativeActivity

ResinNativeActivity::ResinNativeActivity()
    : App(), mAndroidApp(0)
{
}

ResinNativeActivity::~ResinNativeActivity()
{
    CI_LOGD("~ResinNativeActivity()");
    JNIEnv* env = getJNIEnv();
    env->DeleteGlobalRef(mClassLoader);
}

void ResinNativeActivity::pause()
{
}

void ResinNativeActivity::resume( bool renewContext )
{
    //  Override this to handle lost/recreated GL context
    //  You should recreate your GL context assets (textures/shaders) in this method
    if (renewContext) {
        setup();
    }
}

void ResinNativeActivity::destroy()
{
}

void ResinNativeActivity::setSavedState(void** state, size_t* size)
{
    *state = NULL;
    *size = 0;
}

void* ResinNativeActivity::getSavedState()
{
    return mImpl->savedState;
}

fs::path ResinNativeActivity::getInternalDataPath() const
{
    const char* path = (mAndroidApp && mAndroidApp->activity) ? 
        mAndroidApp->activity->internalDataPath : NULL;
    return path ? fs::path(path) : fs::path();
}

fs::path ResinNativeActivity::getExternalDataPath() const
{
    const char* path = (mAndroidApp && mAndroidApp->activity) ? 
        mAndroidApp->activity->externalDataPath : NULL;
    return path ? fs::path(path) : fs::path();
}

int32_t ResinNativeActivity::getSdkVersion()
{
    return (mAndroidApp && mAndroidApp->activity) ?
        mAndroidApp->activity->sdkVersion : -1;
}

void ResinNativeActivity::setNativeAndroidState( struct android_app* androidApp )
{
    mAndroidApp = androidApp;
}

void ResinNativeActivity::launch( const char *title, int argc, char * const argv[] )
{
    clock_gettime(CLOCK_MONOTONIC, &mStartTime);

    app_dummy();
    mImpl = new ResinNativeActivityImpl(this, mAndroidApp);

    while (mImpl->eventLoop()) ;

    //  delete mImpl;
}

int ResinNativeActivity::getWindowDensity() const
{
    int density = AConfiguration_getDensity(mAndroidApp->config);
    if (density == ACONFIGURATION_DENSITY_DEFAULT || density == ACONFIGURATION_DENSITY_NONE)
        density = ACONFIGURATION_DENSITY_MEDIUM;

    return density;
}

void ResinNativeActivity::updateWindowSizes()
{
    for (auto window : mWindows)
        window->updateWindowSize();
}

void ResinNativeActivity::preSetup()
{
    // Only single window support on Android 
    mActiveWindow = createWindow(getSettings().getDefaultWindowFormat());
}

WindowRef ResinNativeActivity::createWindow( Window::Format format )
{
    //  Single window support on Android
    if( ! mWindows.empty() )
        return getWindow();

    if( ! format.getRenderer() ) {
        format.setRenderer( getDefaultRenderer()->clone() );
    }

    // XXX ???
    // mWindows.push_back( new WindowImplAndroid( format, findSharedRenderer( format.getRenderer() ), this ) );
    mWindows.push_back( new WindowImplAndroid( format, format.getRenderer(), this ) );

    // XXX ??? emit initial resize if we have fired setup
    // if ( mSetupHasBeenCalled )
    //     mWindows.back()->getWindow()->emitResize();

    return mWindows.front()->getWindow();
}


//! Returns the maximum frame-rate the App will attempt to maintain.
float ResinNativeActivity::getFrameRate() const
{
    return 0;
}

//! Sets the maximum frame-rate the App will attempt to maintain.
void ResinNativeActivity::setFrameRate( float aFrameRate )
{
}

//! Returns whether the App is in full-screen mode or not.
bool ResinNativeActivity::isFullScreen() const
{
    return true;
}

//! Sets whether the active App is in full-screen mode based on \a fullScreen
void ResinNativeActivity::setFullScreen( bool aFullScreen )
{
}

double ResinNativeActivity::getElapsedSeconds() const
{
    struct timespec currentTime;
    clock_gettime(CLOCK_MONOTONIC, &currentTime);
    return ( (currentTime.tv_sec + currentTime.tv_nsec / 1e9) 
            - (mStartTime.tv_sec + mStartTime.tv_nsec / 1e9) );
}

fs::path ResinNativeActivity::getAppPath()
{ 
    // XXX TODO
    return fs::path();
}

void ResinNativeActivity::quit()
{
    return;
}

Orientation_t ResinNativeActivity::orientation()
{
    return mImpl->orientation;
}

void ResinNativeActivity::privatePrepareSettings__()
{
    prepareSettings( &mSettings );
}

void ResinNativeActivity::privatePause__()
{
    pause();
}

void ResinNativeActivity::privateResume__( bool renewContext ) 
{
    resume(renewContext);
}

void ResinNativeActivity::privateDestroy__()
{
    destroy();
}

void ResinNativeActivity::privateTouchesBegan__( const TouchEvent &event )
{
    bool handled = false;
    if( ! handled )
        touchesBegan( event );
}

void ResinNativeActivity::privateTouchesMoved__( const TouchEvent &event )
{
    bool handled = false;
    if( ! handled )
        touchesMoved( event );
}

void ResinNativeActivity::privateTouchesEnded__( const TouchEvent &event )
{
    bool handled = false;
    if( ! handled )
        touchesEnded( event );
}

Orientation_t ResinNativeActivity::orientationFromConfig()
{
    Orientation_t ret = ORIENTATION_ANY;
    int orient = AConfiguration_getOrientation(mAndroidApp->config);

    switch (orient) {
        case ACONFIGURATION_ORIENTATION_PORT:
            ret = ORIENTATION_PORTRAIT;
            break;
        case ACONFIGURATION_ORIENTATION_LAND:
            ret = ORIENTATION_LANDSCAPE;
            break;
        case ACONFIGURATION_ORIENTATION_SQUARE:
            ret = ORIENTATION_SQUARE;
            break;
        default:
            break;
    }

    return ret;
}

WindowRef ResinNativeActivity::getWindow() const
{
    return mWindows.front()->getWindow();
}

size_t ResinNativeActivity::getNumWindows() const
{
    return 1;
}

WindowRef ResinNativeActivity::getWindowIndex( size_t index ) const
{
    if (index != 0 || mWindows.empty())
        throw ExcInvalidWindow();

    return mWindows.front()->getWindow();
}

#if defined( CINDER_AASSET )

//  static loadResource method, loads from assets/
DataSourceAssetRef ResinNativeActivity::loadResource(const std::string &resourcePath)
{
    ResinNativeActivity* cinderApp = ResinNativeActivity::get();
    if (cinderApp) {
        AAssetManager* mgr = cinderApp->mAndroidApp->activity->assetManager;
        return DataSourceAsset::create(mgr, resourcePath);
    }
    else {
        throw ResourceLoadExc( resourcePath );
    }
}

bool ResinNativeActivity::hasResource(const fs::path& assetPath)
{
    ResinNativeActivity* cinderApp = ResinNativeActivity::get();
    AAssetManager* mgr = cinderApp->mAndroidApp->activity->assetManager;
    AAsset* asset = AAssetManager_open(mgr, assetPath.string().c_str(), AASSET_MODE_STREAMING);
    bool hasResource = bool(asset);
    AAsset_close(asset);
    return hasResource;
}

void ResinNativeActivity::copyResource(const fs::path& assetPath, const fs::path& destDir, bool overwrite)
{
    ResinNativeActivity* cinderApp = ResinNativeActivity::get();
    AAssetManager* mgr = cinderApp->mAndroidApp->activity->assetManager;

    if (assetPath.empty() || destDir.empty()) {
        CI_LOGE("copyResource: Missing assetPath or destination dir");
        return;
    }

    // CI_LOGD("Copying resource %s to %s", assetPath.string().c_str(), destDir.string().c_str());
    fs::path outPath = destDir / assetPath.filename();

    if (fs::exists(outPath) && !overwrite)
        return;

    {
        // XXX fix createParentDirs in writeFileStream
        OStreamFileRef os = writeFileStream(outPath);
        if (!os) {
            return;
        }
        AAsset* asset = AAssetManager_open(mgr, assetPath.string().c_str(), AASSET_MODE_STREAMING);

        const int BUFSIZE = 8192;
        unsigned char buf[BUFSIZE];
        int readSize;

        while (true) {
            readSize = AAsset_read(asset, (void *) buf, BUFSIZE);
            if (readSize > 0) {
                os->writeData(buf, readSize);
            }
            else {
                break;
            }
        }
        AAsset_close(asset);
    }
}

void ResinNativeActivity::copyResourceDir(const fs::path& assetPath, const fs::path& destDir, bool overwrite)
{
    ResinNativeActivity* cinderApp = ResinNativeActivity::get();
    AAssetManager* mgr = cinderApp->mAndroidApp->activity->assetManager;

    // XXX Untested
    AAssetDir* dir = AAssetManager_openDir(mgr, assetPath.string().c_str());
    fs::path outDir = destDir / assetPath.filename();
    if (!fs::exists(outDir)) {
        fs::create_directory(outDir);
    }

    char* filename = NULL;
    while (true) {
        const char* filename = AAssetDir_getNextFileName(dir);
        if (filename == NULL) {
            break;
        }
        copyResource(filename, outDir, overwrite);
    }
    AAssetDir_close(dir);
}

JavaVM* ResinNativeActivity::getJavaVM()
{
    return mImpl->vm;
}

JNIEnv* ResinNativeActivity::getJNIEnv()
{
    JNIEnv* env = 0;
    int err = mImpl->vm->GetEnv((void**) &env, JNI_VERSION_1_4);
    if (err == JNI_EDETACHED) {
        CI_LOGE("getJNIEnv error: current thread not attached to Java VM");
    }
    else if (err == JNI_EVERSION) {
        CI_LOGE("getJNIEnv error: VM doesn't support requested JNI version");
    }
    return env;
}

#define MAX_LOCAL_REFS 20
void ResinNativeActivity::initJNI()
{
    JNIEnv* env = getJNIEnv();
    env->PushLocalFrame(MAX_LOCAL_REFS);
    jclass activityClass     = env->FindClass("android/app/NativeActivity");
    jmethodID getClassLoader = env->GetMethodID(activityClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
    jclass loader            = env->FindClass("java/lang/ClassLoader");

    mClassLoader             = env->NewGlobalRef(env->CallObjectMethod(mAndroidApp->activity->clazz, getClassLoader));
    mFindClassMID            = env->GetMethodID(loader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

    env->PopLocalFrame(NULL);
}

jclass ResinNativeActivity::findClass(const char* className)
{
    JNIEnv* env = getJNIEnv();
    jstring strClassName  = env->NewStringUTF(className);
    jclass theClass = static_cast<jclass>(env->CallObjectMethod(mClassLoader, mFindClassMID, strClassName));
    env->DeleteLocalRef(strClassName);

    //  Returns a local class reference, that will have to be deleted by the caller
    return theClass;
}

jobject ResinNativeActivity::getActivity()
{
    return mAndroidApp->activity->clazz;
}

#endif

fs::path ResinNativeActivity::getAppPath() const
{
    return fs::path();
}

} // namespace RESIN

