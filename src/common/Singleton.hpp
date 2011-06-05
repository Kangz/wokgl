#ifndef COMMON_SINGLETON_HPP
#define COMMON_SINGLETON_HPP

template<typename T>
class Singleton {
	public:
		static T& getInstance() {
			if(!instance) {
				instance = new T;
			}
			return *instance;
		}

		static T* getInstancePtr() {
			if(!instance) {
				instance = new T;
			}
			return instance;
		}

	protected:
		static T* instance;
		Singleton() {};
		~Singleton() {};
		T& operator=(T& other) {
			return other;
		}
};

template<typename T>
T* Singleton<T>::instance = 0;

#endif

