#ifndef SINGLEINSTANCEGUARD_H
#define SINGLEINSTANCEGUARD_H

#include <QObject>
#include <QString>

class QLocalServer;

class SingleInstanceGuard : public QObject
{
    Q_OBJECT

public:
    explicit SingleInstanceGuard(const QString &serverName, QObject *parent = nullptr);
    ~SingleInstanceGuard();

    bool tryToRun();
    bool isPrimary() const { return m_isPrimary; }
    void notifyExistingInstance() const;

signals:
    void activationRequested();

private slots:
    void handleNewConnection();

private:
    QString m_serverName;
    QLocalServer *m_server;
    bool m_isPrimary;
};

#endif // SINGLEINSTANCEGUARD_H
