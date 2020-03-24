#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QString>

class Logic;

class FileHandler
{
public:
    FileHandler(Logic* logic);

    bool exists(QString filename);
    bool save(QString filename);
    bool open(QString filename);

private:
    Logic* m_logic;
};

#endif // FILEHANDLER_H
