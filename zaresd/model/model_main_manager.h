// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZARESD_MODEL_MODEL_MAIN_MANAGER_H_
#define ZARESD_MODEL_MODEL_MAIN_MANAGER_H_

#include "base/memory/singleton.h"

#include "zaresd/model/model_module.h"

// ���ݴ洢�п��ܲ�����ֱ�ӷ���mysql���������������ʹ��Atlas(360)���������Բ������������ݿ�(MogoDB��)��
// ��model�ṩһЩ�ӿڣ�
// �����Բ����ʽʵ�ֲ�ͬ�����ݷ���
class ModelMainManager {
public:
  ~ModelMainManager() {
    Destroy();
  }

  static ModelMainManager* GetInstance() {
    return Singleton<ModelMainManager>::get();
  }

  bool Initialize(const std::string model_type);
  void Destroy();

  LoginManager* GetLoginManager();
  DepartmentManager* GetDepartmentManager();
  UserManager* GetUserManager();
  GroupManager* GetGroupManager();
  RelationshipManager* GetRelationshipManager();
  MessageManager* GetMessageManager();
  FileManager* GetFileManager();
  CounterManager* GetCounterManager();
  AudioManager* GetAudioManager();

private:
  friend struct DefaultSingletonTraits<ModelMainManager>;

  ModelMainManager() {
    module_ = NULL;
  }

protected:
  ModelModule* module_;
};

#endif

