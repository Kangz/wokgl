#ifndef COMMON_MAINLOGGER_HPP
#define COMMON_MAINLOGGER_HPP

#include "Singleton.hpp"
#include <fstream>

class MainLogger : public Singleton<MainLogger> {
	public:
		MainLogger();
		static void create(int flags, const std::string& file = "");
		static void destroy();

		enum {
			CONSOLE = 1,
			ERRORCONSOLE = 2,
			LOGFILE = 4
		};

		void write(const std::string& message);
	
	private:
		MainLogger(int flags, const std::string& file);
		~MainLogger();

		std::ofstream _output;
		int _flags;
};

#endif

