// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZARESD_MODEL_ZARES_COUNTER_MANAGER_IMPL_H_
#define ZARESD_MODEL_ZARES_COUNTER_MANAGER_IMPL_H_

#include "zaresd/model/counter_manager.h"

class ZAresCounterManagerImpl : public CounterManager {
public:
  ZAresCounterManagerImpl() {}
  virtual ~ZAresCounterManagerImpl() {}

  // ����û����е�δ����Ϣ
  virtual bool GetUnreadMsgCount(uint32 user_id, Counter* unread_msg_count);

  // ����û�����ĳ���ѵ�δ����Ϣ����
  virtual bool ClearUserUnreadItemCount(uint32 user_id, uint32 friend_user_id);

  // ����û���������ĳ���ѵ�δ����Ϣ
  virtual size_t GetUserFriendUnreadCount(uint32 user_id, uint32 friend_user_id);

  // ����ĳ�û���ĳ��Ⱥ���δ����Ϣ����
  virtual size_t GetUserGroupUnreadCount(uint32 user_id, const std::vector<uint32>& group_ids, std::map<uint32, uint32>* unreads);

  virtual bool DeleteUserReadedDialogMessages(uint32 user_id, uint32 friend_user_id);

  //////////////////////////////////////////////////////////////////////////
  // ����ĳ�û���ĳȺ���δ����Ϣ��
  virtual bool GetUserGroupCount(uint32 user_id, uint32 group_id, GroupCounterItem* item);

  // ����û���Ӧ��ĳ��Ⱥ��ļ���
  virtual bool ClearUserGroupCounter(uint32 user_id, uint32 group_id);
};

#endif
