// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZARESD_MODEL_ZARES_RELATIONSHIP_MANAGER_IMPL_H_
#define ZARESD_MODEL_ZARES_RELATIONSHIP_MANAGER_IMPL_H_

#include "zaresd/model/relationship_manager.h"

namespace db {
class CdbConnPoolManager;
}

class ZAresRelationshipManagerImpl : public RelationshipManager {
public:
  ZAresRelationshipManagerImpl(db::CdbConnPoolManager* db_conn_pool)
    : db_conn_pool_(db_conn_pool) {}
  virtual ~ZAresRelationshipManagerImpl() {}

  // ��ȡ�û������������ϵ���б�Ĭ�����1000��
  virtual bool GetRecentContactByUserId(uint32 user_id, int limit, std::vector<FriendInfo*>* friends);

  //  ɾ���û���ĳ�������ϵ��
  virtual bool DeleteRecentContactByUserId(uint32 user_id, uint32 friend_user_id);

  // ��ȡ�����û�֮��Ĺ�ϵId,��Ϊ���ѵ�ʱ������������صĺ��ѹ�ϵ,
  // ��С���û�Id��ǰ�ļ�¼��relateId��Ϊ������֮��Ĺ�ϵId,��ѯ�Ի���ʱ����ݹ�ϵId��ѯ
  virtual uint32 GetRelateId(uint32 user_AId, uint32 user_BId);

  bool CheckAndUpdateRelation(uint32 usera_id, uint32 userb_id);
  uint32 AddFriendship(uint32 usera_id, uint32 userb_id);

private:
  db::CdbConnPoolManager* db_conn_pool_;
};

#endif
