// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZARESD_MODEL_USER_MANAGER_H_
#define ZARESD_MODEL_USER_MANAGER_H_

#include "proto/base_teamtalk_pdu.h"

class UserManager {
public:
  virtual ~UserManager() {}

  // ��ȡһ���û�����Ϣ
  // ���룺
  //  user_id: �û�ID
  //  user_info: �û��ľ�����Ϣ��ÿ��Ԫ�ذ����û�id���û�����ͷ������
  // ����ֵ��
  //  ����û����ڣ���������user_infoֵ��������user_info
  //  ����û������ڻ��ڲ����󣬷���NULL
  virtual const UserInfo* GetUserInfo(const uint32 user_id, UserInfo* user_info) = 0;

  // ��ȡһ���û�����Ϣ
  virtual bool GetUserInfo(const std::vector<uint32> user_ids, std::vector<UserInfo*>* user_infos) = 0;

  // ��ȡһ���û�����Ϣ
  // ���룺
  //  user_name: �û���
  //  user_info: �û��ľ�����Ϣ��ÿ��Ԫ�ذ����û�id���û�����ͷ������
  // ����ֵ��
  //  ����û����ڣ���������user_infoֵ��������user_info
  //  ����û������ڻ��ڲ����󣬷���NULL
  virtual const UserInfo* GetUserInfo(const std::string& user_name, UserInfo* user_info) = 0;

  // ��ȡһ���û�����Ϣ
  virtual bool GetUserInfo(const std::vector<std::string>& user_names, std::vector<UserInfo*>* user_infos) = 0;

  // ��ȡ�����û�����Ϣ;Ĭ�������û�����5000��
  virtual bool GetAllUserInfo(std::vector<UserInfo*>* user_infos) = 0;

};

#endif
