/**
  ******************************************************************************
  * \file    main.c
  * \author  Александр Смирнов
  * \version 1.0.0
  * \date    3.02.2023
  * \brief   Программа на языке C для учебного стенда на базе
  *          STM32F072RBT6 в среде разработки Keil uVision 5.
  *          Подключение библиотек поддержки МК STM32F072RBT6 осуществляется
  *          средствами IDE Keil через Run-Time Environment (RTE).
  *          В программе показаны основы синтаксиса языка C.
  ******************************************************************************
  */


/* Функция main - точка входа в программу.
   В любой программе на языке C должна быть одна функция main.
   В ней начинается исполнение программы. */
int main(void)
{
    /* Объявление и инициализация переменных */
    int a = 1;
    int b = 2;
    int c;

    /* Арифметические операторы */
    c = a + b;
    c = a + 10;
    c = a - b;
    c = a * b;
    c = 100 / b;
    c = a / b;

    /* Логические операторы */
    c = a & 2;
    c = a | 3;
    c = a ^ 1;
    c = ~b;
    c = a << 4;
    c = a >> 1;

    /* Массив */
    int arr[10];
    arr[0] = -1;
    arr[9] = 0;

    /* Условный оператор */
    if (a > 0)
    {
        c = 1;
    }
    else
    {
        c = -1;
    }

    /* Циклы */
    int i = 0;
    while (i < 10)
    {
        arr[i] = i;
        i++;
    }

    for (int j = 0; j < 10; j++)
    {
        arr[j] = 10 - j;
    }
}
