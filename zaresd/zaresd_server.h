// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZARESD_ZARESD_SERVER_H_
#define ZARESD_ZARESD_SERVER_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base2/daemon/base_daemon.h"

namespace net {
class NetEngineManager;
}

class ZAresdServer : public base::BaseDaemon {
public:
  ZAresdServer();
  virtual ~ZAresdServer();

protected:
  // ���� net::BaseDaemon
  virtual int LoadConfig( const FilePath& xml_ini_file );
  virtual int	Initialize( int argc, char** argv );
  virtual int Destroy();

  net::NetEngineManager* net_engine_manager_;
};


#endif
