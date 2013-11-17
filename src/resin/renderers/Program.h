#pragma once

#include "resin/core/GL.h"

#include <map>
#include <memory>
#include <string>

namespace RESIN {

typedef std::shared_ptr<struct Program> ProgramRef;
typedef std::map<std::string, GLint> LocationMap;

struct Program
{
    uint32_t id;
    GLuint object;
    std::string code;
    int usedTimes;

    static ProgramRef create();

    //  Cached locations of uniforms and attributes
    LocationMap uniforms;
    LocationMap attributes;

    GLint uniform(const std::string& name) const;
    GLint attribute(const std::string& name) const;

  protected:
    Program();
    
    static int ProgramCount;
};

}
