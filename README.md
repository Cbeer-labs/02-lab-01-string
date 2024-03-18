# Лабораторная работа № 1

## Цель
Главная цель данной лабораторной работы научиться создавать пользовательские классы. На простом
примере следует изучить как устроены класс, как создавать методы класса, что такое инкапсуляция,
правильно реализовывать конструкторы копирования, операторы присваивания и деструктор. 

## Теоретическая справка
Класс представляет составной тип. Класс в С++ сильно напоминает структуру в С. Более того, класс
и структура в С++, ничем практически не отличатся. Единственное отличие в модификаторах доступа по
умолчанию. В классе по умолчанию все поля закрытые (`private`), в структурах открытые (`public`)  

Следует различать понятия класс и объект (экземпляр класса)

**Класс** описывает свойства и методы, которые будут доступны у **объекта**, построенного по
описанию, которое заложено в классе:
```cpp
class MyClass {
...
};
```

**Объект** представляет конкретное воплощение класса:
```cpp
MyClass my_object;
```

### Поля класса

Класс представляет составной тип. Класс, как и структура, может содержать дополнительные поля, члены
класса.

Члены класса имеют разный модификатор доступа: `private`, `public`, `protected`.

- `private` - член класса доступен только самому классу. Из вне доступа к этому полю нет. Если
попытаться обратиться к закрытому полю, в коде вашей программы, то возникнет **ошибка времени
компиляции**.
- `public` - член класса доступен как самому классу, так внешнему по отношению к классу коду.
- `protected` - будет рассмотрен в теме про наследование.

### Friend
Дружественная (`friend`) функция — это функция, которая не является членом класса, но имеет доступ
к членам класса, объявленным в полях private или protected.

Дружественные функции (или классы) нужно использовать в нескольких случаях, а именно только в случае
**сильной связности** компонент. Чаще всего это два случая:
- unit-test. Проверять иногда требуется и те только public методы. Для этого тест объявляется дружественным
- сильная связность компонентов/классов. Например, итератор и контейнер. Итератор без контейнера
вряд ли может существовать. И поэтому итератор иногда (не всегда) можно объявлять как дружественный
к контейнеру.

**Никогда** не используйте `friend` чтобы нарушить инкапсуляцию.

### Указатель `this`
Ключевое слово `this` представляет указатель на текущий **объект**, к которому был применён метод.
Соответственно через `this` мы можем обращаться внутри класса к любым его членам (зачастую это НЕ
требуется делать). Явное использование `this` затрудняет понимание вашего кода. Использовать
`this` явно советуется только при разработке шаблонов класса и в ограниченном случае.

**ВАЖНО**

Избегайте ситуаций следующего вида:
```cpp
class Point {
  float x;
  float y;
  Point(float x, float y) {
    this->x = x;
    this->y = y;
  }
};
```
В примере выше имена полей класса и имена аргументов совпадают. И разработчику приходится явно
использовать ключевое слово `this`, чтобы решить неясность в коде кто кому присвоится. Использование
слова `this` не является идеальным решением проблемы. Лучшее решение - избавиться от дублирующих
имен, например довольно частым способом решения этой проблемы есть добавление нижних прочерков к
названиям полей классов.

### Методы класса
Чтобы взаимодействовать с классом используют методы класса - функции, которые имеют доступ к
закрытым полям класса.

Методы бывают **константными** и **не константными**. Константный метод не может изменять поля
класса. Такие методы могут только читать поля (исключением составляют `mutable` поля).

Если концептуально метод не должен изменять класс, то его следует объявлять константным! Например,
методы `size()` у класса `std::string` не должен никак менять саму строку, поэтому он объявляется
константным:
```cpp
size_t string::size() const;
```
Неконстантные методы не могут быть вызваны по отношению к константным объектам. В случае, если все
методы вашего класса были объявлены неконстантными, будь то из-за лени, непонимания или иных причин,
использовать их будет невозможно.

В каждый *не статический* метод класса первым аргументом **неявно** передается указатель на текущий
объект (тот самый `this`). Именно благодаря этому, программа понимают к какому именно объекту идет
обращение.

```cpp
// первым аргументом неявно (поэтому и закомментирован) передается неизменяемый
// указатель на объект класса.
void string::push_back( /* std::string* const this, */ char ch ); 
```

*Замечание*

Отметим, что `this` - это неизменяемый указатель (`MyClass *const`), а не указатель на неизменяемый
объект (`const MyClass*`).

Кстати, если метод является константным, то `this` будет неизменяемым указателем на константный
объект (`const MyClass* const`)

### Конструкторы
Конструктор - специальный метод, который вызывается при **создании** объекта класса. В нем принято
инициализировать поля класса.

По сути конструктор представляет функцию, которая может принимать параметры, и которая **должна
называться по имени класса**.

Есть несколько видов конструкторов:
1. конструктор по умолчанию (не принимает никаких аргументов) `string::string()`
2. конструктор копирования (принимает константную ссылку на объект такого же класса) `string::string(const string&)`
3. конструктор перемещения (принимает rvalue ссылку на объект такого же класса) `string::string(string&&)`.
4. пользовательский конструктор. Объявляется самим разработчиком, принимает любое количество аргументов и разных типов.

Компилятор может генерировать самостоятельно первые три конструктора (не факт, что он сделает это
корректно).
Если в классе определен хотя бы один пользовательский конструктор, то компилятор не станет
генерировать конструкторы. Подробнее смотри **правило трёх** и **правило пяти**!

### Список инициализации
Подробнее про список инициализации можно прочитать по ссылке -
https://ravesli.com/urok-117-spisok-initsializatsii-chlenov-klassa/
Здесь мы отметим, что **предпочтительнее** использовать список инициализации полей класса, а не
инициализация в теле конструктора.

Таким образом, **используйте** вариант
```cpp
Student(const std::string& name, const std::string& group_id)
  : name_(name), group_id_(group_id)
{}
```

Вариант с копированием в теле, **менее** предпочтительный
```cpp
Student(const std::string& name, const std::string& group_id) {
  name_ = name;
  group_id_ = group_id;
}
```

### Деструктор
Деструктор - специальный метод, который вызывается при **уничтожении** объекта. В деструкторе
требуется освобождать выделенные ресурсы, которые требовались для работы объекта, например,
динамическая память и прочие ресурсы, требующие освобождения.

*На заметку студенту*

Студенты очень любят писать следующий код:
```cpp
~MyClass() {
  var_a = 0;
  var_b = 0;
  ptr = nullptr;
  // ...
}
```
Т.е. студенты по какой-то причине считают, что есть скрытый смысл в обнулении полей класса. В
большинстве случае это лишь захламляет код. Не надо так делать, без сильных обоснований.

### Перегрузка операторов
В качестве "синтаксического сахара" в С++ разрешена перегрузка операторов класса. Чаще всего к
перегрузке прибегают для удобства разработчика. Правда же соединить две строки удобнее и
выразительнее через `оператор +`, чем прибегать к какому-то специальному методу.

В перегрузках операторов особняком стоят перегрузка присваивания `operator=`.

#### Перегрузка присваивания
Эти операторы вызываются, когда разработчику требуется присвоить один объект другому.
```cpp
name_one = name_two;
```

Есть два вида таких операторов:
1. Оператор присваивания (или копирования) - присваивает правый операнд левому. 
Принимает константную ссылку на объект. Возвращает ссылку на левый операнд.
```cpp
string& operator=(const string& oth);
```
Возвращает ссылку на левый операнд, чтобы возможно было написать код вида `a = b = c = d;`.

Так же в реализации этого оператора принято проверять, что левый и правый операнды не являются одним
и тем же объектом.
```cpp
string name;
string& ref = name;
name = ref;
```

Типичный оператор присваивания выглядит так:
```cpp
ClassName& operator=(const ClassName& rhs) {
  if (&rhs == this) {
    return *this;
  }
  // реализация присваивания
  // pointer = new PointerType[buffer_size];
  // std::copy(rhs.pointer, pointer, buffer_size);
  ...
  return *this;
}
```

2. Оператор перемещения - перемещает правый операнд в левый, при этом уничтожая правый операнд.
Полезно для избежания лишнего копирования данных без передачи ссылки. Можно считать, что левый
операнд становится новым владельцем динамической памяти, если таковая имеется:
```cpp
std::string new_string = std::move(old_string);
```
Типичный оператор перемещения выглядит так:
```cpp
ClassName& operator=(ClassName&& rhs) {
  if (&rhs == this) {
    return *this;
  }
  // реализация перемещения
  // pointer = rhs.pointer;
  // rhs.pointer = nullptr;
  ...
}
```

**ВАЖНО**

Операторы присваивания так же могут быть сгенерированы компилятором (не факт, что корректно). Про
генерацию операторов компилятором читайте **правило трёх** и **правило пяти**!

#### Перегрузка сравнения и равенства
Для многих классов применимо свойство равенства. Задавать отношение равенства в C++ принято при
помощи оператора `bool operator==`. Реализация этого оператора тривиальна, если задание равенства
возможно.

Также для некоторых классов применимо свойство упорядоченности. Довольно частая задача при
программировании -- сортировка или поиск минимума (максимума) на множестве элементов. Эта задача
выполнима только если на интересующем множестве задан какой-то порядок.

Порядок на множестве можно задать, если определить оператор `bool operator<`. При этом все
остальные операторы сравнения будут сгенерированы компилятором автоматически:
- `bool operator>(const auto& lhs, const auto& rhs) { return rhs < lhs; }`
- `bool operator<=(const auto& lhs, const auto& rhs) { return !(rhs < lhs); }`
- `bool operator>=(const auto& lhs, const auto& rhs) { return !(lhs < rhs); }`
Заметим, что если такой порядок не применим к определяемому классу, эти операторы можно
самостоятельно переопределить.

А теперь вспомним математику.
Множество может быть:
- частично упорядоченным (когда некоторые элементы могут быть несравнимы)
- слабо упорядоченным (когда при `a == b`, может быть `f(a) != f(b)`)
- полностью упорядоченным (когда между элементами возможно отношение эквивалентности)

В C++20 эти упорядоченности представлены типами из `<compare>` соответственно:
- [std::partial_ordering](https://en.cppreference.com/w/cpp/utility/compare/partial_ordering)
- [std::weak_ordering](https://en.cppreference.com/w/cpp/utility/compare/weak_ordering)
- [std::strong_ordering](https://en.cppreference.com/w/cpp/utility/compare/strong_ordering)
Возможно задать порядок на множестве объектов класса при помощи `operator<=>`. В отличии от
остальных операторов сравнения, этот оператор может возвращать разные типы значений, а именно
перечисленные выше типы упорядоченности. В самих этих типах определены константы, обозначающие
отношение между типами (например `std::partial_ordering::less`)

Задача оператора `operator<=>` приводит к автоматической генерации остальных операторов сравнения,
что особенно удобно, когда реализуемый класс имеет свойство упорядоченности на основании
упорядоченности низлежащих типов (как, например, в случае строки -- `strcmp`). Зачастую, реализация
операторов сравнения в таких ситуациях сводится к реализации оператора `operator<=>` через
аналогичный(-ые) операторы типов-полей класса или достаточно тривиальный `switch`

#### Перегрузка остальных операторов
В перегрузке операторов для класса нет ничего сложного или необычного. Оставим на самостоятельное
рассмотрение - https://metanit.com/cpp/tutorial/5.14.php

Единственное замечание, которое необходимо дать. Не стоит злоупотреблять перегрузкой операторов.
И не надо использовать операторы класса для специфичных задач. Т.е. если вы реализуете свой класс и
для него концептуально не применимы различные операторы, то не надо их перегружать. Например, для
класса "строки" сложно определить оператор вычитания, поэтому не надо реализовывать этот оператор.
Как использовать перегруженные вами операторы для класса должно быть понятно не только вам, но и
вашим коллегам. Поэтому настоятельно просим избегать ситуаций, когда только вы знаете, что делает
перегруженный оператор.   

## Задание
1. Реализуйте класс `String`. Интерфейс класса объявлен в [string.hpp](include/string.hpp).
2. Реализованный класс должен проходить [unit-тесты](tests/string_unittest.cpp)

## Полезные ссылки
* https://metanit.com/cpp/tutorial/5.1.php
* https://metanit.com/cpp/tutorial/5.2.php
* https://metanit.com/cpp/tutorial/5.3.php
* https://metanit.com/cpp/tutorial/5.4.php
* https://metanit.com/cpp/tutorial/5.5.php
* https://metanit.com/cpp/tutorial/5.6.php
* https://metanit.com/cpp/tutorial/5.7.php
* https://metanit.com/cpp/tutorial/5.13.php
* https://metanit.com/cpp/tutorial/5.14.php
* https://metanit.com/cpp/tutorial/5.15.php
