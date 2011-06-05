#include "MainLogger.hpp"

#include <cassert>
#include <iostream>

void MainLogger::create(int flags, const std::string& file) {
	assert(!instance && "There is already an instance of MainLogger");
	instance = new MainLogger(flags, file);
}

void MainLogger::destroy() {
	delete instance;
	instance = NULL;
}

//Default constructor to avoid a compilation error with Singleton
MainLogger::MainLogger() {
	assert(false && "MainLogger must be created using MainLogger::create");
}

MainLogger::MainLogger(int flags, const std::string& file) :
	_flags(flags) {
	if(!file.empty() && (flags & LOGFILE)) {
		_output.open(file.c_str());
		assert(_output.is_open() && "Could not open file");
	}
}

MainLogger::~MainLogger() {
	_output.close();
}

void MainLogger::write(const std::string& message) {
	if(_flags & CONSOLE)
		std::cout << message << std::endl;
	if(_flags & ERRORCONSOLE)
		std::cerr << message << std::endl;
	if(_flags & LOGFILE) {
		_output << message << std::endl;
	}
}

