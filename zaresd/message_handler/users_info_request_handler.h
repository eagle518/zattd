// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//
// �˴���Ϊ�Զ����ɣ��벻Ҫ����Ķ�
//

#ifndef ZARESD_MESSAGE_HANDLER_USERS_INFO_REQUEST_HANDLER_H_
#define ZARESD_MESSAGE_HANDLER_USERS_INFO_REQUEST_HANDLER_H_

#include "message_pdu/message_pdu.h"

class ZAresHandlerThread;
class UsersInfoRequestHandler {
public:
  static int Execute(ZAresHandlerThread* context, uint64 session_id, const message::MessagePDU* message);

private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(UsersInfoRequestHandler);
};

#endif