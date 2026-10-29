#include "LocalizationManager.h"

#include <fstream>
#include <iostream>

using namespace std;

// Загружает файл локализации.
// Например:
// assets/localization/en.txt
// assets/localization/ru.txt
bool LocalizationManager::LoadLanguage(const string& languageCode)
{
    string path =
        "assets/localization/" +
        languageCode +
        ".txt";

    ifstream file(path);

    // Если файл не удалось открыть,
    // выводим ошибку в консоль.
    if (!file.is_open())
    {
        cout
            << "Failed to load language file: "
            << path
            << endl;

        return false;
    }

    // Очищаем старый язык перед загрузкой нового.
    translations.clear();

    string line;

    while (getline(file, line))
    {
        // Пустые строки пропускаем.
        if (line.empty())
        {
            continue;
        }

        // Строки с # считаем комментариями.
        if (line[0] == '#')
        {
            continue;
        }

        // Ищем знак =
        size_t separator = line.find('=');

        // Если знака = нет,
        // строка имеет неправильный формат.
        if (separator == string::npos)
        {
            continue;
        }

        // Всё слева от "=" — ключ.
        string key =
            line.substr(0, separator);

        // Всё справа от "=" — перевод.
        string value =
            line.substr(separator + 1);

        // Сохраняем перевод.
        translations[key] = value;
    }

    file.close();

    // Запоминаем текущий язык.
    currentLanguage = languageCode;

    return true;
}


// Возвращает перевод по ключу.
string LocalizationManager::Get(const string& key) const
{
    auto result =
        translations.find(key);

    // Если перевод найден — возвращаем его.
    if (result != translations.end())
    {
        return result->second;
    }

    // Если перевод забыли добавить,
    // показываем сам ключ вместо краша.
    return key;
}


// Возвращает текущий код языка.
string LocalizationManager::GetCurrentLanguage() const
{
    return currentLanguage;
}