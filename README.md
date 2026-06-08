# Lab05 Tutorial
В ходе выполнения tutorial была создана лабораторная работа `lab06` на основе предыдущего репозитория `lab04`.

---
Начальные настроечки и подготовки для выполнения:
```bash
cd "/home/vboxuser/Рабочий стол/project/projects"

rm -rf lab06

git clone https://github.com/edenabr/lab04.git lab06

cd lab06

git remote remove origin

git remote add origin https://github.com/edenabr/lab06.git

git config --global user.name "edenabr"

git config --global user.email "edenabramova@gmail.com"

git branch -M main

alias gsed=sed
```

Была создана новая локальная папка `lab06` на основе репозитория `lab04`.

Команда:

```bash
git clone https://github.com/edenabr/lab04.git lab06
```
После этого старый удалённый репозиторий `origin`, который указывал на `lab04`, был удалён:

```bash
git remote remove origin
```

Затем был добавлен новый удалённый репозиторий:

```bash
git remote add origin https://github.com/edenabr/lab06.git
```

Команда:

```bash
alias gsed=sed
```
используется для совместимости с командами tutorial, где применяется `gsed`.


---

```bash
mkdir -p third-party

git submodule add https://github.com/google/googletest third-party/gtest

cd third-party/gtest

git checkout release-1.8.1

cd ../..

git add third-party/gtest .gitmodules

git commit -m "added gtest framework"
```

Была создана директория `third-party`, предназначенная для сторонних библиотек.

С помощью команды:

```bash
git submodule add https://github.com/google/googletest third-party/gtest
```

в проект был добавлен Google Test как Git submodule.

После этого была выбрана версия:

```bash
git checkout release-1.8.1
```


```bash
git commit -m "added gtest framework"
```



---
Добавление BUILD_TESTS, которая позволяет включать или отключать сборку тестов.

```bash
gsed -i '/option(BUILD_EXAMPLES "Build examples" OFF)/a\
option(BUILD_TESTS "Build tests" OFF)
' CMakeLists.txt
```

После этого в `CMakeLists.txt` появилась опция:

```cmake
option(BUILD_TESTS "Build tests" OFF)
```

Чтобы включить тесты при сборке, используется параметр:

```bash
-DBUILD_TESTS=ON
```


---
Добавление блока тестов в CMakeLists.txt
```bash
cat >> CMakeLists.txt <<'EOF'

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(third-party/gtest)
  file(GLOB ${PROJECT_NAME}_TEST_SOURCES tests/*.cpp)
  add_executable(check ${${PROJECT_NAME}_TEST_SOURCES})
  target_link_libraries(check ${PROJECT_NAME} gtest_main)
  add_test(NAME check COMMAND check)
endif()
EOF
```

В конец `CMakeLists.txt` был добавлен блок для сборки тестов.


---
Создание каталога tests и первого теста


```bash
mkdir -p tests
```

Был создан каталог `tests`, в котором хранятся файлы с тестами.

Сначала по tutorial был создан тест для `print.hpp`:

```bash
cat > tests/test1.cpp <<'EOF'
#include <print.hpp>

#include <gtest/gtest.h>

TEST(Print, InFileStream)
{
  std::string filepath = "file.txt";
  std::string text = "hello";
  std::ofstream out{filepath};

  print(text, out);
  out.close();

  std::string result;
  std::ifstream in{filepath};
  in >> result;

  EXPECT_EQ(result, text);
}
EOF
```

Однако при сборке возникла ошибка, потому что в текущем проекте нет файла `print.hpp`.

Ошибка:

```text
fatal error: print.hpp: Нет такого файла или каталога
```

---
Тест под наш проект:

```bash
cat > tests/test1.cpp <<'EOF'
#include <solver.h>

#include <gtest/gtest.h>

TEST(Solver, QuadraticEquation)
{
  float x1 = 0;
  float x2 = 0;

  solve(1, 2, 1, x1, x2);

  EXPECT_FLOAT_EQ(x1, -1);
  EXPECT_FLOAT_EQ(x2, -1);
}
EOF
```

Вместо теста для `print.hpp` был создан тест для функции `solve` из библиотеки `solver`.

Тест проверяет решение квадратного уравнения:

```text
x² + 2x + 1 = 0
```
Проверка выполняется с помощью макросов Google Test:

```cpp
EXPECT_FLOAT_EQ(x1, -1);
EXPECT_FLOAT_EQ(x2, -1);
```

---

CMake:
```bash
python3 - <<'PY'
from pathlib import Path

p = Path("CMakeLists.txt")
text = p.read_text()

start = text.find("if(BUILD_TESTS)")
end = text.find("endif()", start)

new_block = """if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(third-party/gtest)
  file(GLOB TEST_SOURCES tests/*.cpp)
  add_executable(check ${TEST_SOURCES})
  target_link_libraries(check solver gtest_main)
  add_test(NAME check COMMAND check)
endif()"""

if start != -1 and end != -1:
    end += len("endif()")
    text = text[:start] + new_block + text[end:]

p.write_text(text)
PY
```


---

Переименование:

```bash
sed -i 's/project(lab03)/project(lab06)/g' CMakeLists.txt
```

Так как проект был создан на основе предыдущей лабораторной работы, в `CMakeLists.txt` осталось имя:

```cmake
project(lab03)
```

Оно было заменено на:

```cmake
project(lab06)
```

---

Исправление ошибки со sqrt

```bash
sed -i 's/std::sqrtf/std::sqrt/g' solver_lib/solver.cpp

grep -q '#include <cmath>' solver_lib/solver.cpp || sed -i '1i #include <cmath>' solver_lib/solver.cpp
```

В файле `solver_lib/solver.cpp` была исправлена ошибка, связанная с математической функцией.

Изначально использовалась функция:

```cpp
std::sqrtf
```
Была изменена на:

```cpp
std::sqrt
```

Также в начало файла было добавлено подключение:

```cpp
#include <cmath>
```

Эта библиотека нужна для использования функции `std::sqrt`.

---
справление ошибки сборки Google Test на современном GCC:
#ИСПОЛЬЗОВАЛАСЬ ПОМОЩЬ ГПТ!

При сборке Google Test версии `release-1.8.1` возникла ошибка:

```text
error: ‘dummy’ may be used uninitialized [-Werror=maybe-uninitialized]
cc1plus: all warnings being treated as errors
```

Причина ошибки: старая версия Google Test собиралась на современном компиляторе GCC 15, который выдаёт предупреждение `maybe-uninitialized`. Из-за флага `-Werror` это предупреждение воспринималось как ошибка, и сборка останавливалась.

Для исправления в `CMakeLists.txt` был добавлен блок:

```bash
cat >> CMakeLists.txt <<'EOF'

# Disable warnings-as-errors for old GoogleTest on modern GCC
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  foreach(_target gtest gtest_main gmock gmock_main)
    if(TARGET ${_target})
      target_compile_options(${_target} PRIVATE -Wno-error)
    endif()
  endforeach()
endif()
EOF
```

Этот блок отключает обработку предупреждений как ошибок только для целей Google Test:

```text
gtest
gtest_main
gmock
gmock_main
```

Это не изменяет код самой библиотеки Google Test и позволяет собрать старую версию `release-1.8.1` на новом компиляторе.

---

СБорка + тесты:

```bash
rm -rf _build

cmake -H. -B_build -DBUILD_TESTS=ON

cmake --build _build
```

Команда:

```bash
cmake -H. -B_build -DBUILD_TESTS=ON
```

конфигурирует проект и включает сборку тестов.

После этого команда:

```bash
cmake --build _build
```

собирает проект, Google Test и исполняемый файл тестов `check`.


---

Запуск тестов<

```bash
cmake --build _build --target test
```

Команда запускает тесты через стандартную цель CMake `test`.

```bash
cmake --build _build --target test -- ARGS=--verbose
```

Флаг:

```bash
ARGS=--verbose
```

показывает более подробный вывод выполнения тестов.


---
Добавление изменений в Git:

```bash
git status

git add CMakeLists.txt README.md tests

git add .travis.yml 2>/dev/null || true

git add .github/workflows/ci.yml 2>/dev/null || true

git add -p

git commit -m "added tests"
```

Команда:

```bash
git status
```

После этого был создан коммит:

```bash
git commit -m "added tests"
```
---
Отправка:
```bash
git push -u origin main
```

Команда отправляет локальную ветку `main` в удалённый репозиторий:

```text
https://github.com/edenabr/lab06.git
```

---

# Подтверждения выполнения:
<pre>
[ 10%] Linking CXX static library libgtest.a
[ 10%] Built target gtest
[ 15%] Building CXX object third-party/gtest/googlemock/gtest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o
[ 20%] Linking CXX static library libgtest_main.a
[ 20%] Built target gtest_main
[ 25%] Building CXX object solver_lib/CMakeFiles/solver.dir/solver.cpp.o
[ 30%] Linking CXX static library libsolver.a
[ 30%] Built target solver
[ 35%] Building CXX object CMakeFiles/check.dir/tests/test1.cpp.o
[ 40%] Linking CXX executable check
[ 40%] Built target check
[ 45%] Building CXX object formatter_lib/CMakeFiles/formatter.dir/formatter.cpp.o
[ 50%] Linking CXX static library libformatter.a
[ 50%] Built target formatter
[ 55%] Building CXX object formatter_ex_lib/CMakeFiles/formatter_ex.dir/formatter_ex.cpp.o
[ 60%] Linking CXX static library libformatter_ex.a
[ 60%] Built target formatter_ex
[ 65%] Building CXX object hello_world_application/CMakeFiles/hello_world.dir/hello_world.cpp.o
[ 70%] Linking CXX executable hello_world
[ 70%] Built target hello_world
[ 75%] Building CXX object solver_application/CMakeFiles/equation.dir/equation.cpp.o
[ 80%] Linking CXX executable equation
[ 80%] Built target equation
[ 85%] Building CXX object third-party/gtest/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o
[ 90%] Linking CXX static library libgmock.a
[ 90%] Built target gmock
[ 95%] Building CXX object third-party/gtest/googlemock/CMakeFiles/gmock_main.dir/src/gmock_main.cc.o
[100%] Linking CXX static library libgmock_main.a
[100%] Built target gmock_main</pre>

<pre>
UpdateCTestConfiguration  from :/home/vboxuser/Рабочий стол/project/projects/lab06/_build/DartConfiguration.tcl
UpdateCTestConfiguration  from :/home/vboxuser/Рабочий стол/project/projects/lab06/_build/DartConfiguration.tcl
Test project /home/vboxuser/Рабочий стол/project/projects/lab06/_build
Constructing a list of tests
Done constructing a list of tests
Updating test list for fixtures
Added 0 tests to meet fixture requirements
Checking test dependency graph...
Checking test dependency graph end
test 1
    Start 1: check

1: Test command: /home/vboxuser/Рабочий\ стол/project/projects/lab06/_build/check
1: Working Directory: /home/vboxuser/Рабочий стол/project/projects/lab06/_build
1: Test timeout computed to be: 10000000
1: Running main() from /home/vboxuser/Рабочий стол/project/projects/lab06/third-party/gtest/googletest/src/gtest_main.cc
1: [==========] Running 1 test from 1 test case.
1: [----------] Global test environment set-up.
1: [----------] 1 test from Solver
1: [ RUN      ] Solver.QuadraticEquation
1: [       OK ] Solver.QuadraticEquation (0 ms)
1: [----------] 1 test from Solver (0 ms total)
1: 
1: [----------] Global test environment tear-down
1: [==========] 1 test from 1 test case ran. (0 ms total)
1: [  PASSED  ] 1 test.
1/1 Test #1: check ............................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 1 </pre>

<pre>
Running main() from /home/vboxuser/Рабочий стол/project/projects/lab06/third-party/gtest/googletest/src/gtest_main.cc
[==========] Running 1 test from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from Solver
[ RUN      ] Solver.QuadraticEquation
[       OK ] Solver.QuadraticEquation (0 ms)
[----------] 1 test from Solver (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test case ran. (0 ms total)
[  PASSED  ] 1 test.</pre>
