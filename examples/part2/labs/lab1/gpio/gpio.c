/**
  ******************************************************************************
  * \file    gpio.c
  * \author  Александр Смирнов
  * \version 1.0.0
  * \date    2.02.2023
  * \brief   Программа на языке C для учебного стенда на базе
  *          STM32F072RBT6 в среде разработки Keil uVision 5.
  *          Подключение библиотек поддержки МК STM32F072RBT6 осуществляется
  *          средствами IDE Keil через менеджер пакетов Run-Time Environment (RTE).
  *          Условие:
  *          Разработать программу, которая осуществляет
  *          периодическое включение и выключения светодиода D1.
  *          Программа работает в режиме 0 учебного стенда (S1 = 0, S2 = 0).
  ******************************************************************************
  */

/* Подключение заголовочного файла с макроопределениями всех регистров специальных
   функций МК STM32F072RBT6. */
#include <stm32f0xx.h>

/* Функция программной временной задержки */
void software_delay(int ticks)
{
    while (ticks > 0)
    {
        ticks = ticks - 1;
    }
}

/* Функция main - точка входа в программу */
int main(void)
{
    /* Инициализация периферийных модулей */

    /* Включение тактирования порта C */
    RCC->AHBENR = RCC->AHBENR | RCC_AHBENR_GPIOCEN;

    /* Настройка на вывод линии PC0 (D1).
       Режим линии задается парой бит. Режим вывод два бита - 01.
       То есть, чтобы настроить на вывод линию PC1 необходимо записать в регистр
       число 4.
       */
    GPIOC->MODER = GPIOC->MODER | 1;

    /* Бесконечный цикл */
    while (1)
    {
        /* Включить светодиод PC0 (D1) */
        GPIOC->ODR = GPIOC->ODR | 1;

        /* Программная временная задержка */
        software_delay(1000000);

        /* Выключить светодиод PC0 (D1) */
        GPIOC->ODR = GPIOC->ODR & ~1;

        /* Программная временная задержка */
        software_delay(1000000);
    }
}
