#include "resin/app/App.h"

namespace RESIN {

class AppImpl
{
  public:
    AppImpl() : elapsed(0)
    {
    }

    double elapsed;
    int width;
    int height;
};

App::App()
{
    mImpl = new AppImpl();
}

App::~App()
{
    delete mImpl;
    mImpl = 0;
}

void App::setup()
{
}

void App::update()
{
}

void App::draw()
{
}

Config App::config()
{
    return Config();
}

double App::elapsed() const
{
    return mImpl->elapsed;
}

int App::width() const
{
    return mImpl->width;
}

int App::height() const
{
    return mImpl->height;
}

float App::aspectRatio() const
{
    return float(width()) / float(height());
};

void App::setElapsed__(double elapsed)
{
    mImpl->elapsed = elapsed;
}

void App::setDimensions__(int width, int height)
{
    mImpl->width = width;
    mImpl->height = height;
}

}

