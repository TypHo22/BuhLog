#include <BuhLog.h>
//QT
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QDir>
#include <QTextStream>
//STL
#include <sstream>
#include <iostream>
#include <mutex>

Q_LOGGING_CATEGORY(network,"network");
Q_LOGGING_CATEGORY(server,"server");
Q_LOGGING_CATEGORY(client,"client");

bool BuhLog::logging_ = false;
std::unique_ptr<LogManager> BuhLog::manager_ = std::make_unique<LogManager>();
ThreadPool BuhLog::pool_ = ThreadPool(3);
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(nullptr);
std::mutex mut;

void dumpToFile(QFile *file, const QString &logMsg)
{
    QTextStream ts(file);
    ts << logMsg << "\n";
    ts.flush();
}

BuhLog::BuhLog()
{

}

BuhLog::~BuhLog()
{
    shutDown();
}

void BuhLog::startUp()
{
    BuhLog::logging_ = true;
    qInstallMessageHandler(BuhLog::logMessage);
    qSetMessagePattern("[%{time yyyyMMdd h:mm:ss.zzz t}"
                       " %{category} "
                       " %{threadid}"
                       " %{if-debug}Debug%{endif}"
                       " %{if-info}Info%{endif}"
                       " %{if-warning}Warning%{endif}"
                       " %{if-critical}Critcal%{endif}"
                       " %{if-fatal}Fatal%{endif}] "
                       " %{file}:%{line} - %{message}");

    QSysInfo sys;

    qInfo() << "System Info";
    qInfo() << "Boot Id: " << sys.bootUniqueId();
    qInfo() << "Build: " << sys.buildAbi();
    qInfo() << "Cpu: " << sys.buildCpuArchitecture();
    qInfo() << "Kernel: " << sys.kernelType();
    qInfo() << "Version: " << sys.kernelVersion();

    qInfo() << "Host: " << sys.machineHostName();
    qInfo() << "Product: " << sys.prettyProductName();
    qInfo() << "Type: " << sys.productType();
    qInfo() << "Version: " << sys.productVersion();

#ifdef Q_OS_LINUX
    qInfo() << "Linux code here";
#elif defined(Q_OS_WIN)
    qInfo() << "Windows code here";
#elif defined(Q_OS_MACX)
    qInfo() << "Mac code here";
#else
    qInfo() << "Unknown OS code here";
#endif
}

void BuhLog::logMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(BuhLog::logging_)
    {
        QString txt;
        switch (type) {
        case QtInfoMsg:
            txt = QString("Info: %1").arg(msg);
            break;
        case QtDebugMsg:
            txt = QString("Debug: %1").arg(msg);
            break;
        case QtWarningMsg:
            txt = QString("Warning: %1").arg(msg);
            break;
        case QtCriticalMsg:
            txt = QString("Critical: %1").arg(msg);
            break;
        case QtFatalMsg:
            txt = QString("Fatal: %1").arg(msg);
            break;
        }

        std::stringstream ss;
        ss << std::this_thread::get_id();
        const QString threadId = QString::fromStdString(ss.str());

        QString logMsg;
        logMsg.append(QDateTime::currentDateTime().toString() + " - ");
        logMsg.append(context.category);
        logMsg.append(" ");
        logMsg.append("Log: " + txt + " ");
        logMsg.append("From Thread: " + threadId + " ");

        pool_.addTask(Task(logMsg,threadId,manager_.get()));
    }

    (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
}

void BuhLog::setLogging(bool logging, const QString &category, QtMsgType type)
{
    QString cmd = category;
    switch (type)
    {
        case QtInfoMsg:
        {
            cmd += ".info=";
            break;
        }
        case QtDebugMsg:
        {
            cmd += ".debug=";
            break;
        }
        case QtWarningMsg:
        {
            cmd += ".warning=";
            break;
        }
        case QtCriticalMsg:
        {
            cmd += ".critical=";
            break;
        }
        case QtFatalMsg:
        {
            cmd += ".fatal=";
            break;
        }
    }

    if(logging)
        cmd += "true";
    else
        cmd += "false";

    QLoggingCategory::setFilterRules(cmd);
}

void BuhLog::configureLogger(bool singleLogFile, int maxFileSize, int maxLogSeconds)
{
    manager_->setSingleLog(singleLogFile);
    manager_->setMaxFileSize(maxFileSize);
    manager_->setMaxLogSeconds(maxLogSeconds);
}

void BuhLog::shutDown()
{
    logging_ = false;
    manager_->closeAll();
    pool_.stop();
}




