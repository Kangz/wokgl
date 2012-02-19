#ifndef UTILS_LOGGER_LOGGER_HPP
#define UTILS_LOGGER_LOGGER_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

class Logger {
	public:
		class Stream;

		static Logger* create(int minPriority, const std::string& name);

		static void destroy(Logger*& logger);

		enum {
			DEBUG = 0,
			INFO = 1,
			WARN = 2,
			ERR = 3,
			CRIT = 4,
			FATAL = 5
		};

		void write(const std::string& message, int priority = DEBUG);

		inline Stream& debug()	{return *(_streams[DEBUG]);}
		inline Stream& info()	 {return *(_streams[INFO]);}
		inline Stream& warning()  {return *(_streams[WARN]);}
		inline Stream& error()	{return *(_streams[ERR]);}
		inline Stream& critical() {return *(_streams[CRIT]);}
		inline Stream& fatal()	{return *(_streams[FATAL]);}

	protected:
		Logger(int minPriority, const std::string& name);
		virtual ~Logger();
		Logger(Logger& other){}
		Logger& operator=(Logger& other) {
			return other;
		}


		std::vector<Stream*> _streams;
		int _minPriority;
		std::string _name;

	public:
		struct endlStruct{};
		static endlStruct endl;
		class Stream {
			public:
				Stream(Logger* parent, int priority);
				Stream(const Stream& other);
				Stream& operator=(const Stream& other);
				~Stream();
				template<typename T> Stream& operator<<(const T& f) {
					_buffer << f;
					return *this;
				}

				Stream& operator<<(const Logger::endlStruct& f) {
					_parent->write(_buffer.str(), _priority);
					_buffer.str("");
					return *this;
				}
			protected:
				Logger* _parent;
				std::ostringstream _buffer;
				int _priority;
		};
};

#endif

