#include "passworddatabase.h"
#include "encryptor.h"
#include <iostream>

using namespace std;

PasswordDataBase::PasswordDataBase()
{
    data_file.setFileName("./saves.txt");

    encr.setFileName(data_file.fileName());
    encr.setFlag("encrypted");

    iter = element_multi_map.begin();
}

PasswordDataBase::~PasswordDataBase()
{}

void PasswordDataBase::start()
{
    decryptDataFile();
    readToMap();
    showMenu();
}

void PasswordDataBase::finish()
{
    writeFromMap();
    encryptDataFile();
    exit(0);
}

void PasswordDataBase::decryptDataFile()
{
//    if(encr.isFileDataEncrypted())
//        encr.decryptFileProcess();
}

void PasswordDataBase::encryptDataFile()
{
//    if(!encr.isFileDataEncrypted())
//        encr.encryptFileProcess();
}

void PasswordDataBase::readToMap()
{
    if(!data_file.isOpen())
        data_file.open(QIODevice::ReadOnly);

    while(!data_file.atEnd())
    {
        QString temp_key = data_file.readLine();

        element_multi_map.insertMulti(temp_key, data_file.readLine());
        element_multi_map.insertMulti(temp_key, data_file.readLine());
        element_multi_map.insertMulti(temp_key, data_file.readLine());
    }

    data_file.close();
}

void PasswordDataBase::writeFromMap()
{
    if(!data_file.isOpen())
        data_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Unbuffered);

    QString last_key = NULL;
    QByteArray key_buff, name_buff, note_buff, password_buff;

    for(iter = element_multi_map.begin(); iter != element_multi_map.end(); ++iter)
    {
        if(iter.key() != last_key)
        {
            last_key = iter.key();
            key_buff.append(last_key);
            data_file.write(key_buff);
            key_buff.clear();
        }

        password_buff.append((iter++).value());
        note_buff.append((iter++).value());
        name_buff.append((iter).value());

        data_file.write(name_buff);
        data_file.write(note_buff);
        data_file.write(password_buff);

        name_buff.clear();
        note_buff.clear();
        password_buff.clear();
        key_buff.clear();
    }

    data_file.close();
}

void PasswordDataBase::showMap()
{
    short counter = 0;
    QString last_key = NULL;

    cout << "\n\tSaved data :\n";

    for(iter = element_multi_map.begin(); iter != element_multi_map.end(); ++iter)
        if(iter.key() != last_key)
        {
            last_key = iter.key();
            cout << "\t\tdata#" << counter++ << " " << last_key.toStdString();
        }

}
void PasswordDataBase::showMenu()
{
    system("clear");
    showMap();

    cout << "\n\t1. Add data"
            "\n\t2. Show data"
            "\n\t3. Edit data"
            "\n\t4. Remove data"
            "\n\t5. Wipe data"
            "\n\t6. Exit"
            "\n\tset : ";

    switch(getInt())
    {
    case 1 : addElement(); break;
    case 2 : showElement(); break;
    case 3 : editElement(); break;
    case 4 : removeElement(); break;
    case 5 : wipeDataFile(); break;
    case 6 : finish(); break;
    }
}

void PasswordDataBase::addElement()
{}
void PasswordDataBase::showElement()
{}
void PasswordDataBase::editElement()
{}
void PasswordDataBase::removeElement()
{}
void PasswordDataBase::wipeDataFile()
{}
void PasswordDataBase::generatePassword()
{}

int PasswordDataBase::getInt()
{
    int value;

    while(true)
    {
        cin  >> value;
        if(cin.good())
        {
            cin.ignore(10, '\n');
            break;
        }
        cin.clear();
        cin.ignore(10, '\n');
    }

    return value;
}
