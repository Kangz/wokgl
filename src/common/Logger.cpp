#include "Logger.hpp"

#include <cassert>

#include "MainLogger.hpp"

Logger::endlStruct Logger::endl = Logger::endlStruct();

Logger* Logger::create(int minPriority, const std::string& name) {
	return new Logger(minPriority, name);
}

void Logger::destroy(Logger*& logger) {
	delete logger;
	logger = 0;
}

Logger::Logger(int minPriority, const std::string& name) :
	_minPriority(minPriority), _name(name) {
	assert(_name.size() <= 10 && "Logger's name is too long");
	for(int i = DEBUG; i <= FATAL; ++i) {
		_streams.push_back(new Stream(this, i));
	}

	int spaceLength = 5 - (_name.length()/2);
	std::string spaces;
	for(int i = 0; i < spaceLength; ++i) {
		spaces.push_back(' ');
	}
	std::string newName = spaces + _name + spaces;
	if(_name.length()%2) {
		newName.push_back(' ');
	}
	_name = newName;
}

Logger::~Logger() {
	for(std::vector<Stream*>::iterator it = _streams.begin(); it != _streams.end(); ++it) {
		delete (*it);
	}
}

void Logger::write(const std::string& message, int priority) {
	if(priority >= _minPriority) {
		time_t rawtime;
		struct tm* timeinfo;
		char buffer[80];
		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, 80, "%x %X", timeinfo);
		MainLogger::getInstance().write(std::string(buffer) + " | " + _name + " | " + message);
	}
}

Logger::Stream::Stream(Logger* parent, int priority) {
	_parent = parent;
	_priority = priority;
}

Logger::Stream::Stream(const Stream& other) {
	_parent = other._parent;
	_priority = other._priority;
	_buffer.str(other._buffer.str());
}

Logger::Stream& Logger::Stream::operator=(const Stream& other) {
	_parent = other._parent;
	_priority = other._priority;
	_buffer.str(other._buffer.str());
	return *this;
}

Logger::Stream::~Stream() {
}
