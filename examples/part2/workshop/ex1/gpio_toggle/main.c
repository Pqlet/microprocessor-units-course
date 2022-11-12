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
  *          В верхнем положении ключа SW1 включается светодиод D16,
  *          в нижнем светодиод D16 выключается.
  *          Программа работает в режиме 0 учебного стенда (S1 = 0, S2 = 0).
  ******************************************************************************
  */

/* Подключение заголовочного файла с макроопределениями всех регистром специальных
   функций МК STM32F072RBT6. */
#include <stm32f0xx.h>

/* Функция main - точка входа в программу */
int main(void)
{
    /* Инициализация периферийных модулей */

    /* Включение тактирования модулей */
    RCC->AHBENR = RCC->AHBENR | RCC_AHBENR_GPIOAEN; /* Включение тактирования порта A */
    RCC->AHBENR = RCC->AHBENR | RCC_AHBENR_GPIOCEN; /* Включение тактирования порта C */

    /* Настройка на вывод линии PC15 (D16) */
    GPIOC->MODER = GPIOC->MODER | GPIO_MODER_MODER15_0;

    /* Включение подтягивающего резистора линии PA11 (SW1) */
    GPIOA->PUPDR = GPIOA->PUPDR | GPIO_PUPDR_PUPDR11_0;

    /* Бесконечный цикл */
    while (1)
    {
        /* Объявление переменной sw1 и чтение состояние линии PA11 (SW1)
           В регистре IDR содержится состояние от всех других линий.
           Чтобы прочитать только линию PA11 необхдимо выделить только
           бит 11. Это делается с помощью маски и операции побитового И (оператор & в C).
           В числе 0x00000800 единица установлена только в 11 бите. */
        int sw1 = GPIOA->IDR & 0x00000800;

        /* Исполнение ветви в зависимости от состояния sw1 */
        if (sw1 != 0)
        {
            /* Включить светодиод PC15 (D16).
               Для того, чтобы записать 1 в позицию 15 необходимо сделать побитовое логическое
               ИЛИ регистра и маски - 0x00008000 */
            GPIOC->ODR = GPIOC->ODR | 0x00008000;
        }
        else
        {
            /* Выключить светодиод PC15 (D16).
               Для того, чтобы сбросить бит в позиции 15 необходимо сделать побитовое логическое И
               регистра и маски - 0xFFFF7FFF */
            GPIOC->ODR = GPIOC->ODR & 0xFFFF7FFF;
        }
    }
}
