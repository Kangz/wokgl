#include <iostream>
#include <GL/glew.h>

#include "Debug.hpp"

namespace renderer{

namespace Debug{

void callback(DebugSourceAPI source, DebugType type, int id, DebugSeverity severity, int length, const char* msg, void* userData){
    std::cout << "Source: " << sourceAPIName(source) << " "; 
    std::cout << "Type: " << typeName(type) << " "; 
    std::cout << "Severity: " << severityName(severity) << " ";
    std::cout << "Id: " << id << " ";
    std::cout << "Message: " << msg;
    std::cout << std::endl;
}

void init(){
    glDebugMessageCallbackARB((GLDEBUGPROCARB)&callback, NULL);
}

void Emit(DebugType type, DebugSeverity severity, int id, const std::string& msg){
    glDebugMessageInsertARB(static_cast<int>(DebugSourceAPI::Application), static_cast<int>(type),
        id, static_cast<int>(severity), msg.length(), msg.c_str());
}

std::string sourceAPIName(DebugSourceAPI s){
    switch(s){
        case DebugSourceAPI::OpenGL:
            return "OpenGL API";
        case DebugSourceAPI::WindowSystem:
            return "Window System";
        case DebugSourceAPI::ShaderCompiler:
            return "Shader Compiler";
        case DebugSourceAPI::ThirdParty:
            return "Third Party";
        case DebugSourceAPI::Application:
            return "Application";
        case DebugSourceAPI::Other:
            return "Other";
        default:
            return "Unknown Source API";
    }
}

std::string typeName(DebugType t){
    switch(t){
        case DebugType::Error:
            return "Error";
        case DebugType::DeprecatedBehavior:
            return "Deprecated Behavior";
        case DebugType::UndefinedBehavior:
            return "Undefined Behavior";
        case DebugType::Portability:
            return "Portability";
        case DebugType::Performance:
            return "Performance";
        case DebugType::Other:
            return "Other";
        default:
            return "Unknown Debug Type";
    }
}
std::string severityName(DebugSeverity s){
    switch(s){
        case DebugSeverity::Low:
            return "Low";
        case DebugSeverity::Medium:
            return "Medium";
        case DebugSeverity::High:
            return "High";
        default:
            return "Unknown Debug Severity";
    }
}

}

}
