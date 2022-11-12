/**
  ******************************************************************************
  * \file    main.c
  * \author  Александр Смирнов
  * \version 1.0.0
  * \date    1.02.2022
  * \brief   Программа на языке C для учебного стенда на базе
  *          STM32F072RBT6 в среде разработки Keil uVision 5.
  *          Подключение библиотек поддержки МК STM32F072RBT6 осуществляется
  *          средствами IDE Keil через Run-Time Environment (RTE).
  *          Программа включает светодиод D1.
  *          Программа работает в режиме 0 учебного стенда (S1 = 0, S2 = 0).
  ******************************************************************************
  */

/* Подключение заголовочного файла с макроопределениями всех регистров специальных
   функций МК STM32F072RBT6. Описание содержит адреса регистров, маски и смещения.
   Имена макроопределений соответствует именам в техническом описании (Datasheet
   и Reference Manual).
   Пример такого макроопределения - GPIOC. Если его раскрыть (через множество
   промежуточных макроопределений) то это будет не что иное как абсолютный адрес
   начала регистров специальных функций порта C. Обращаясь к этому адресу
   со смещением (здесь это реализовано через структуры, например GPIOC->ODR)
   можно обращаться к любому регистру этого порта.
   Данные описания входят в библиотеку Cortex-M -
   Common Microcontroller Software Interface Standard (CMSIS).
   Все современные МК с процессором Cortex-M имеют подобное описание.
   За подключение других файлов отвечает директива препроцессора #include */
#include <stm32f0xx.h>

/* Функция main - точка входа в программу.
   Если опустить детали, то именно с этой функции начинается работа
   программы после ее запуска. В случае встраиваемой системы именно
   она начинает исполняться после выхода МК из состояния сброса.
   Функция с таким названием должна быть только одна на весь проект.*/
int main(void)
{
    /* Инициализация периферийных модулей.
       Перед началом использования любого периферийного модуля его необходимо
       инициализировать, то есть настроить.
       После сброса МК все периферийные модули как правило выключены.
       Режимы альтернативных функций отключены и все линии МК подключены к
       портам ввода/вывода общего назначения (GPIO).
       При этом сам порт настроен на ввод. */

    /* При разработке учебных программ важно иметь под рукой два документа:
         * функциональную или принципиальную схему стенда
         * техническое описание МК (Reference Manual)
       Функциональная схема нужна для определения куда и какие устройства подключены.
       Например, по этой схеме определяется, что светодиод D1 подключен
       к линии 0 порта C (PC0) и подключен анодом к порту, то есть выход
       порта должен подключить анод к питанию, чтобы он засветился.
       Следует отметить, что функциональная схема значительно упрощена
       по сравнению с принципиальной схемой. Назначение функциональной схемы -
       это быстрое получение информации о подключенных к МК устройств.
       Но если например обратиться к принципиальной схеме и посмотреть
       как подключается D1 к МК, то можно увидеть промежуточный инвертирующий
       буфер. Наличие этого промежуточного буфера не влияет на написание кода,
       поэтому он в функциональной схеме опущен.
       В техническом описании содержится информация о том в
       каких регистрах и в какой последовательности нужно установить биты,
       чтобы настроить модуль нужным образом.
    */

    /* Включение тактирования модулей.
       Практически все современные МК имеют возможность управлять тактированием
       каждого модуля. При выключенном тактировании модуль не потребляет
       электроэнергию. После сброса МК тактирование отключено практически
       у всех модулей, в том числе и у порта ввода вывода.
       В строчке кода ниже производится включение тактирования порта А.
       Для включения тактирования порта A необходимо в регистре
       AHB peripheral clock enable register (RCC_AHBENR) записать 1
       в позицию 19 (в девятнадцатый бит IOPCEN).
       В начале читается состояние регистра (RCC->AHBENR), затем
       с помощью операции побитового ИЛИ (оператор | в языке  С) устанавливается
       в 1 бит номер 19 (число 0x00080000 - в этом числе 1 в 19 бите), и результат
       записывается вновь в регистр RCC->AHBENR.
       Эту строку можно написать аналогичным образом:
       RCC->AHBENR = RCC->AHBENR | RCC_AHBENR_GPIOCEN;
       Макроопределение RCC_AHBENR_GPIOCEN находится файле stm32f0xx.h и означает также 0x00080000.
       Такие макроопределения позволяют перейти от чисел к осмысленным названиям.
       */
    RCC->AHBENR = RCC->AHBENR | 0x00080000;

    /* Настройка на вывод линии PC0 (D1).
       Порты по сбросу настроены на ввод.
       Согласно Reference Manual для перевода линии PC0 в режим вывода
       (Push/Pull) необходимо установить бит 0 в 1 в регистре
       GPIO port mode register (MODER) (маска 0x00000001).
       Эту строку можно написать аналогичный образом:
       GPIOC->MODER = GPIOC->MODER | GPIO_MODER_MODER0_0;
       Макроопределение GPIO_MODER_MODER0_0 находится файле stm32f0xx.h и означает также 0x0000000й */
    GPIOC->MODER = GPIOC->MODER | 0x00000001;

    /* Включить светодиод PC0 (D1).
       Согласно Reference manual для управления портом вывода необходимо
       осуществлять запись в регистр GPIO port output data register (ODR).
       При этом 0 - низкий активный уровень (Vss), 1 - высокий (Vdd) на
       выходе соответствующей линии порта.
       Согласно функциональной схеме, чтобы включить светодиод нужно
       подключить к его анод к питанию (Vdd), то есть записать в PC0 значение 1.
       Установка осуществляется с помощью маски 0x00000001 (все нули кроме
       нулевого бита) и операции побитового ИЛИ (оператор |).
       */
    GPIOC->ODR = GPIOC->ODR | 0x00000001;

    /* Бесконечный цикл.
       Программа на языке C для ПК с операционной системой (ОС) может не иметь
       бесконечного цикла в конце. После завершения работы программы для ПК
       управление возвращается ОС и центральный процессор ПК начинает
       вычислять другие программы. В программе для МК возвращать управление
       некуда. Если не установить эту заглушку, МК начнет исполнять команды
       находящиеся в памяти за программой. В лучшем случаем это будет пустая
       память (заполненная 0xFF или 0x00), в худшем случае обрывки другой
       программы, которая была ранее загружена в МК и которая была больше
       этой программы. Если МК "провалится" туда, то поведение системы будет
       непредсказуемым.
       В языке ассемблера ARM также применялась похожая конструкция:
       STOP    B STOP
       Полным аналогом на языке C является:
       for(;;);
    */
    while (1)
    {
    }
}
