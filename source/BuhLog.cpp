#include <BuhLog.h>
#include <QThread>
Q_LOGGING_CATEGORY(network,"network");
Q_LOGGING_CATEGORY(server,"server");
Q_LOGGING_CATEGORY(client,"client");

QString BuhLog::filename_ = QDir::currentPath() + QDir::separator() + "log2.txt";
bool BuhLog::logging_ = false;

static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(nullptr);


BuhLog::BuhLog()
{

}

void BuhLog::attach()
{
    BuhLog::logging_ = true;
    qInstallMessageHandler(BuhLog::handler);
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

void BuhLog::handler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QThread* t = QThread::currentThread();
    Qt::HANDLE hand = t->currentThreadId();
    std::thread::id this_id = std::this_thread::get_id();
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

        QFile file(BuhLog::filename_);
        if(file.open(QIODevice::Append))
        {
            QTextStream ts(&file);

            ts << QDateTime::currentDateTime().toString() << " - "
               << txt <<" "
               << context.file <<" "
               << context.category <<" "
               << " line: " << context.line
               << '\r' << '\n';
            ts.flush();
            file.close();
        }
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


