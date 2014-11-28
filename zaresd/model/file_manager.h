// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZARESD_MODEL_FILE_MANAGER_H_
#define ZARESD_MODEL_FILE_MANAGER_H_

#include "proto/base_teamtalk_pdu.h"

// #include <string>
// #include <vector>
// #include "base/basictypes.h"
// 
struct TransmitFile {
  uint32 id; 
  uint32 from_user_id; // ������
  uint32 to_user_id; // ������
  std::string task_id; //������
  std::string file_path; // �ļ�·��
  uint32 file_size;		//�ļ���С.
  uint32 status; // ״̬���Ƿ��Ѿ�������
  uint32 created; // ����ʱ��
  uint32 updated; // ����ʱ��
};

class FileManager {
public:
  virtual ~FileManager() {}

  // ��ȡһ���û�δ���յ������ļ��б�
  virtual size_t GetUserFiles(uint32 to_user_id, std::vector<OfflineFile*>* files) = 0;

  // ����һ��δ���յ������ļ�
  virtual bool SaveFileRecord(uint32 user_id, uint32 to_user_id, const std::string& task_id, const std::string& file_path, uint32 fileSize) = 0;
 
  // ����һ���ļ��Ѿ����Է�����
  virtual bool DeleteFileRecord(const std::string& task_id) = 0;
  
  // ��ȡһ�����͵��ļ��ļ�¼
  virtual const TransmitFile* GetFileRecord(const std::string& task_id, TransmitFile* file) = 0;

};

#endif
