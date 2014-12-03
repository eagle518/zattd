// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//
#ifndef PROTO_BASE_DATA_H_
#define PROTO_BASE_DATA_H_

#include "base/basictypes.h"

#include <string>
#include <vector>

//////////////////////////////////////////////////////////////////////////
// string���л�ռ�ó���
// #define SIZEOF sizeof
// #define SIZEOF_STRING(s) (sizeof(uint32)+s.length())

//////////////////////////////////////////////////////////////////////////
struct UserInfo {
  uint32 	user_id;
  std::string name;
  std::string nick_name;
  std::string avatar_url;
  std::string title;          // ְ��
  std::string position;       // ��ַ
  uint32 role_status;    // �û���ְ״̬ 0:��ְ 1:��ְ
  uint32 sex;            // 0:Ů 1:��
  uint32 depart_id;      // ����id
  uint32 job_num;        // ����
  std::string telphone;       // �绰
  std::string email;          // ����
  uint32 user_updated;   // ����ʱ��
};

struct FriendInfo {
  uint32 friend_user_id;
  uint32 updated;   // ����ʱ��
};

struct DepartmentInfo {
  uint32    depart_id;          //����id
  std::string title;          // ��������
  std::string description;          // ��������
  uint32    parent_depart_id;   //�ϼ�����id
  uint32    leader;             //����leader id
  uint32    status;         //����״̬  0:���� 1:ɾ��
};

struct UserUnreadMsgCount {
  uint32 from_user_id;
  uint32 unread_msg_count;
} ;

struct ServerMsg {
  uint32 	from_user_id;
  std::string	 	from_name;
  std::string		from_nick_name;
  std::string		from_avatar_url;
  uint32 	create_time;
  uint8	 	msg_type;
  std::string 	msg_data;
};

//////////////////////////////////////////////////////////////////////////
struct GroupInfo {
  uint32 	group_id;
  std::string	group_name;
  std::string	group_avatar;
  uint32	group_creator_id;
  uint32	group_type;		// 1--normal group, 2--temporary group
  uint32	group_updated;
  std::vector<uint32> group_memeber_list;
};

struct GroupUnreadMsgCount {
  uint32 group_id;
  uint32 unread_msg_count;
};

struct GroupMsg {
  uint32 from_user_id;
  uint32 create_time;
  std::string msg_content;
};

//////////////////////////////////////////////////////////////////////////
struct OfflineFile {
  uint32	from_id;
  std::string task_id;
  std::string file_name;
  uint32	file_size;
};

#endif
