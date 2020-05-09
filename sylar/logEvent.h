#pragma once
#include<string>
#include<sstream>
#include<thread>
#include<stdarg.h>

#include <unistd.h>
#include <sys/syscall.h>

#include"log.h"


namespace sylar {
	class logger;
	class logEvent {
	public:
		typedef std::shared_ptr<logEvent> ptr;
		logEvent(std::shared_ptr<logger> &m_logger,logLevel::Level m_level, std::string filename, uint32_t fileLine, time_t elaps_time, time_t local_time, pid_t thread_id, std::string threadName); 
	public:
		//��ȡ��Ϣ
		pid_t getThreadId() { return thread_id; }
		std::string getThreadName() { return thread_name; }

		std::string getFileName() { return m_file; }
		uint32_t getFileLine() { return m_line; }

		uint64_t getTime() { return elaps_time; }
		time_t getLocalTime() { return local_time; }

		std::ostringstream& getMss() { return m_ss; }
		std::string getMessage() { return m_ss.str(); }

		logLevel::Level getLevel() {return eventLevel;}
		std::shared_ptr<logger> getLogger() { return m_loggre; }

		pid_t getId() { return syscall(__NR_gettid); }

		//����event��Ϣ
		void setThreadId() { thread_id = getId(); }
		void setTime() { elaps_time = time(NULL); }
		void setLocalTime() {	local_time = time(NULL);}
		void setMss(char* fmt,...) {
			char *buff;
			va_list   arg_ptr;   //����ɱ����ָ�� 
			va_start(arg_ptr, fmt);   // iΪ���һ���̶�����
			vasprintf(&buff, fmt, arg_ptr);
			va_end(arg_ptr);        //  ��ղ���ָ��
			m_ss << buff;
			free(buff);
		}
		void setMss(std::string msg) {
			m_ss << msg;
		}
		void setLogLevel(logLevel::Level& l) { eventLevel = l; }
	private:
		pid_t				thread_id;		//event �����߳�
		std::string			thread_name;	//�߳���
		std::string			m_file;			//�ļ���
		uint32_t			m_line;			//�к�

		time_t				elaps_time;		//event ����ʱ��
		time_t				local_time;     //event ʱ���

		std::ostringstream  m_ss;			//event ��ӡ��ϢE
		logLevel::Level		eventLevel;		//�¼��ȼ�

		std::shared_ptr<logger> m_loggre;	//��־������
	};
}