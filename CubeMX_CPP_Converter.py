#! python3
# CubeMX+CPP_Converter - заменяет расширения указанных файлов в папке
# Использование:
# Скрипт размещается в каталоге с проектом

import os, sys, time

# Имена/окончания заменяемых файлов (без расширения)
fileList = [
    "main", # Это имя используется в качестве маркера для динамического определения текущего расширения
    "*it"
]

# Массив заменяемых расширений
fileExtensions = [
    ".c",
    ".cpp"
]

# Текущий путь приложения
path = os.path.realpath(os.path.dirname(sys.argv[0]))


class ExtensionConverter(object):
    """Класс конвертера расширения файлов"""

    # Текущее расширение файлов
    __CurrentExtension = "no_extension"
    # Новое расширение файлов
    __NewExtension = "no_extension"

    def __init__(self, fileList, extensionList, directory):
        """ Конструктор класса """
        # Список файлов
        self.__FileList = fileList
        # Список расширений
        self.__ExtensionList = extensionList
        # Имя файла-маркера
        self.__MarkerName = self.__FileList[0]
        # Директория
        self.__Directory = directory

    def __IsNeedToChange(self, filename):
        """ Метод проверки необходимости изменения расширения для текущего файла """
        if not (filename.lower()).endswith(self.__CurrentExtension):
            return False
        
        for item in self.__FileList:
            if (item.find("*") == 0):
                item = item[1:]
                if (filename.lower()).endswith(item + self.__CurrentExtension):
                    return True
            else:
                if filename.lower() == (item + self.__CurrentExtension):
                    return True


            

        return False

    def __GetExtensionSettings(self, project_path):
        """ Метод автоматического определения настроек конвертации """
        for folderName, subfoldersName, fileNames in os.walk(project_path):
            for fileName in fileNames:
                ind = 0
                for item in self.__ExtensionList:
                    if self.__MarkerName.find("*") == 0:
                        if (fileName.lower()).endswith(self.__MarkerName + item):
                            self.__CurrentExtension = item
                            self.__NewExtension = self.__ExtensionList[(ind + 1) % len(self.__ExtensionList)]
                            return
                    else:
                        if fileName.lower() == (self.__MarkerName + item):
                            self.__CurrentExtension = item
                            self.__NewExtension = self.__ExtensionList[(ind + 1) % len(self.__ExtensionList)]
                            return
                    ind = ind + 1

    def __ChangeFileExtensions(self, project_path):
        """ Метод замены расширений файлов """
        for folderName, subfoldersName, fileNames in os.walk(project_path):
            for fileName in fileNames:
                if (self.__IsNeedToChange(fileName)):
                    source = folderName + '/' + fileName
                    os.rename(source, os.path.join(folderName, (fileName[:-(len(self.__CurrentExtension))] + self.__NewExtension)))

    def Process(self):
        """ Метод обработки файлов """
        self.__GetExtensionSettings(self.__Directory)
        self.__ChangeFileExtensions(self.__Directory)


# Выполнение программы
if __name__ == '__main__':
    # Экземпляр класса
    converter = ExtensionConverter(fileList, fileExtensions, path)
    converter.Process()
    print('Выполнено')