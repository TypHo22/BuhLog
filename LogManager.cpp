#include <LogManager.h>
#include <QDir>

LogManager::LogManager():maxFileSize_(-1),maxLogSeconds_(-1),singleLog_(true)
{

}

LogManager::~LogManager()
{

}

QFile* LogManager::getFile()
{
    if(files_.empty())
    {
        addNewFile();

        return files_.front().file_.get();
    }

    LogFile* pickedLog = nullptr;

    if(singleLog_)
    {
        pickedLog = singleLog();
    }
    else
    {
        pickedLog = multiLog();
    }

    return pickedLog->file_.get();
}

void LogManager::addNewFile()
{
    QString name = requestedThread_ + "_" + QDateTime::currentDateTime().toString();

    name = QDir::currentPath() + QDir::separator() + name;

    files_.emplace_back(LogFile{std::make_unique<QFile>(name),
                                requestedThread_,
                                QDateTime::currentDateTime()});

    files_.back().file_->open(QIODevice::Append);
}

void LogManager::removeFile(LogFile* pickedLog)
{
    auto logIt = std::find_if(files_.begin(),files_.end(),
                 [pickedLog](const LogFile& logFile)
    {
        return pickedLog->threadId_ == logFile.threadId_;
    });

    pickedLog->file_->close();
    files_.erase(logIt);
}

bool LogManager::checkMaxFileSize(LogFile* logFile) const
{
    if(maxFileSize_ != -1)
    {
        return logFile->file_->size() < maxFileSize_;
    }

    return true;
}

bool LogManager::checkMaxLogTime(LogFile* logFile) const
{
    if(maxLogSeconds_ != -1)
    {
        return logFile->timeCreated_.secsTo(QDateTime::currentDateTime()) < maxLogSeconds_;
    }

    return true;
}

bool LogManager::checkLogValid(LogFile* logFile) const
{
    return checkMaxFileSize(logFile) && checkMaxLogTime(logFile);
}

LogManager::LogFile *LogManager::singleLog()
{
    LogFile* pickedLog = &files_.back();

    if(checkLogValid(pickedLog))
    {
       return pickedLog;
    }
    else
    {
       removeFile(pickedLog);
       addNewFile();
       return &files_.back();
    }
}

LogManager::LogFile *LogManager::multiLog()
{
    auto logIt = std::find_if(files_.begin(),files_.end(),
                 [this](const LogFile& logFile)
    {
        return this->requestedThread_ == logFile.threadId_;
    });

    if(logIt == files_.end())
    {
        addNewFile();
        return &files_.back();
    }

    LogFile* pickedLog = &(*logIt);
    if(checkLogValid(pickedLog))
    {
        return pickedLog;
    }
    else
    {
        removeFile(pickedLog);
        addNewFile();
        return &files_.back();
    }
}

void LogManager::setRequestedThread(const QString &requestedThread)
{
    requestedThread_ = requestedThread;
}

void LogManager::closeAll()
{
    for(const auto& f : files_)
        f.file_.get()->close();
}

void LogManager::setMaxLogSeconds(int maxLogSeconds)
{
    maxLogSeconds_ = maxLogSeconds;
}

void LogManager::setSingleLog(bool singleLog)
{
    singleLog_ = singleLog;
}

void LogManager::setMaxFileSize(int maxFileSize)
{
    maxFileSize_ = maxFileSize;
}



