// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZARESD_MODEL_ZARES_ZARES_FILE_MANAGER_IMPL_H_
#define ZARESD_MODEL_ZARES_ZARES_FILE_MANAGER_IMPL_H_

#include "zaresd/model/file_manager.h"

namespace db {
class CdbConnPoolManager;
}

class ZAresFileManagerImpl : public FileManager {
public:
  ZAresFileManagerImpl(db::CdbConnPoolManager* db_conn_pool) :
    db_conn_pool_(db_conn_pool) {}
  virtual ~ZAresFileManagerImpl() {}

  // ��ȡһ���û�δ���յ������ļ��б�
  virtual size_t GetUserFiles(uint32 to_user_id, std::vector<OfflineFile*>* files);

  // ����һ��δ���յ������ļ�
  virtual bool SaveFileRecord(uint32 user_id, uint32 to_user_id, const std::string& task_id, const std::string& file_path, uint32 fileSize);

  // ����һ���ļ��Ѿ����Է�����
  virtual bool DeleteFileRecord(const std::string& task_id);

  // ��ȡһ�����͵��ļ��ļ�¼
  virtual const TransmitFile* GetFileRecord(const std::string& task_id, TransmitFile* file);

private:
  db::CdbConnPoolManager* db_conn_pool_;

};

#endif
