// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZARESD_MODEL_ZARES_ZARES_MESSAGE_MANAGER_IMPL_H_
#define ZARESD_MODEL_ZARES_ZARES_MESSAGE_MANAGER_IMPL_H_

#include "zaresd/model/message_manager.h"

namespace db {
  class CdbConnPoolManager;
}

class ZAresRelationshipManagerImpl;
class ZAresMessageManagerImpl : public MessageManager {
public:
  ZAresMessageManagerImpl(db::CdbConnPoolManager* db_conn_pool) :
    db_conn_pool_(db_conn_pool),
    relationship_manager_(NULL) {}
  virtual ~ZAresMessageManagerImpl() {}

  void SetRelationshipManager(ZAresRelationshipManagerImpl* relationship_manager) { relationship_manager_ = relationship_manager; }

  //////////////////////////////////////////////////////////////////////////
  // ��ȡ�������ݣ�ȡδ����Ϣ�õ�,����ֻȡ����ģ����Բ���relateId
  virtual size_t GetMessages(uint32 from_user_id, uint32 to_user_id, uint32 offset, int count, std::vector<ServerMsg*>* messages);

  // ���ݷ����ߺͽ����ߵ��û�Id��ȡ�Ի���Ϣ,˫������
  virtual size_t GetDialogMessages(uint32 from_user_id, uint32 to_user_id, uint32 offset, int count, std::vector<ServerMsg*>* messages);

  //////////////////////////////////////////////////////////////////////////
  // ��ȡȺ��Ϣ
  virtual size_t GetGroupMessages(uint32 group_id, uint32 offset, int count, std::vector<GroupMsg*>* messages);

  // ��ȡȺ��Ϣ,������Ϣ��
  virtual size_t GetGroupMessagesFromId(uint32 user_id, uint32 last_msg_id, int count, std::vector<GroupMsg*>* group_msgs);

  //////////////////////////////////////////////////////////////////////////
  // ����һ��������Ϣ
  virtual bool SendAudioMessage(uint32 from_user_id, uint32 to_user_id, const std::string& content, uint32 time);

  // ����һ��IM��Ϣ,Ŀǰֻ�������¼����ʱ������ԭ��ҵ���е�˽�ţ�at��Ϣ��
  virtual bool SendIMGroupMessage(uint32 userId, uint32 to_group_id, const std::string& content, uint32 time);

  // ����һ��IM��Ϣ,Ŀǰֻ�������¼����ʱ������ԭ��ҵ���е�˽�ţ�at��Ϣ��
  virtual bool SendIMMessage(uint32 from_user_id, uint32 to_user_id, uint32 type, const std::string& content, uint32 time);

private:
  db::CdbConnPoolManager* db_conn_pool_;
  ZAresRelationshipManagerImpl* relationship_manager_;
};

#endif
