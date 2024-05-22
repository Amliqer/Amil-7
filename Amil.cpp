#include <iostream> 
#include <fstream> 
#include <filesystem> 
#include <string> 

using namespace std;

void searchAndReplace(const string& folderPath, const string& searchWord, const string& replaceWord, bool removeWord) {
    int matches = 0;
    for (const auto& entry : filesystem::recursive_directory_iterator(folderPath)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        string filePath = entry.path().string();

        ifstream inputFile(filePath);
        ofstream outputFile(filePath + ".tmp"); // Создаем временный файл для записи 

        string line;
        while (getline(inputFile, line)) {
            size_t pos = line.find(searchWord);
            if (pos != string::npos) {
                matches++;
                if (removeWord) {
                    line.erase(pos, searchWord.length());
                }
                else {
                    line.replace(pos, searchWord.length(), replaceWord);
                }
            }
            outputFile << line << endl; // Записываем измененную строку во временный файл 
        }

        inputFile.close();
        outputFile.close();

        // Заменяем исходный файл временным файлом 
        filesystem::rename(filePath + ".tmp", filePath);
    }

    if (matches == 0) {
        cout << "Совпадений не найдено." << endl;
    }
    else {
        cout << "Всего найдено совпадений: " << matches << endl;
    }
}

int main() {
    string folderPath, searchWord, replaceWord;
    bool removeWord = false;

    cout << "Введите путь к папке: ";
    getline(cin, folderPath);

    cout << "Введите слово для поиска: ";
    getline(cin, searchWord);

    cout << "Вы хотите удалить искомое слово? (y/n): ";
    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        removeWord = true;
    }
    else {
        cout << "Введите слово для замены: ";
        cin >> replaceWord;
    }

    searchAndReplace(folderPath, searchWord, replaceWord, removeWord);

    return 0;
}