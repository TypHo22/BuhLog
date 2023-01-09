#include <LogManager.h>
#include <QDir>
LogManager::LogManager():maxFileSize_(-1),maxLogSeconds_(-1),oneLogFile_(true)
{

}

LogManager::~LogManager()
{
    for(auto& f : files_)
    {
        f.file_->close();
        delete f.file_;
    }
}

QFile* LogManager::getFile()
{
    if(files_.empty())
    {
        addNewFile();

        return files_.front().file_;
    }

    LogFile* pickedLog = nullptr;

    if(oneLogFile_)
    {
        pickedLog = singleLog();
    }
    else
    {
        pickedLog = multiLog();
    }

    return pickedLog->file_;
}

void LogManager::addNewFile()
{
    QString name;

    if(requestedThread_.isEmpty())
    {
        name = "logFile_" + QDateTime::currentDateTime().toString();
    }
    else
    {
        name = requestedThread_ + "_" + QDateTime::currentDateTime().toString();
    }

    name = QDir::currentPath() + QDir::separator() + name;

    files_.emplace_back(LogFile{new QFile(name),requestedThread_,QDateTime::currentDateTime()});
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
    delete pickedLog->file_;
    pickedLog->file_ = nullptr;
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

void LogManager::setOneLogFile(bool oneLogFile)
{
    oneLogFile_ = oneLogFile;
}

void LogManager::setMaxLogSeconds(int maxLogSeconds)
{
    maxLogSeconds_ = maxLogSeconds;
}

void LogManager::setMaxFileSize(int maxFileSize)
{
    maxFileSize_ = maxFileSize;
}



