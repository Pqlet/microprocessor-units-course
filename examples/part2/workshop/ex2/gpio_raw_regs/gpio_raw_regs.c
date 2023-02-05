/**
  ******************************************************************************
  * \file    gpio_raw_regs.c
  * \author  Александр Смирнов
  * \version 1.0.0
  * \date    1.02.2022
  * \brief   Программа на языке C для учебного стенда на базе
  *          STM32F072RBT6 в среде разработки Keil uVision 5.
  *          Подключение библиотек поддержки МК STM32F072RBT6 осуществляется
  *          средствами IDE Keil через менеджер пакетов Run-Time Environment (RTE).
  *          Программа включает светодиод D1.
  *          Программа работает в режиме 0 учебного стенда (S1 = 0, S2 = 0).
  ******************************************************************************
  */

/* В этой программе используется обращение к регистра специальных функций
   по абсолютным адресам вместо библиотеки CMSIS. */


/* Для обращения к периферийным модулям по абсолютным адресам в языке C
   необходимо освоить очень важный элемент языка - указатели.
   По сути указатели это те же самые адреса в терминологии C. С помощью указателей можно
   производить адресную арифметику (из базового адреса вычислять другие адреса) и обращаться к любым
   областям памяти в адресном пространстве.
   Рассмотрим в начале простой пример:
   #define REG *((int *)(0x4000))
   В этом примере через директиву #define дается определение некого регистра REG.
   Это определение *((int *)(0x4000)) состоит из числа 0x4000 - адреса регистра в шестнадцатиричном формате.
   Нужно показать, что 0x4000 это не просто число, а адрес.
   Делается это с помощью операции приведение типов:
   (int *)(0x4000) - здесь символ * показывает то, что это указатель, а не число. Ключевое
   слово int то, что по этому адресу лежит целочисленное число со знаком.
   Чтобы обратиться по адресу мы должны заключить его в скобки с символом * в начале - *(pointer).
   Это операция в C называется разыменованием.
   В нашем примере это будет *((int *)(0x4000)).
   Теперь чтобы записать единицу по адресу 0x4000 можно написать так:
   *((int *)(0x4000)) = 1;
   Но чтобы не писать постоянно эту конструкцию использован define:
   REG = 1;
   В этом примере есть небольшие отличия в объявлении адреса и приведении типа:
   #define RCC_AHBENR      (*((volatile unsigned int *)(0x40021000 + 0x14)))
   Во-первых, (0x40021000 + 0x14) - это адрес базового регистра модуля (RCC) + смещение конкретного
   регистра (AHBENR) относительно базового. Каждый периферийный модуль имеет несколько регистров.
   Все они располагаются друг за другом начиная с какого-то базового адреса. В техническом описании адреса
   регистров приводятся именно так - базовый адрес + смещение.
   Во-вторых, в приведение типов (volatile unsigned int *) добавлены два ключевых слова.
   unsigned - указывает на то, что число по адресу беззнакового типа. И действительно знак (самый
   старший бит) во всех регистрах управления периферийными модулями не имеет смысла.
   Использование знаковой переменной при обращении к регистрам может создавать потенциальные ошибки
   со старшим битом.
   volatile - указывает компилятору на то, что каждый раз когда в коде будет производится чтение
   по данному адресу необходимо именно читать эту ячейку, а не использовать копию из регистра центрального
   процессора. Современные компиляторы стараются оптимизировать программы и снизить количество
   обращений к памяти, так как эти операции медленные по сравнению с операциями на внутренних регистрах
   центрального процессора. Поэтому они пытаются создать как можно больше
   копий переменных во внутренних регистрах центрального процессора. Но периферийные модули работают
   независимо от центрального процессора и соответственно содержимое регистров изменяются независимо.
   Если будет использоваться копия, то невозможны такие операции как проверка флага в цикле,
   потому что всегда будет проверяться старое значение регистра.
   Так так язык С это высокоуровневый язык, то конструкции языка для обращения к регистрам
   на первый взгляд перегружены и трудновоспринимаемы. Но это только в начале знакомства с языком.
   После получения определённого опыта работы с указателями эти конструкции будут казаться элементарными */
/* Обращение к регистру разрешения тактирования
   AHB peripheral clock enable register (RCC_AHBENR) через указатель */
#define RCC_AHBENR      (*((volatile unsigned int *)(0x40021000 + 0x14)))

/* Обращение к регистру режима порта C
   GPIO port mode register (MODER) через указатель */
#define GPIOC_MODER     (*((volatile unsigned int *)(0x48000800 + 0x00)))

/* Обращение к регистру выходных данных порта C
   GPIO port output data register (ODR) через указатель */
#define GPIOC_ODR       (*((volatile unsigned int *)(0x48000800 + 0x14)))

/* Функция main - точка входа в программу. */
int main(void)
{
    /* Инициализация периферийных модулей. */

    /* Включение тактирования модулей. */
    RCC_AHBENR = RCC_AHBENR | 0x00080000;

    /* Настройка на вывод линии PC0 (D1) */
    GPIOC_MODER = GPIOC_MODER | 0x00000001;

    /* Включить светодиод PC0 (D1). */
    GPIOC_ODR = GPIOC_ODR | 0x00000001;

    /* Бесконечный цикл. */
    while (1)
    {
    }
}
