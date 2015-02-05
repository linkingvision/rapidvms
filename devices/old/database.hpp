

#ifndef _SVR_DATABASE_H_
#define _SVR_DATABASE_H_

#include "utility.hpp"

typedef enum
{
    SVRDB_CMD_CNT = 1,
    SVRDB_CMD_LAST
} SVRDBCmd;

#define SVR_DB_RECORD_TIME 60

typedef struct
{
    /* Command */
    SVRDBCmd cmd;
    u32 channel;
    s32 timeLast;
    s32 timeNow;
    s32 in;
    s32 out;
}SVRDBCmdItem;

typedef CmdBuffer<SVRDBCmdItem> DatabaseQueue;

class SvrDatabase
{
public:
    SvrDatabase(astring pPath);
    ~SvrDatabase();
public:
    static void Run(void * pParam);
    void Run1();

public:
    static void PushCmd(SVRDBCmdItem &item);
    void PushCmdInternal(SVRDBCmdItem &item);

private:
    DatabaseQueue m_Queue;
    astring m_DbFile;
    thread *m_pThread;
};

typedef SvrDatabase* LPSvrDatabase;

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <windows.h>

#include "SQLiteC++.h"

#define DATABASE_NAME "test.db3"

LPSvrDatabase gSvrDatabase = NULL;

inline const std::string currentDateTime(time_t nTime) {
    time_t     now = nTime;
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y/%m/%d %X", &tstruct);

    return buf;
}

inline SvrDatabase::SvrDatabase(astring pPath)
{
    m_DbFile = pPath + "\\" +  DATABASE_NAME;

    gSvrDatabase = this;

    m_pThread = new thread(SvrDatabase::Run, (void *)this);
}

inline SvrDatabase::~SvrDatabase()
{

}

inline void SvrDatabase::PushCmd(SVRDBCmdItem &item)
{
    if (gSvrDatabase)
    {
        gSvrDatabase->PushCmdInternal(item);
    }

    return;
}

inline void SvrDatabase::PushCmdInternal(SVRDBCmdItem &item)
{
    m_Queue.Push(&item);

    return;
}


inline void SvrDatabase::Run(void * pParam)
{
    LPSvrDatabase pThread = (LPSvrDatabase)pParam;

    if (pThread)
    {
        //Sleep(1000 * 2);
        pThread->Run1();
    }
}

inline void SvrCleanUpData(SQLite::Database & db)
{
    char bufferSql[2048];
    s32 currentLastTime = time(NULL) - (60 * 60 * 24) * 30;

    astring strCurrentLastTime = currentDateTime(currentLastTime);

    sprintf(bufferSql, "delete from data_counter where time_start <\"%s\"",
            strCurrentLastTime.c_str());
    try
    {
        db.exec(bufferSql);
    }
    catch (std::exception& e)
    {
        std::cout << "SQLite exception: " << e.what() << std::endl;
        //Sleep(40);
    }

    return;
}

inline void SvrDatabase::Run1()
{
    try
    {
        SQLite::Database    db(m_DbFile.c_str(), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
        db.exec("CREATE TABLE DATA_COUNTER (ID INTEGER PRIMARY KEY, TIME_START DATE, \
TIME_END DATE, DEVICE INTEGER, IN_ INTEGER, OUT INTEGER)");
    }
    catch (std::exception& e)
    {
        std::cout << "SQLite exception: " << e.what() << std::endl;
    }

    char bufferInsert[2048];
    // Open a database file in readonly mode
    SQLite::Database    db(m_DbFile.c_str(), SQLITE_OPEN_READWRITE);
    std::cout << "SQLite database file '" << db.getFilename().c_str() << "' opened successfully\n";
    SVRDBCmdItem cmd;
    memset(&cmd, 0, sizeof(cmd));
    s32 startTime = time(NULL);
    s32 currentTime = startTime;

    SVR_DEBUG(CMN_DEBUG, "%s Clean up data\n", __FUNCTION__);
    SvrCleanUpData(db);

    while(1)
    {
        currentTime = time(NULL);
        //if (currentTime - startTime >= 60)
        if (currentTime - startTime >= 60 * 60 * 24)
        {
            SVR_DEBUG(CMN_DEBUG, "%s Clean up data\n",
                    __FUNCTION__);
            SvrCleanUpData(db);
            startTime = currentTime;
        }

        if (m_Queue.Pop(&cmd) == FALSE)
        {
            Sleep(1000 * 1);
            continue;
        }

        if (cmd.cmd == SVRDB_CMD_CNT)
        {
            astring strLastTime = currentDateTime(cmd.timeLast);
            astring strCurrentTime = currentDateTime(cmd.timeNow);
            SVR_DEBUG(CMN_DEBUG, "%s Time Start %s End %s Channel %d In %d Out %d\n",
                    __FUNCTION__, strLastTime.c_str(), strCurrentTime.c_str(),
                    cmd.channel, cmd.in, cmd.out);
            sprintf(bufferInsert, "INSERT INTO DATA_COUNTER VALUES (NULL, \
                \"%s\", \"%s\", %d, %d, %d)", strLastTime.c_str(), strCurrentTime.c_str(),
                cmd.channel, cmd.in, cmd.out);
again:
            try
            {
                db.exec(bufferInsert);
            }
            catch (std::exception& e)
            {
                std::cout << "SQLite exception: " << e.what() << std::endl;
                Sleep(40);
                goto again;
            }

        }else if (cmd.cmd == SVRDB_CMD_LAST)
        {
            //TODO list all the table
        }
    }

    return ;
}



#endif /* _SVR_DATABASE_H_ */
