#ifndef RENDERER_DEBUG_HPP
#define RENDERER_DEBUG_HPP
#include "enums.hpp"

namespace renderer{

namespace Debug{
    void init();
    void makeSynchronous(bool sync = true);

    void Emit(DebugType type, DebugSeverity severity, int id, const std::string& msg);

    std::string sourceAPIName(DebugSourceAPI s);
    std::string typeName(DebugType t);
    std::string severityName(DebugSeverity s);
  
}

}

#endif //RENDERER_DEBUG_HPP
