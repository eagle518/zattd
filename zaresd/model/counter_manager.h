// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZARESD_MODEL_COUNTER_MANAGER_H_
#define ZARESD_MODEL_COUNTER_MANAGER_H_

#include "base/basictypes.h"

#include <vector>
#include <map>

class Counter {
public:
  inline uint32 user_id() const { return user_id_; }
  inline void set_user_id(uint32 val) { user_id_ = val; }

  inline const std::map<uint32, uint32>& unread_count() const { return unread_count_; }
  inline const std::map<uint32, uint32>& msg_count() const { return msg_count_; }

  inline std::map<uint32, uint32>* mutable_unread_count() { return &unread_count_; }
  inline std::map<uint32, uint32>* mutable_msg_count() { return &msg_count_; }

private:
  uint32 user_id_;
  std::map<uint32, uint32> unread_count_;
  std::map<uint32, uint32> msg_count_;
};

struct GroupCounterItem {
  uint32 user_id;
  uint32 group_id;
  uint32 group_total_count; // Ⱥ����Ϣ��
  uint32 user_unread_count; // �û��ڸ�Ⱥ�е�δ����Ϣ��
  uint32 last_message_id;
};

class CounterManager {
public:
  virtual ~CounterManager() {}

  // ����һ����Ϣ��ͬʱ�������ͺͽ����û�����Ϣ������
  // virtual void IncrUserMsgCount(uint32 from_user_id, uint32 to_user_id) = 0;

  // ����һ����Ϣ��ͬʱ�������ͺͽ����û�����Ϣ������
  // virtual void IncreaseUserUnreadMsgCount(uint32 from_user_id, uint32 to_user_id);

  // ����û����е�δ����Ϣ
  virtual const Counter* GetUnreadMsgCount(uint32 user_id, uint32 client_type, Counter* unread_msg_count) = 0;

  // ����û�����ĳ���ѵ�δ����Ϣ����
  virtual bool ClearUserUnreadItemCount(uint32 user_id, uint32 friend_user_id, uint32 client_type) = 0;

  // ����û���������ĳ���ѵ�δ����Ϣ
  virtual size_t GetUserFriendUnreadCount(uint32 user_id, uint32 friend_user_id, uint32 client_type) = 0;

  // ����ĳ�û���ĳ��Ⱥ���δ����Ϣ����
  virtual size_t GetUserGroupUnreadCount(uint32 user_id, const std::vector<uint32>& group_ids, uint32 client_type, std::map<uint32, uint32>* unreads) = 0;

  // �����û�ID����Ի����û�IDɾ���û�(ǰ��)�������Ѷ��Ի���Ϣ,��������
  virtual bool DeleteUserReadedDialogMessages(uint32 user_id, uint32 friend_user_id, uint32 client_type) = 0;

  //////////////////////////////////////////////////////////////////////////
  // ����ĳ�û���ĳȺ���δ����Ϣ��
  virtual const GroupCounterItem* GetUserGroupCount(uint32 user_id, uint32 group_id, uint32 client_type, GroupCounterItem* item) = 0;

  // ����û���Ӧ��ĳ��Ⱥ��ļ���
  virtual bool ClearUserGroupCounter(uint32 user_id, uint32 group_id, uint32 client_type) = 0;

};

#endif
