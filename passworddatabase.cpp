#include "passworddatabase.h"
//#include "encryptor.h"
#include <iostream>
#include <iomanip>

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
    encr.decryptFileProcess();
}

void PasswordDataBase::encryptDataFile()
{
    if(!encr.isFileDataEncrypted())
        encr.encryptFileProcess();
}

void PasswordDataBase::readToMap()
{
    element_multi_map.clear();

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
    while(true)
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

        switch(getSwitchChoice(1, 6))
        {
        case 1 : addElement(); writeFromMap(); break;
        case 2 : showElement(); break;
        case 3 : editElement(); writeFromMap(); break;
        case 4 : removeElement(); writeFromMap(); break;
        case 5 : wipeDataFile(); writeFromMap(); break;
        case 6 : finish(); break;
        default : cerr << "\n\t#error : fatal error..."; exit(1); break;
        }
    }
}

void PasswordDataBase::addElement()
{
    string name, note, password;

    cout << "\n\tSet new data name (20 max) : ";
    cin >> setw(20) >> name;

    if(element_multi_map.contains(QString::fromStdString(name + '\n')))
    {
        cin.ignore(10, '\n');
        short temp;
        cout << "\n\t#error : \"" << name << "\" is already saved...";
        cout << "\n\tPress Enter...";
        cin.unsetf(ios::skipws);
        cin >> temp;
        cin.setf(ios::skipws);
        cin.clear();
        cin.ignore(10, '\n');
        return;
    }

    cin.ignore(10, '\n');
    cin.unsetf(ios::skipws);
    cout << "\n\tSet a short note for this data (120 max) : ";
    cin >> setw(120) >> note;
    cin.setf(ios::skipws);

    cin.clear();
    cin.ignore(10, '\n');

    system("clear");
    password = generatePassword();

    name += "\n";
    note += "\n";
    password += "\n";

    element_multi_map.insertMulti(QString::fromStdString(name), QString::fromStdString(name));
    element_multi_map.insertMulti(QString::fromStdString(name), QString::fromStdString(note));
    element_multi_map.insertMulti(QString::fromStdString(name), QString::fromStdString(password));
}

void PasswordDataBase::showElement()
{
    string name, note, password;

    cout << "\n\tWhat to show : ";
    cin >> setw(20) >> name;

    if(!element_multi_map.contains(QString::fromStdString(name + '\n')))
    {
        cin.ignore(10, '\n');
        short temp;
        cout << "\n\t#error : \"" << name << "\" is not saved...";
        cout << "\n\tPress Enter...";
        cin.unsetf(ios::skipws);
        cin >> temp;
        cin.setf(ios::skipws);
        cin.clear();
        cin.ignore(10, '\n');
        return;
    }

    system("clear");
    iter = element_multi_map.find(QString::fromStdString(name + '\n'));

    password = (iter++).value().toStdString();
    note = (iter++).value().toStdString();
    name = (iter++).value().toStdString();

    cout << "\tName : " << name;
    cout << "\tNote : " << note;
    cout << "\tPassword : " << password;

    cin.ignore(10, '\n');
    short temp;
    cout << "\n\tPress Enter to back to menu...";
    cin.unsetf(ios::skipws);
    cin >> temp;
    cin.setf(ios::skipws);
    cin.clear();
    cin.ignore(10, '\n');
    return;
}

void PasswordDataBase::editElement()
{
    string new_name, name, note, password;

    cout << "\n\tWhat to edit : ";
    cin >> setw(20) >> name;

    name += "\n";

    if(!element_multi_map.contains(QString::fromStdString(name)))
    {
        cin.ignore(10, '\n');
        short temp;
        cout << "\n\t#error : \"" << name << "\" is not saved...";
        cout << "\n\tPress Enter...";
        cin.unsetf(ios::skipws);
        cin >> temp;
        cin.setf(ios::skipws);
        cin.clear();
        cin.ignore(10, '\n');
        return;
    }

    cin.ignore(10, '\n');

    while(true)
    {
        system("clear");
        iter = element_multi_map.find(QString::fromStdString(name));

        if(iter == element_multi_map.end())
            clog << "\nlog : iter == end()";

        password = (iter++).value().toStdString();
        note = (iter++).value().toStdString();
        name = (iter++).value().toStdString();

        cout << "\tName : " << name;
        cout << "\tNote : " << note;
        cout << "\tPassword : " << password;

        cout << endl;

        cout << "\n\t1. Change name"
                "\n\t2. Change note"
                "\n\t3. Change password"
                "\n\t4. Back"
                "\n\tset : ";

        switch(getSwitchChoice(1, 4))
        {
        case 1 :
        {
            cout << "\n\tSet new name (20 max) : ";
            cin >> setw(20) >> new_name;
            new_name += "\n";
            cin.ignore(10, '\n');
            break;
        }
        case 2 :
        {
            cout << "\n\tSet new note (120 max) : ";
            cin >> setw(120) >> note;
            note += "\n";
            cin.ignore(10, '\n');
            break;
        }
        case 3 : system("clear"); password = generatePassword(); password += "\n"; break;
        case 4 : return; break;
        default : cerr << "\n\t#error : fatal error..."; exit(1); break;
        }

        iter = element_multi_map.find(QString::fromStdString(name));

        if(iter == element_multi_map.end())
            clog << "\nlog : iter == end()";

        element_multi_map.erase(iter++);
        element_multi_map.erase(iter++);
        element_multi_map.erase(iter++);

        name = new_name;

        element_multi_map.insertMulti(QString::fromStdString(name), QString::fromStdString(name));
        element_multi_map.insertMulti(QString::fromStdString(name), QString::fromStdString(note));
        element_multi_map.insertMulti(QString::fromStdString(name), QString::fromStdString(password));
    }
}

void PasswordDataBase::removeElement()
{
    string name;
    cout << "\n\tWhat to remove : ";
    cin >> setw(20) >> name;

    if(!element_multi_map.contains(QString::fromStdString(name + '\n')))
    {
        cin.ignore(10, '\n');
        short temp;
        cout << "\n\t#error : \"" << name << "\" is not saved...";
        cout << "\n\tPress Enter...";
        cin.unsetf(ios::skipws);
        cin >> temp;
        cin.setf(ios::skipws);
        cin.clear();
        cin.ignore(10, '\n');
        return;
    }

    iter = element_multi_map.find(QString::fromStdString(name + '\n'));

    element_multi_map.erase(iter++);
    element_multi_map.erase(iter++);
    element_multi_map.erase(iter++);

    cin.clear();
    cin.ignore(10, '\n');
}

void PasswordDataBase::wipeDataFile()
{
    element_multi_map.clear();
}

string PasswordDataBase::generatePassword()
{
    cout << "\n\tSelect the complexity of the password"
            "\n\t1. Only digits"
            "\n\t2. Only letters"
            "\n\t3. Only specials"
            "\n\t4. Digits + letters"
            "\n\t5. Digits + specials"
            "\n\t6. Letters + specials"
            "\n\t7. Digits + letters + specials (recommended)"
            "\n\tset : ";

    switch(getSwitchChoice(1, 7))
    {
    case 1 : setPasswordSimbolsDigits(); break;
    case 2 : setPasswordSimbolsLetters(); break;
    case 3 : setPasswordSimbolsSpecials(); break;
    case 4 : setPasswordSimbolsDigitsLetters(); break;
    case 5 : setPasswordSimbolsDigitsSpecials(); break;
    case 6 : setPasswordSimbolsLettersSpecials(); break;
    case 7 : setPasswordSimbolsDigitsLettersSpecials(); break;
    default : cerr << "\n\t#error : fatal error..."; exit(1); break;
    }

    string password;
    short password_length = 25;
    short password_simbols_length = strlen(password_simbols);

    srand(time(NULL));

    for(int i = 0; i < password_length; ++i)
        password += password_simbols[rand() % password_simbols_length];

    return password;
}

int PasswordDataBase::getSwitchChoice(int min, int max)
{
    int value;
    cin.unsetf(ios::skipws);
    while(true)
    {
        cin  >> value;
        if(value < min || value > max)
            cin.clear(ios::failbit);
        if(cin.good())
        {
            cin.ignore(10, '\n');
            cin.setf(ios::skipws);
            return value;
        }
        cout << "\n\t#error : input error, try again...\n\tset : ";
        cin.clear();
        cin.ignore(10, '\n');
    }
}

//password simbols generation functions
void PasswordDataBase::setPasswordSimbolsDigits()
{
    password_simbols = new char[11];

    for(int i = 0; i < 10; ++i)
        password_simbols[i] = i + 48;

    password_simbols[10] = '\0';
}
void PasswordDataBase::setPasswordSimbolsLetters()
{
    password_simbols = new char[53];

    for(int i = 0; i < 26; ++i)
        password_simbols[i] = i + 65;
    for(int i = 26; i < 52; ++i)
        password_simbols[i] = i + 71;

    password_simbols[52] = '\0';
}
void PasswordDataBase::setPasswordSimbolsSpecials()
{
    password_simbols = new char[16];

    for(int i = 0; i < 15; ++i)
        password_simbols[i] = i + 33;

    password_simbols[15] = '\0';
}
void PasswordDataBase::setPasswordSimbolsDigitsLetters()
{
    password_simbols = new char[63];

    for(int i = 0; i < 26; ++i)
        password_simbols[i] = i + 65;
    for(int i = 26; i < 52; ++i)
        password_simbols[i] = i + 71;
    for(int i = 52; i < 62; ++i)
        password_simbols[i] = i - 4;

    password_simbols[62] = '\0';
}
void PasswordDataBase::setPasswordSimbolsDigitsSpecials()
{
    password_simbols = new char[26];

    for(int i = 0; i < 25; ++i)
        password_simbols[i] = i + 33;

    password_simbols[25] = '\0';
}
void PasswordDataBase::setPasswordSimbolsLettersSpecials()
{
    password_simbols = new char[68];

    for(int i = 0; i < 26; ++i)
        password_simbols[i] = i + 65;
    for(int i = 26; i < 52; ++i)
        password_simbols[i] = i + 71;
    for(int i = 52; i < 67; ++i)
        password_simbols[i] = i - 19;

    password_simbols[67] = '\0';
}
void PasswordDataBase::setPasswordSimbolsDigitsLettersSpecials()
{
    password_simbols = new char[78];

    for(int i = 0; i < 26; ++i)
        password_simbols[i] = i + 65;
    for(int i = 26; i < 52; ++i)
        password_simbols[i] = i + 71;
    for(int i = 52; i < 77; ++i)
        password_simbols[i] = i - 19;

    password_simbols[77] = '\0';
}
//end of password simbols generation functions
