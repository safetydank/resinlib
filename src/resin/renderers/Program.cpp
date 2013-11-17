#include "Program.h"

namespace RESIN {

int Program::ProgramCount = 1;

ProgramRef Program::create()
{
    return ProgramRef(new Program);
}

Program::Program()
{
    id = ProgramCount++;
}

GLint Program::uniform(const std::string& name) const
{
    auto it = uniforms.find(name);
    return (it == uniforms.end()) ? -1 : it->second;
}

GLint Program::attribute(const std::string& name) const
{
    auto it = attributes.find(name);
    return (it == attributes.end()) ? -1 : it->second;
}


}

