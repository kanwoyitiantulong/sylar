#include "logFormat.h"
#include <vector>
#include <iostream>
namespace sylar {
	logFormat::logFormat(std::shared_ptr<logEvent> event, std::string m_fmt):m_event(event),fmt(m_fmt) {
		vec.clear();
		fraseFmt();
	}
	/**
	* @brief ���캯��
	* @param[in] pattern ��ʽģ��
	* @details
	*  %m ��Ϣ
	*  %p ��־����
	*  %r �ۼƺ�����
	*  %c ��־����
	*  %t �߳�id
	*  %n ����
	*  %d ʱ��
	*  %f �ļ���
	*  %l �к�
	*  %T �Ʊ��
	*  %F Э��id
	*  %N �߳�����
	*
	*  Ĭ�ϸ�ʽ "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
	*/
	std::map<std::string, std::function<logFormat::formatItem::ptr(std::string s)>> logFormat::m_mp = {
#define XX(str,C)  {#str,[](std::string s){return logFormat::formatItem::ptr(new C(s));}}

		XX(m,messageItem),		//m		��־��Ϣ
		XX(p,evemtLevelItem),	//p		��־����
		XX(r,elapseTimeItem),	//n
		XX(c,loggerNameItem),	//ltm
		XX(t,threadIdItem),		//tid
		XX(n,enterItem),		//ctm
		XX(d,nowTimeItem),		//l
		XX(f,fileNameItem),		//t
		XX(l,lineNumItem),		//endl
		XX(T,tabItem),			//l
		XX(F,corThreadId),		//t
		XX(N,threadNameItem),	//endl

#undef XX
	};

	void logFormat::setFmt(std::string m_fmt) {
		fmt = m_fmt;
	}
	std::string logFormat::getFmt() { return fmt; }

	std::string logFormat::getMsg(std::shared_ptr<logger>m_logger, logLevel::Level le, std::shared_ptr<logEvent> event) {
		if (!m_logger->getFormat().empty() && m_logger->getFormat() != fmt) {
			vec.clear();
			setFmt(m_logger->getFormat());
			fraseFmt();
		}
		for (auto temp : vec) {
			//std::cout << temp.first << std::endl;
			if (m_mp.find(temp.first)!= m_mp.end()) {
				m_mp[temp.first](temp.second)->log(m_ostringstream,m_logger,le,event);
			}
			else {
				stringItem(temp.first).log(m_ostringstream, m_logger, le, event);
			}
		}
		return m_ostringstream.str();
	}
	void logFormat::fraseFmt() {
		//����fmt
		//std::cout << "enter fraseFmt" << std::endl;
		std::string m_str;
			for (int i = 0;i < fmt.size();i++) {
				if (fmt[i] != '%') {
					m_str.push_back(fmt[i]);
					continue;
				}
				if ((i + 1)< fmt.size() && fmt[i + 1] == '%') {
					m_str.push_back(fmt[++i]);
					continue;
				}
				int fmt_start = i + 1;
				int fmt_end = fmt_start;
				std::string m_fmt;
				std::string m_fmt_fmt;
				int flag = 0;
				while (fmt_end < fmt.size()) {
					//%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n
					if (isalpha(fmt[fmt_end]) && fmt[fmt_end] != '{' && fmt[fmt_end] != '}' && flag == 0) {
						m_fmt.push_back(fmt[fmt_end]);
						fmt_end++;
						continue;
					}
					if (fmt[fmt_end] == '{') {
						flag = 1;
						fmt_end++;
						continue;
					}
					if (fmt[fmt_end] == '}') {
						flag = 0;
						fmt_end;
						i = fmt_end;
						break;
					}
					if (flag == 1) {
						m_fmt_fmt.push_back(fmt[fmt_end]);
						fmt_end++;
						continue;
					}
					if (flag == 0) {
						i = fmt_end-1;
						break;
					}
				}
				//std::cout << __FILE__ <<" "<< __LINE__ <<" "<< m_str << " " << m_fmt << " " << m_fmt_fmt <<"||||"<< std::endl;
				if(!m_str.empty())vec.push_back(std::make_pair(m_str,std::string( "")));
				vec.push_back(std::make_pair(m_fmt,m_fmt_fmt));
				m_str.clear();
			}
	}
}