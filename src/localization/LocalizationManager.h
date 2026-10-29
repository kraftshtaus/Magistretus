#pragma once

#include <string>
#include <unordered_map>

using namespace std;

// LocalizationManager отвечает за загрузку
// и получение переводов из языковых файлов.
class LocalizationManager
{
public:
    // Загружает язык по коду.
    // Например: "en" или "ru".
    bool LoadLanguage(const string& languageCode);

    // Возвращает перевод по ключу.
    // Если ключ не найден, вернётся сам ключ.
    string Get(const string& key) const;

    // Возвращает код текущего языка.
    string GetCurrentLanguage() const;

private:
    // Здесь хранятся пары:
    // "menu.play" -> "Play"
    unordered_map<string, string> translations;

    // Язык по умолчанию.
    string currentLanguage = "en";
};