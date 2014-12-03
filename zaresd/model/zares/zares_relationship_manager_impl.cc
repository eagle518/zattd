// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zaresd/model/zares/zares_relationship_manager_impl.h"

#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/stringprintf.h"
#include "base2/time2.h"
#include "base/string_number_conversions.h"

#include "db/database_util.h"
#include "db/conn_pool_manager.h"

#include "zaresd/const_zaresd_defines.h"

namespace {

// .1..4
// "SELECT relateid,frienduserid,status,created,updated FROM imrecentcontact WHERE userid = %d AND status = 0 ORDER BY updated desc, relateId DESC LIMIT 100";
bool ParseFromDatabase(FriendInfo* friend_info, const db::QueryAnswer& answ) {
  enum {
    kColumn_RelateID = 0,
    kColumn_FriendUserID,
    kColumn_Status,
    kColumn_Created,
    kColumn_Updated,
  };

  bool result  = true;
  do {
    DB_GET_RETURN_COLUMN(kColumn_FriendUserID, friend_info->friend_user_id);
    DB_GET_RETURN_COLUMN(kColumn_Updated, friend_info->updated);
  } while (0);

  return result;
}

}
bool ZAresRelationshipManagerImpl::GetRecentContactByUserId(uint32 user_id, int limit, std::vector<FriendInfo*>* friends) {
  if (friends == NULL || user_id == 0) {
    LOG(ERROR) << "friends is NULL or user_id is 0!!!!";
    return false;
  }

  db::ScopedPtr_DatabaseConnection db_conn(db_conn_pool_);

  std::string sql = StringPrintf(
      "SELECT relateId,friendUserId,status,created,updated FROM IMRecentContact WHERE userId = %d AND status = 0 ORDER BY updated DESC, relateId DESC LIMIT 100",
      user_id
    );

  scoped_ptr<db::QueryAnswer> answ(db_conn->Query(sql));
  if (answ.get() != NULL) {
    while (answ->FetchRow()) {
      FriendInfo* friend_info = new FriendInfo();
      CHECK(friend_info);
      ParseFromDatabase(friend_info, *answ);
      friends->push_back(friend_info);
    }
  } else {
    return false;
  }

  return true;
}

bool ZAresRelationshipManagerImpl::DeleteRecentContactByUserId(uint32 user_id, uint32 friend_user_id) {
  // db::BaseDatabase* db_conn = context->GetDatabaseConncetion();
  std::string sql;
  base::SStringPrintf(&sql, "UPDATE IMRecentContact SET status = 1 WHERE userId = %d AND friendUserId = %d LIMIT 1",
      user_id,friend_user_id
    );

  db::ScopedPtr_DatabaseConnection db_conn(db_conn_pool_);
  return 0 == db_conn->Execute(sql);
}

uint32 ZAresRelationshipManagerImpl::GetRelateId(uint32 user_AId, uint32 user_BId) {
  uint32 relate_id = kInvalidID;

  if (user_AId == 0 || user_BId == 0) {
    LOG(ERROR) << "user_AId or user_BId is 0";
    return relate_id;
  }

  uint32 small_user_id;
  uint32 big_user_id;
  // ȡ�û�IdС����ǰ��relateId��Ϊ�����˵�relateId
  if (user_AId > user_BId) {
    small_user_id = user_BId;
    big_user_id = user_AId;
  } else {
    small_user_id = user_AId;
    big_user_id = user_BId;
  }

  db::ScopedPtr_DatabaseConnection db_conn(db_conn_pool_);
  std::string sql;
  base::SStringPrintf(&sql, "SELECT relateId FROM IMRecentContact WHERE userId = %i and friendUserId = %i limit 1", small_user_id, big_user_id);
  scoped_ptr<db::QueryAnswer> answ(db_conn->Query(sql));
  if (answ.get() != NULL) {
    if (answ->FetchRow()) {
      answ->GetColumn(0, &relate_id);
    }
  } else {
    LOG(ERROR) << "";
  }

  return relate_id;
}

bool ZAresRelationshipManagerImpl::CheckAndUpdateRelation(uint32 usera_id, uint32 userb_id) {
  if (usera_id == 0 ||userb_id == 0) {
      return true;
  }

  db::ScopedPtr_DatabaseConnection db_conn(db_conn_pool_);

  uint32 relate_id = 0;
  std::string relate_ids;

  db::QueryParam p;
  p.AddParam(&usera_id);
  p.AddParam(&userb_id);
  p.AddParam(&usera_id);
  p.AddParam(&userb_id);

  std::string sql;
  db::MakeQueryString("SELECT relateId,status FROM IMRecentContact WHERE (userId=:1 AND friendUserId=:2) OR (userId=:3 AND friendUserId=:4) LIMIT 2", &p, &sql);
  scoped_ptr<db::QueryAnswer> answ(db_conn->Query(sql));
  if (answ.get() != NULL) {
    while (answ->FetchRow()) {
      uint32 status = 0;
      answ->GetColumn(0, &relate_id);
      answ->GetColumn(1, &status);
      if (status != 0) {
        if (!relate_ids.empty()) {
          relate_ids.push_back(',');
        }
        relate_ids.append(base::UintToString(relate_id));
      }
    }
  } else {
    LOG(ERROR) << "";
  }

  bool is_sucess = false;
  if (!relate_ids.empty()) {
    sql = StringPrintf("UPDATE IMRecentContact SET status = 0 WHERE relateId in (%s) LIMIT 4", relate_ids.c_str());
    is_sucess = ( 0 == db_conn->Execute(sql));
  }

  return is_sucess;
}

// ��Ϊ���ѣ������ϵ��һ�β�����
uint32 ZAresRelationshipManagerImpl::AddFriendship(uint32 usera_id, uint32 userb_id) {
  if (usera_id == 0 || userb_id == 0) {
      return kInvalidID;
  }

  // �Ѵ��ڹ�ϵ
  uint32 relate_id = GetRelateId(usera_id, userb_id);
  if (relate_id != kInvalidID) {
      return relate_id;
  }

  uint32 mid;
  // ȡ�û�IdС����ǰ��relateId��Ϊ�����˵�relateId,��Ƚ�����ʾ������֤�����ϵId����һ����¼С
  if (usera_id > userb_id) {
      mid = usera_id;
      usera_id = userb_id;
      userb_id = mid;
  }

  uint32 ctime = base::NowMSTime()/1000;
  int default_status = 0;
  uint32 id = kInvalidID;

  db::QueryParam p;
  std::string sql;
  if (usera_id != userb_id) {
    p.AddParam(&usera_id);
    p.AddParam(&userb_id);
    p.AddParam(&default_status);
    p.AddParam(&ctime);
    p.AddParam(&ctime);
    p.AddParam(&userb_id);
    p.AddParam(&usera_id);
    p.AddParam(&default_status);
    p.AddParam(&ctime);
    p.AddParam(&ctime);
    MakeQueryString("INSERT INTO IMRecentContact(`userId`,`friendUserId`,`status`,`created`,`updated`) VALUES(:1, :2, :3, :4, :5),(:6, :7, :8, :9, :10)", &p, &sql);
  } else {
    p.AddParam(&usera_id);
    p.AddParam(&userb_id);
    p.AddParam(&default_status);
    p.AddParam(&ctime);
    p.AddParam(&ctime);
    MakeQueryString("INSERT INTO IMRecentContact(`userId`,`friendUserId`,`status`,`created`,`updated`) VALUES(:1, :2, :3, :4, :5)", &p, &sql);
  }

  db::ScopedPtr_DatabaseConnection db_conn(db_conn_pool_);
  return static_cast<uint32>(db_conn->ExecuteInsertID(sql));
}


#if 0
package com.mogujie.ares.model;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import com.mogujie.ares.data.Relationship;
import com.mogujie.ares.manager.DBManager;
import com.mogujie.ares.manager.DBManager.DBPoolName;
import com.mogujie.ares.util.MoguUtil;

/*
 * 
 * @Description: ���ѹ�ϵ��صĲ���model
 * @author ziye - ziye[at]mogujie.com
 * @date 2013-7-22 ����2:35:34
 *
 */
public class RelationshipModel {

    private static RelationshipModel instance = new RelationshipModel();

    public static RelationshipModel getInstance() {
        if (instance == null) {
            instance = new RelationshipModel();
        }
        return instance;
    }

    /*
     * @Description: ����ӿ���Ĭ�ϲ���master��
     * 
     * @param userAId
     * 
     * @param userBId
     * 
     * @return
     * 
     * @throws SQLException
     */
    public int getRelateId(int userAId, int userBId) throws SQLException {
        return getRelateId(userAId, userBId, false);
    }

    /*
     * @Description: ��ȡ�����û�֮��Ĺ�ϵId,��Ϊ���ѵ�ʱ������������صĺ��ѹ�ϵ,
     * ��С���û�Id��ǰ�ļ�¼��relateId��Ϊ������֮��Ĺ�ϵId,��ѯ�Ի���ʱ����ݹ�ϵId��ѯ
     * 
     * @param userAId
     * 
     * @param userBId
     * 
     * @param isFromMaster ��ӹ�ϵ֮ǰ��checkҪ��master
     * 
     * @return
     * 
     * @throws SQLException
     */
    public int getRelateId(int userAId, int userBId, boolean isFromMaster)
            throws SQLException {

        int relateId = -1;

        if (userAId <= 0 || userBId <= 0) {
            return relateId;
        }
        int smallUserId;
        int bigUserId;
        // ȡ�û�IdС����ǰ��relateId��Ϊ�����˵�relateId
        if (userAId > userBId) {
            smallUserId = userBId;
            bigUserId = userAId;
        } else {
            smallUserId = userAId;
            bigUserId = userBId;
        }

        DBManager dbManager = DBManager.getInstance();
        DBPoolName dbPoolName = isFromMaster ? DBPoolName.macim_master
                : DBPoolName.macim_slave;
        Connection conn = dbManager.getConnection(dbPoolName);
        PreparedStatement statement = null;
        ResultSet rs = null;
        try {
            String sql = "select relateId from IMRecentContact where userId = ? "
                    + "and friendUserId = ? limit 1";
            statement = conn.prepareStatement(sql);
            int index = 1;
            statement.setObject(index++, smallUserId);
            statement.setObject(index++, bigUserId);
            rs = statement.executeQuery();

            if (rs.next()) {
                relateId = rs.getInt("relateId");
            }
        } catch (SQLException e) {
            throw e;
        } finally {
            dbManager.release(dbPoolName, conn, statement, rs);
        }

        return relateId;
    }

    /*
     * @Description: ��ȡ�û��ǵĹ�ϵId
     * 
     * @param userAId
     * 
     * @param userBIds
     * 
     * @return
     * 
     * @throws SQLException
     */
    public int[] getRelateIds(int userAId, int[] userBIds) throws SQLException {

        if (userAId <= 0 || userBIds.length <= 0) {
            return new int[0];
        }
        List<Integer> smallUserIds = new ArrayList<Integer>();
        List<Integer> bigUserIds = new ArrayList<Integer>();
        String smallWhereClause = "";
        String bigWhereClause = "";
        for (int i = 0; i < userBIds.length; i++) { // �ֿ������顣����
            if (userBIds[i] <= 0)
                continue;
            // ȡ�û�IdС����ǰ��relateId��Ϊ�����˵�relateId
            if (userAId > userBIds[i]) {
                smallUserIds.add(userBIds[i]);
                smallWhereClause += ",?";
            } else {
                bigUserIds.add(userBIds[i]);
                bigWhereClause += ",?";
            }
        }

        DBManager dbManager = DBManager.getInstance();
        Connection conn = dbManager.getConnection(DBPoolName.macim_slave);
        PreparedStatement statement = null;
        ResultSet rs = null;
        List<Integer> relateIdList = new ArrayList<Integer>();
        try {
            int smallLength = smallUserIds.size();
            if (smallLength > 0) { // �û�Id�ȵ�ǰ�û�IdС��һ��
                String sql = "select relateId from IMRecentContact where userId in ("
                        + smallWhereClause.substring(1)
                        + ") and friendUserId = ?";
                statement = conn.prepareStatement(sql);
                int index = 1;
                for (int i = 0; i < smallLength; i++) {
                    statement.setObject(index++, smallUserIds.get(i));
                }
                statement.setObject(index++, userAId);
                rs = statement.executeQuery();
                while (rs.next()) {
                    relateIdList.add(rs.getInt("relateId"));
                }
                rs.close();
            }
            int bigLength = bigUserIds.size();
            if (bigLength > 0) { // �û�Id�ȵ�ǰ�û�Id���һ��
                String sql = "select relateId from IMRecentContact where userId = ? "
                        + " and friendUserId in ("
                        + bigWhereClause.substring(1) + ")";
                statement = conn.prepareStatement(sql);
                int index = 1;
                statement.setObject(index++, userAId);
                for (int i = 0; i < bigLength; i++) {
                    statement.setObject(index++, bigUserIds.get(i));
                }
                rs = statement.executeQuery();
                while (rs.next()) {
                    relateIdList.add(rs.getInt("relateId"));
                }
                rs.close();
            }
        } catch (SQLException e) {
            throw e;
        } finally {
            dbManager.release(DBPoolName.macim_slave, conn, statement, rs);
        }
        int size = relateIdList.size();
        int[] relateIds = new int[size];
        for (int i = 0; i < size; i++) {
            relateIds[i] = relateIdList.get(i);
        }

        return relateIds;
    }

    /*
     * 
     * @Description: ��ȡ�û������������ϵ���б�Ĭ�����1000��
     * 
     * @param userId �û�Id
     * 
     * @return
     * 
     * @throws SQLException
     */
    public Relationship[] getRecentContactByUserId(int userId, int limit)
            throws SQLException {
        List<Relationship> relateList = new ArrayList<Relationship>();
        Relationship[] relations;
        if (userId <= 0 || limit <= 0) {
            relations = new Relationship[relateList.size()];
            relateList.toArray(relations);
            return relations;
        }

        DBManager dbManager = DBManager.getInstance();
        Connection conn = dbManager.getConnection(DBPoolName.macim_slave);
        PreparedStatement statement = null;
        ResultSet rs = null;
        try {
            String sql = "select * from IMRecentContact where userId = ? and status = 0 order by updated desc, relateId desc limit "
                    + limit;
            statement = conn.prepareStatement(sql);
            statement.setObject(1, userId);
            rs = statement.executeQuery();

            Relationship relate;
            while (rs.next()) {
                relate = new Relationship();
                relate.setRelateId(rs.getInt("relateId"));
                relate.setUserId(userId);
                relate.setFriendUserId(rs.getInt("friendUserId"));
                relate.setStatus(rs.getInt("status"));
                relate.setCreated(rs.getInt("created"));
                relate.setUpdated(rs.getInt("updated"));
                relateList.add(relate);
            }
        } catch (SQLException e) {
            throw e;
        } finally {
            dbManager.release(DBPoolName.macim_slave, conn, statement, rs);
        }

        relations = new Relationship[relateList.size()];
        relateList.toArray(relations);
        return relations;
    }

    /*
     * @Description: ɾ���û���ĳ�������ϵ��
     * 
     * @param userId �û�Id
     * 
     * @param friendUserId �����û�Id
     * 
     * @return
     * 
     * @throws SQLException
     */
    public Boolean deleteRecentContactByUserId(int userId, int friendUserId)
            throws SQLException {

        if (userId <= 0 || friendUserId <= 0) {
            return false;
        }

        DBManager dbManager = DBManager.getInstance();
        Connection conn = dbManager.getConnection(DBPoolName.macim_slave);
        PreparedStatement statement = null;
        ResultSet rs = null;
        int deletCount = 0;
        try {
            String sql = "update IMRecentContact set status = 1 where userId = ? and friendUserId = ? limit 1";
            statement = conn.prepareStatement(sql);
            statement.setInt(1, userId);
            statement.setInt(2, friendUserId);
            deletCount = statement.executeUpdate();

        } catch (SQLException e) {
            throw e;
        } finally {
            dbManager.release(DBPoolName.macim_slave, conn, statement, rs);
        }
        if (deletCount >= 0) { // �ɹ�
            return true;
        }
        return false;
    }

    /*
     * @Description: ��Ϊ���ѣ������ϵ��һ�β�����
     * 
     * @param userAId
     * 
     * @param userBId
     * 
     * @return
     * 
     * @throws SQLException
     */
    public int addFriendship(int userAId, int userBId) throws SQLException {

        if (userAId <= 0 || userBId <= 0) {
            return -1;
        }
        // �Ѵ��ڹ�ϵ
        int relateId = getRelateId(userAId, userBId);
        if (relateId > 0) {
            return relateId;
        }

        int mid;
        // ȡ�û�IdС����ǰ��relateId��Ϊ�����˵�relateId,��Ƚ�����ʾ������֤�����ϵId����һ����¼С
        if (userAId > userBId) {
            mid = userAId;
            userAId = userBId;
            userBId = mid;
        }

        DBManager dbManager = DBManager.getInstance();
        Connection conn = dbManager.getConnection(DBPoolName.macim_master);
        PreparedStatement statement = null;
        ResultSet rs = null;
        int ctime = (int) (new Date().getTime() / 1000);
        int defaultStatus = 0;
        int id = -1;
        try {
            // һ�β���2����¼
            if (userAId != userBId) {
                String sql = "insert into IMRecentContact(`userId`, `friendUserId`, `status`, `created`, `updated`) "
                        + "values(?, ?, ?, ?, ?),(?, ?, ?, ?, ?)";
                int index = 1;
                statement = conn.prepareStatement(sql,
                        Statement.RETURN_GENERATED_KEYS);
                statement.setObject(index++, userAId);
                statement.setObject(index++, userBId);
                statement.setObject(index++, defaultStatus);
                statement.setObject(index++, ctime);
                statement.setObject(index++, ctime);
                statement.setObject(index++, userBId);
                statement.setObject(index++, userAId);
                statement.setObject(index++, defaultStatus);
                statement.setObject(index++, ctime);
                statement.setObject(index++, ctime);
            } else { // �Լ����Լ�
                String sql = "insert into IMRecentContact(`userId`, `friendUserId`, `status`, `created`, `updated`) "
                        + "values(?, ?, ?, ?, ?)";
                int index = 1;
                statement = conn.prepareStatement(sql,
                        Statement.RETURN_GENERATED_KEYS);
                statement.setObject(index++, userAId);
                statement.setObject(index++, userBId);
                statement.setObject(index++, defaultStatus);
                statement.setObject(index++, ctime);
                statement.setObject(index++, ctime);
            }
            // ����
            int count = statement.executeUpdate();
            if (count > 0) {
                rs = statement.getGeneratedKeys();
                if (rs.next()) {
                    id = rs.getInt(1);
                }
            }
        } catch (SQLException e) {
            throw e;
        } finally {
            dbManager.release(DBPoolName.macim_master, conn, statement, rs);
        }
        return id;
    }

    public boolean checkAndUpdateRelation(int userAId, int userBId,
            boolean isFromMaster) throws SQLException {
        if (userAId <= 0 || userBId <= 0) {
            return true;
        }
        DBManager dbManager = DBManager.getInstance();
        DBPoolName dbPoolName = isFromMaster ? DBPoolName.macim_master
                : DBPoolName.macim_slave;
        Connection conn = dbManager.getConnection(dbPoolName);
        PreparedStatement statement = null;
        ResultSet rs = null;
        int relateId = 0;
        List<Integer> updateRelateIds = new ArrayList<Integer>();
        try {
            String sql = "select * from IMRecentContact where (userId = ? "
                    + "and friendUserId = ?) or (userId = ? "
                    + "and friendUserId = ?) limit 2";
            statement = conn.prepareStatement(sql);
            int index = 1;
            statement.setObject(index++, userAId);
            statement.setObject(index++, userBId);
            statement.setObject(index++, userBId);
            statement.setObject(index++, userAId);
            rs = statement.executeQuery();
            while (rs.next()) {
                relateId = rs.getInt("relateId");
                if (rs.getInt("status") != 0) {
                    updateRelateIds.add(relateId);
                }
            }
        } catch (SQLException e) {
            throw e;
        } finally {
            dbManager.release(dbPoolName, conn, statement, rs);
        }
        int size = updateRelateIds.size();
        int succCount = 0;
        if (size > 0) {
            dbPoolName = DBPoolName.macim_master;
            conn = dbManager.getConnection(dbPoolName);
            statement = null;
            try {
                String sql = "update IMRecentContact set status = 0 where relateId in (";
                for (int i = 0; i < size; i++) {
                    sql += "?,";
                }
                sql = sql.substring(0, sql.length() - 1) + ") limit 4";
                statement = conn.prepareStatement(sql);
                for (int i = 0; i < size; i++) {
                    statement.setInt(i + 1, updateRelateIds.get(i));
                }
                succCount = statement.executeUpdate();
            } catch (SQLException e) {
                throw e;
            } finally {
                dbManager.release(dbPoolName, conn, statement, null);
            }
        }
        return succCount > 0;
    }

    /*
     * �ж��ǲ��Ǻ���
     * 
     * @param userAId
     * 
     * @param userBId
     * 
     * @return
     * 
     * @throws SQLException
     */
    public boolean isRecentContact(int userAId, int userBId)
            throws SQLException {
        int relateId = getRelateId(userAId, userBId);
        return relateId > 0;
    }

    /*
     * 
     * @Description: ��ȡ����������û��Ŀͷ���ֻ��һ��֮�ڣ������ϵ����һ�ܾͺ�����...
     * 
     * @return
     * 
     * @throws Exception
     */
    public int getLastServiceUser(int userId, int[] shopUserId)
            throws Exception {
        if (userId <= 0 || shopUserId == null || shopUserId.length == 0) {
            return 0;
        }

        List<Integer> friendUserIdList = new ArrayList<Integer>();
        String smallWhereClause = MoguUtil.getArgsHolder(shopUserId.length);
        // һ��֮�ڵ���Ч
        int oneWeekAgo = (int) (System.currentTimeMillis() / 1000) - 604800; // 86400
                                                                             // *
                                                                             // 7
        String sql = "select friendUserId from IMRecentContact where userId = ? "
                + "and updated > ? and friendUserId in ("
                + smallWhereClause
                + ") order by updated desc limit 1";

        DBManager dbManager = DBManager.getInstance();
        Connection conn = dbManager.getConnection(DBPoolName.macim_slave);
        PreparedStatement statement = null;
        ResultSet rs = null;
        try {
            statement = conn.prepareStatement(sql);
            int index = 1;
            statement.setInt(index++, userId);
            statement.setInt(index++, oneWeekAgo);
            for (int i = 0; i < friendUserIdList.size(); i++) {
                statement.setObject(index++, friendUserIdList.get(i));
            }
            rs = statement.executeQuery();

            if (rs.next()) {
                return rs.getInt("friendUserId");
            }
        } catch (Exception e) {
            throw e;
        } finally {
            dbManager.release(DBPoolName.macim_slave, conn, statement, rs);
        }
        return 0;
    }

    public boolean updateRelationShip(int fromUserId, int toUserId, int updated)
            throws SQLException {
        if (fromUserId <= 0 || updated <= 0) {
            return false;
        }

        String sql = "update IMRecentContact set updated=? where "
                + "(userId=? and friendUserId=?) or (userId=? and friendUserId=?)";

        DBManager dbManager = DBManager.getInstance();
        Connection conn = dbManager.getConnection(DBPoolName.macim_master);
        PreparedStatement statement = null;
        try {
            statement = conn.prepareStatement(sql);
            int index = 1;
            statement.setObject(index++, updated);
            statement.setObject(index++, fromUserId);
            statement.setObject(index++, toUserId);
            statement.setObject(index++, toUserId);
            statement.setObject(index++, fromUserId);
            statement.executeUpdate();
            return true;
        } catch (SQLException e) {
            throw e;
        } finally {
            dbManager.release(DBPoolName.macim_master, conn, statement, null);
        }
    }
}

#endif