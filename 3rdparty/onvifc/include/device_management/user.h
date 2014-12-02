#ifndef USER_H
#define USER_H
#include <QObject>
namespace ONVIF {
    class Users : public QObject {
        Q_OBJECT
        Q_ENUMS(UserLevelType)
        Q_PROPERTY(QString userName READ userName WRITE setUserName)
        Q_PROPERTY(QString passWord READ passWord WRITE setPassWord)
        Q_PROPERTY(UserLevelType userLevel READ userLevel WRITE setUserLevel)
    public:
        explicit Users(QObject *parent = NULL);
        virtual ~Users();
        enum UserLevelType {Administrator,Operator,User,Anonymous,Extended};

        QString userName() const
        {
            return m_userName;
        }

        QString passWord() const
        {
            return m_passWord;
        }

        UserLevelType userLevel() const
        {
            return m_userLevel;
        }

    public slots:
        void setUserName(QString arg)
        {
            m_userName = arg;
        }

        void setPassWord(QString arg)
        {
            m_passWord = arg;
        }

        void setUserLevel(UserLevelType arg)
        {
            m_userLevel = arg;
        }

    private:
        QString m_userName;
        QString m_passWord;
        UserLevelType m_userLevel;
    };
}
#endif // USER_H
