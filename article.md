# Настройка проекта C++ c использованием библиотеки OpenMP. Обёртывание проекта для Python с помощью pybind11 и CMake.


<table>
  <tr>
    <td><img src="https://repository-images.githubusercontent.com/327470624/2d865200-6bf1-11eb-9d0e-2a488615d6ec" alt="Cpp logotype" width="400"/></td>
    <td><img src="https://fuzeservers.ru/wp-content/uploads/f/7/6/f767b64a7d72b5a23b468aa191c085bc.jpeg" alt="Python logotype" width="400"/></td>
  </tr>
  <tr>
    <td><img src="https://github.com/pybind/pybind11/raw/master/docs/pybind11-logo.png" alt="Pybind11 logotype" width="400"/></td>
    <td><img src="https://mms.businesswire.com/media/20201113005450/en/307885/22/OpenMPLogo-rgb.jpg" alt="OpenMP logotype" width="400"></td>
  </tr>
 </table>

## Введение

В этой статье показан простой практический пример настройки проекта на языке `C++` с использованием библиотеки для многопоточных вычислений `OpenMP` (однако можно использовать любую библиотеку, если есть необходимость), а также дальнейшее  обёртывание для использования в проектах написанных на `Python`.

В качестве инструмента обёртывания используется библиотека [pybind11](https://pybind11.readthedocs.io/en/stable/index.html), в качестве системы сборки [CMake](https://cmake.org/cmake/help/latest/guide/tutorial/index.html). 

Работа является следствием полученного личностного опыта и я надеюсь возможно сможет помочь (ускорить разработку) в проектировании схожих проектов. В статье возможны помарки в силу отсутствия глубокого понимания основ программирования на `C++`.

Сначала мы опишем пример библиотеки `C++`, а далее обернем библиотеку в обертку и протестируем в тестовом файле на `Python`, при этом проверим доступ к параллельной обработке (`OpenMP`).

Конечный результат:
* `C++` проект, который вы можете создавать независимо от `pybind11`.
* `Python` библиотека, созданная в результате переноса `C++` кода.

[Исходные файлы рассматриваемые в данной работе.](https://github.com/AntonSHBK/example_cpp_python_wrapper)

Описываемый пример развёрнут и протестирован на базе [Debian OS](https://www.debian.org/) с использованием технологии контейнеризации Docker [смотреть раздел docker исходных файлов](https://github.com/AntonSHBK/example_cpp_python_wrapper/tree/main/docker). Если ещё не ознакомились с данной технологией, крайне рекомендую ознакомиться, хотя бы на базовом уровне. Проект по идее должен нормально функционировать на любом дистрибутиве `Linux`, на платформе Windows могут возникнуть затруднения с использование библиотеки `pybind11`, рекомендую почитать [официальную документацию](https://pybind11.readthedocs.io/en/stable/basics.html) если возникли осложнения.

При проектировании проекта я использовал текстовый редактор `VS Code`, а также  расширение `CMake Tools` для  ускорения процесса сборки и написания кода.

## Зависимости

Для проекта C++:
* компилятор gcc;
* система сборки CMake.

```bush
apt-get update && \
apt-get upgrade && \
apt-get install -y \
    build-essential \
    cmake \
```
Для обёртывания проекта под Python:
* python3 и выше;
* pybind11.

```bush
pip install pybind11
```

## Создаём проект на C++ (CMake)

Мы будем использовать внешний (текущий) рабочий каталог для сборки python и внутренний каталог с именем cpp для сборки `C++` библиотеки.

Хорошей практикой является разделение исходных файлов и заголовочных, поэтому создаём соответствующие директории `src` и `include`, а также надо позаботиться о тестировании нашего кода `tests` (необязательно, как меня учили, это знак хорошего тона).

Промежуточный результат имеет вид:
```tree
workspace.
│   CMakeLists.txt
│
└───cpp
    │   CMakeLists.txt
    │
    ├───include
    │   └───my_lib
    │           my_include_file.h
    │
    ├───src
    │       my_cpp_file.cpp
    │
    └───tests
        │   CMakeLists.txt
        │
        └───src
                test_cpp.cpp
```

###  Разберём каждый файл

1. [/cpp/include/my_lib/my_include_file.h](https://github.com/AntonSHBK/example_cpp_python_wrapper/blob/main/cpp/include/my_lib/my_include_file.h)

Здесь мы описали некоторый класс, который на вход принимает строку. У класса тоже присутствуют методы, их логика не принципиальна и приведена только для демонстрации функционала.

```cpp
#include <string>
#ifndef MY_LIB
#define MY_LIB
namespace talker {
class SomeTalker {
private:
    std::string _text;
public:
    // Конструктор
    SomeTalker(std::string text);
    std::string get_text();
    void set_text(std::string text);
    int get_omp_max_treads();
    void get_text_parallel(int streams);};
int bob(const int &, const int &);
}
#endif
```

2. [/cpp/src/my_cpp_file.cpp](https://github.com/AntonSHBK/example_cpp_python_wrapper/blob/main/cpp/src/my_cpp_file.cpp)

Здесь мы описали все перечисленные функции заголовочного файла, обратите внимание при этом мы использовали библиотеку OpenMP. Функция `get_omp_max_treads` должна вывести в консоль количество доступных потоков на вашей машине; `get_text_parallel` должна вывести строку (`_text`) в параллельном потоке.

```cpp
#include <iostream>
#include <omp.h>
#include "my_lib/my_include_file.h"
namespace talker {
    // Конструктор 
    SomeTalker::SomeTalker(std::string text) {
        _text = text;
    }
    // Вернуть текст
    std::string SomeTalker::get_text() {
        return _text;
    }
    // Установить текст
    void SomeTalker::set_text(std::string text) {
        _text = text;
    }
    // Вернуть количество доступных поток
    int SomeTalker::get_omp_max_treads() {
        return omp_get_max_threads();
    }
    // Вернуть текст в параллельном потоке
    void SomeTalker::get_text_parallel(int streams = 4) {
        #pragma omp parallel for
        for (int i = 0; i < streams; i++)
        {
            std::cout << omp_get_thread_num() << _text;
        }
        std::cout << std::endl;
    }
    // Некоторая функция (просто пример понадобится в дальнейшем)
    int bob(const int &a, const int &b) {
        return a + b;
    }
}
```

3. [/cpp/tests/src/test_cpp.cpp](https://github.com/AntonSHBK/example_cpp_python_wrapper/tree/main/cpp/tests/src)

Здесь мы обращаемся к нашей библиотеке и создаём экземпляр класса, а также вызываем некоторые методы класса.

```cpp
#include "my_lib/my_include_file.h"
#include <iostream>
int main() {
    talker::SomeTalker my_talker("Hello world");
    std::cout << "Max threads is " << my_talker.get_omp_max_treads() << std::endl;
    my_talker.get_text_parallel(6);
    return 0;
}
```

4. [/cpp/tests/CMakeList.txt](https://github.com/AntonSHBK/example_cpp_python_wrapper/blob/main/cpp/tests/CMakeLists.txt)

Здесь мы описываем сборку теста. Добавляем исполняемый файл `add_executable`, добавляем написанную библиотеку `target_link_libraries`, а также указываем тест `add_test`.

```cmake
cmake_minimum_required(VERSION 3.25.1 FATAL_ERROR)

project(test_cpp VERSION 0.1.0 DESCRIPTION "test_cpp description")

# Стандарты
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} src/test_cpp.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC cpp_code)
add_test(NAME test_cpp COMMAND test_cpp)
```

5. [/cpp/CMakeList.txt](https://github.com/AntonSHBK/example_cpp_python_wrapper/blob/main/cpp/CMakeLists.txt)

Здесь мы описываем нашу библиотеку на `c++`. Постарался доходчиво расписать.

```cmake
cmake_minimum_required(VERSION 3.25.1 FATAL_ERROR)

project(cpp_code VERSION 0.1.0DESCRIPTION "cpp_to_python description")

# Стандарты
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Подключаем средства тестирования
include(CTest) # We include CTest which is part of CMake

# Указываем директории
set(PROJECT_SOURCE_DIR "src")
set(PROJECT_INCLUDE_DIR "include/my_lib")

# Заголовочные файлы
set(
    HEADERS 
    ${PROJECT_INCLUDE_DIR}/my_include_file.h
)

# Исходные файлы
set(
    SOURCE
    ${PROJECT_SOURCE_DIR}/my_cpp_file.cpp
)

# Добавляем библиотеку
add_library(
    ${PROJECT_NAME}
    SHARED
    # STATIC   
    ${HEADERS}
    ${SOURCE}
)

# Добавляем директорию для возможности подключения
target_include_directories(${PROJECT_NAME} PUBLIC  include/)

# Добавляем поддержку библиотеки OpenMP
find_package(OpenMP REQUIRED)
if(NOT OpenMP_CXX_FOUND)
	message(SEND_ERROR "Failed to find OpenMP.")
	return()
else()
	include_directories(${OpenMP_INCLUDE_DIRS})
    message("OpenMP FOUND dir is ${OpenMP_INCLUDE_DIRS}.")
endif()
# Дополнительный флаг, необходим для функционирования библиотеки
set(CMAKE_CXX_FLAGS "-fopenmp")

# Указание подключаемых библиотек
target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)

# Подключение самих тестов
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(tests)
endif ()

enable_testing()
```

6. [/CMakeLists.txt](https://github.com/AntonSHBK/example_cpp_python_wrapper/blob/main/CMakeLists.txt)

Пока нас не интересует данный файл, он нам понадобится когда будем делать обёртывание проекта, пока просто зададим название проекту, и установим стандарты... В дальнейшем мы его расширим.

```cmake
cmake_minimum_required(VERSION 3.25.1 FATAL_ERROR)

project(cpp_code VERSION 0.1.0 DESCRIPTION "cpp_to_python description")

# Стандарты
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
```
Возможно более элегантное описание проекта, но это уже на ваше усмотрение. Теперь можно приступать к сборке проекта и промежуточному тестированию.

## Сборка проекта C++ и тестирование
Сборка:
```bush
cd cpp && mkdir build
cd build
cmake ..
make
```
Если проект собрался без ошибок тестируем, если нет тогда выясняем в чём причина.
Тестирование (`--verbose` расширенный вывод):
```bush
ctest --verbose
```
Вывод в консоль достаточно большой поэтому я часть вспомогательной информации вырезал:
```
test 1
    Start 1: test_cpp

1: Test command: /app/cpp/build/tests/test_cpp
1: Working Directory: /app/cpp/build/tests
1: Test timeout computed to be: 1500
1: Max threads is 4
1: 0Hello world0Hello world3Hello world1Hello world1Hello world2Hello world
1/1 Test #1: test_cpp .........................   Passed    0.02 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.07 sec
```
Тест пройден успешно, можно видеть что в моей системе доступно 4 потока...

## Использование C++ библиотеки отдельно
Для использования библиотеки отдельно необходимо разместить её в удобном месте. Подразумевается что в проекте используется система сборки CMake:
```cmake
if (NOT TARGET cpp_code)
    add_subdirectory(path_to_project/name_project/cpp)
endif() 
target_link_libraries(${PROJECT_NAME} cpp_code)
```
Где `path_to_project` - оросительный путь к проекту, `name_project` - название проекта.

## Обертывание в Python при помощи pybind11

Для обёртывания используем библиотеку [pybind11]((https://pybind11.readthedocs.io/en/stable/index.html)).

Во время написания кода использовалась официальная документация а также доступные проекты других разработчиков. В случае возникновения   осложнений рекомендую ознакомиться со следующими материала (сам использовал):

- [Официальный туториал pybind11;](https://pybind11.readthedocs.io/en/stable/basics.html)
- [Официальные примеры Pybind11;](https://github.com/pybind/cmake_example)
- [Интересная статья по функциональным возможностям Pybind11;](https://smyt.ru/blog/sozdaem-s-python-rasshireniya-s-pomshyu-pybind11/?ysclid=lr6j5noobx652421112)
- [Интересная статья по описанию setup.py (сборка обёртки)](https://www.benjack.io/hybrid-python/c-packages-revisited/)

Сперва создадим директорию в которой будут располагаться исходники обёртки, назовём `Python`, также добавим один заголовочный файл и один исходный файл `python_header.h` и `python_src.cpp` соответственно. По хорошему их бы следовало аналогично разнести по директориям `src` и `include` но это в данном случае не принципиально.

Помимо этого в корневую директорию проекта добавим файл `setup.py` который будет осуществлять сборку нашего проекта под `python`. Можно было бы обойтись одним лишь Cmake, однако использование python для сборки мне показалось более практичным ([пример сборки CMake](https://pybind11.readthedocs.io/en/stable/compiling.html)).

Рассмотрим добавленные файлы:

7. [/python/python_header.h](https://github.com/AntonSHBK/example_cpp_python_wrapper/blob/main/python/python_header.h)

Здесь ничего особо важно, просто перечисляю некоторые функции (`add`), которых нет в основной библиотеке. Таким образом мы можем не только использовать нашу библиотеку на C++ но и описывать любую другую логику.

```cpp
#include "my_lib/my_include_file.h"
int add(const int &, const int &);
```

### 8. [/python/python_src.cpp](https://github.com/AntonSHBK/example_cpp_python_wrapper/blob/main/python/python_src.cpp)

Здесь описываются основные этапы обёртывания. Я не буду подробно останавливаться на этом, так как это отдельная тема, при необходимости можно найти множество примеров и обучающих материалов, описывающих реализацию совершенно разной  логики.

```cpp
#include <omp.h>
#include <pybind11/pybind11.h>
#include <my_lib/my_include_file.h>
#include "python_header.h"
// Используем пространство перченных pybind11
namespace py = pybind11;

// Некотная функция не из библиотеки my_lib
int add(const int &a, const int &b) {
    return a + b;
}

// основной описательный модуль: первый аргумент - название библиотеки, обязательно должно совпадать с названием в файле setup.py (это дальше)
PYBIND11_MODULE(CppToPython, m) {
    // Добавление функций: первый аргумент название функции к котрой будет обращение в Python, второй соответствующая функция C++
    m.def("add", &add); // добавили функцию выше
    m.def("bob", &talker::bob); // добавили функцию из mi_lib

    // Добавление класса
    py::class_<talker::SomeTalker>(m, "SomeTalker")
        .def(py::init<std::string>())
        .def("get_omp_max_treads", &talker::SomeTalker::get_omp_max_treads)
        .def("get_text_parallel", &talker::SomeTalker::get_text_parallel);
    // Ну добавление функций из библиотеки OpenMP для демонстрации работоспособности
    m.def("get_max_threads", &omp_get_max_threads, "Returns max number of threads");
    m.def("set_num_threads", &omp_set_num_threads, "Set number of threads");
};
```

### 9. [/setup.py](https://github.com/AntonSHBK/example_cpp_python_wrapper/blob/main/setup.py)

Подробно разбирать его не буду, так как большую часть его взята из [официального туториал](https://github.com/pybind/cmake_example/blob/master/setup.py). Опишу только некоторые изменения, которые на мой взгляд немного улучшают читаемость.

```python
# Импорты

# Метаинформация:

# Название библиотеки должно совпадать с описанной в исходнике
PACKAGE_NAME = "CppToPython"

VERSION = '0.1'
LICENSE = "MIT"
AUTHOR = "Pisarenko Anton"
EMAIL = "anton42@yandex.ru"
DESCRIPTION = "Testing lib"
LONG_DESCRIPTION = ""

# Описание CMake расширения с указанием пути до CMakeList.txt
class CMakeExtension(Extension):
    # код

# Описание сборки (практически не изменял)
class CMakeBuild(build_ext):
    def build_extension(self, ext: CMakeExtension) -> None:
        # код

# ну и сам результат
setup(
    name=PACKAGE_NAME,
    version=VERSION,
    author=AUTHOR,
    author_email=EMAIL,
    description=DESCRIPTION,
    long_description=LONG_DESCRIPTION,
    ext_modules=[CMakeExtension(PACKAGE_NAME)],
    cmdclass={"build_ext": CMakeBuild},
    zip_safe=False,
    python_requires=">=3.7",
)
```

10. [/CMakeLists.txt](https://github.com/AntonSHBK/example_cpp_python_wrapper/blob/main/CMakeLists.txt)

В заверении проектирования описываем наш `CMakeLists` верхнего уровня. Здесь мы перечисляем настройки для `pybind11` указываем все исходные файлы, а также добавляем все необходимые библиотеки, если их не указать сборка свалится с ошибкой (хотя сама библиотека собирается нормально, если кто знает как это правильнее сделать прошу сообщить об этом). 

```cmake
cmake_minimum_required(VERSION 3.25.1 FATAL_ERROR)

project(CppToPython VERSION 0.1.0 DESCRIPTION "CppToPython description")

# Стандарты
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Добавление исходников
set(
  PYTHON_SOURCES 
  python/python_header.h
  python/python_src.cpp
)

file (
  GLOB
  CPP_SOURCES 
  "cpp/src/*.cpp"
  "cpp/include/my_lib/*.h"
)

# Добавляем саму библиотеку
if (NOT TARGET cpp_code)
  add_subdirectory(cpp)
endif() 

# Также добавляем в include директорию
include_directories(cpp/include)

# Находим pybind11
find_package(pybind11 REQUIRED)

MESSAGE( [MAIN] "Found pybind11 v${pybind11_VERSION}: ${pybind11_INCLUDE_DIRS}")
MESSAGE( [Main] " pybind11_INCLUDE_DIRS = ${pybind11_INCLUDE_DIRS}")
MESSAGE( [Main] " pybind11_LIBRARIES = ${pybind11_LIBRARIES}")

# Параметры для сборки pybind11
pybind11_add_module(
  ${PROJECT_NAME}
  ${PYTHON_SOURCES}
  ${CPP_SOURCES}
)

# Также добавляем библиотеки, я не нашёл другого способа
find_package(OpenMP)

if(OpenMP_CXX_FOUND)
    target_link_libraries( ${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()
```

Также нам понадобится некоторый тестовый файл с вызовом наших функций в Python:
[python_test.py](https://github.com/AntonSHBK/example_cpp_python_wrapper/blob/main/python_test.py). Названия вызываемых функций должно совпадать с перечисленными названиями в файле `python_src.cpp`.

```python
import CppToPython
# Вызов функции из python_src.cpp
print(CppToPython.add(40, 10))
# Вызов функции из mi_lib
print(CppToPython.bob(40, 10))
# Обращение к библиотеке OpenMP напрямую
print(CppToPython.get_max_threads())

# Создание экземпляра класса из my_lib
this_talker = CppToPython.SomeTalker('Hello world!')
# Вызов метода класса
this_talker.get_text_parallel(4)
```

Окончательна структура проекта:
```
workspace.
│   CMakeLists.txt
│   python_test.py
│   setup.py
│
├───cpp
│   │   CMakeLists.txt
│   │
│   ├───include
│   │   └───my_lib
│   │           my_include_file.h
│   │
│   ├───src
│   │       my_cpp_file.cpp
│   │
│   └───tests
│       │   CMakeLists.txt
│       │
│       └───src
│               test_cpp.cpp
│
└───python
        python_header.h
        python_src.cpp
```
На этом проектирование проекта завершено и можно приступать к тестированию обёртки.

## Сборка и тестирование Python
Перед сборкой необходимо активировать необходимый интерпретатор, иначе будет использоваться по умолчанию системный. Сначала будет выполняться сборка библиотек на C++, а затем взаимодействие с Python. За нас все сделает `Python`:
```bush
pip3 install . --break-system-packages
```
Далее запускаем наш тестовый файл:
```bush
python3 python_test.py 
```
Вывод в консоль:
```
50
50
4
0Hello world!3Hello world!2Hello world!1Hello world!
```
Таким образом мы можем видеть что весь функционал работает, а также выполняется параллельный вывод информации.

## Вывод

Возможно описанный пример является чересчур синтетический и местами замудрённый, так как я его писал для себя как тестовая проба сборки подобных проектов. Здесь описаны основные моменты сборки рабочего проекта. На основе этого примера можно уже реализовать более сложную логику с использованием множества сторонних библиотек и сконцентрировать внимание на разработке логики нежели сталкиваться с проблемами сборки проекта. Надеюсь данная статья будет вам полезна. Также я привел в статье наиболее полезные на мой взгляд материала, которые мне помогли погрузиться в тему.