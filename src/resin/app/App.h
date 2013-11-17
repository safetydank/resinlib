#pragma once

#include "resin/app/Config.h"

namespace RESIN {

class App
{
  public:
    //  Abstract interface
    virtual void setup();
    virtual void update();
    virtual void draw();

    virtual Config config();

  public:
    //  Public interface

    //  Elapsed time in seconds
    double elapsed() const;
    int width() const;
    int height() const;
    float aspectRatio() const;

  public:
    //  ResinX -> App interface, not for general use
    void setElapsed__(double elapsed);
    void setDimensions__(int width, int height);

    ~App();

  protected:
    App();
    class AppImpl* mImpl;
};

}
