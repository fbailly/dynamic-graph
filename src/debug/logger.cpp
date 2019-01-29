/*
 * Copyright 2015, 2019 
 * LAAS-CNRS
 * Andrea Del Prete, François Bailly, Olivier Stasse
 * 
 * This file is part of dynamic-graph.
 * See license file.
 */

#ifndef WIN32
#include <sys/time.h>
#else
#include <Windows.h>
#endif
#define ENABLE_RT_LOG

#include <stdio.h>
#include <iostream>
#include <iomanip>      // std::setprecision
#include <dynamic-graph/logger.h>

#include <dynamic-graph/real-time-logger.h>

namespace dynamicgraph
{

  using namespace std;

  Logger::Logger(double timeSample, double streamPrintPeriod)
    : m_timeSample(timeSample),
      m_streamPrintPeriod(streamPrintPeriod),
      m_printCountdown(0.0)
  {
    m_lv = VERBOSITY_ERROR;
    //    m_output_fstream.open("/tmp/dg-LOGS.txt",std::ofstream::out|std::ofstream::app);
    //dgADD_OSTREAM_TO_RTLOG(m_output_fstream);
  }

  Logger::~Logger()
  {
    //m_output_fstream.close();

  }
  
  void Logger::setVerbosity(LoggerVerbosity lv)
  {
    m_lv=lv;
  }

  LoggerVerbosity Logger::getVerbosity()
  {
    return m_lv;
    
  }
  void Logger::countdown()
  {
    if(m_printCountdown<0.0)
      m_printCountdown = m_streamPrintPeriod;
    m_printCountdown -= m_timeSample;
  }

  void Logger::sendMsg(string msg, MsgType type, const char* file, int line)
  {
    if(m_lv==VERBOSITY_NONE ||
       (m_lv==VERBOSITY_ERROR && !isErrorMsg(type)) ||
       (m_lv==VERBOSITY_WARNING_ERROR && !(isWarningMsg(type) || isErrorMsg(type))) ||
       (m_lv==VERBOSITY_INFO_WARNING_ERROR && isDebugMsg(type)))
      return;

    // if print is allowed by current verbosity level
    if(isStreamMsg(type))
      {
        // check whether counter already exists
        string id = file+toString(line);
        map<string,double>::iterator it = m_stream_msg_counters.find(id);
        if(it == m_stream_msg_counters.end())
	  {
	    // if counter doesn't exist then add one
	    m_stream_msg_counters.insert(make_pair(id, 0.0));
	    it = m_stream_msg_counters.find(id);
	  }

        // if counter is greater than 0 then decrement it and do not print
        if(it->second>0.0)
	  {
	    it->second -= m_timeSample;
	    return;
	  }
        else  // otherwise reset counter and print
          it->second = m_streamPrintPeriod;
      }
    //    std::cout << msg.c_str() << std::endl;
    dgRTLOG() << msg.c_str() << "\n";
    //m_output_fstream.flush();
  }

  bool Logger::setTimeSample(double t)
  {
    if(t<=0.0)
      return false;
    m_timeSample = t;
    return true;
  }

  bool Logger::setStreamPrintPeriod(double s)
  {
    if(s<=0.0)
      return false;
    m_streamPrintPeriod = s;
    return true;
  }
} // namespace dynamicgraph

