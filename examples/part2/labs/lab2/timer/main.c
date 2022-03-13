/**
  ******************************************************************************
  * \file    main.c
  * \author  Александр Смирнов
  * \version 1.0.0
  * \date    13.03.2022
  * \brief   Программа на языке C для учебного стенда на базе
  *          STM32F072RBT6 в среде разработки Keil uVision 5.
  *          Подключение библиотек поддержки МК STM32F072RBT6 осуществляется
  *          средствами IDE Keil через Run-Time Environment (RTE).
  *          Разработать программу, обеспечивающую управление бегущим огнем
  *          на светодиодах LED1-LED8: один огонь бежит справо налево
  *          с постоянной невысокой скоростью и яркостью. Цвет огня – красный. 
  *          После первого пробега цвет меняется на зеленый и повышается яркость.
  *          После второго пробега цвет возвращается на красный и яркость
  *          возвращается на минимальную. Процесс бесконечный.
  *          Программа работает в режиме 1 учебного стенда (S1 = 0, S2 = 1).
  ******************************************************************************
  */

/* Подключение заголовочного файла с макроопределениями всех регистром специальных
   функций МК STM32F072RBT6. */
#include <stm32f0xx.h>

/* Функция инициализации светодиодов LED1-LED8 и линий управления цветом */
void leds_init(void)
{
    /* Включение тактирования порта A */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    /* Включение тактирования порта C */
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    /* Настройка на вывод линий PA6, PA7, PA8 (RED, GREEN, BLUE) */
    GPIOA->MODER |= (GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0);

    /* Настройка на вывод линий PC0 - PC7 (LED1 - LED8) */
    GPIOC->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 |
                    GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 |
                    GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0;
}

/* Макроопределения с цветами */
#define RED     0
#define GREEN   1
#define BLUE    2

/* Функция включения светодиодов и выбора цвета */
void led_set(uint8_t led, uint8_t color)
{
    /* Записываем в регистр данных порта C новое состояние светодиодов.
       Номер бита соответствует номеру светодиода: бит 0 - LED1, бит 1 - LED2 и
       так далее */
    GPIOC->ODR = led;

    /* Сброс трех битов управления цветом с помощью маски */
    GPIOA->ODR &= ~(7 << 6);

    /* Включение светодиодов нужно цвета */
    if (color == RED)
    {
        GPIOA->ODR |= (1 << 6);
    }
    else if (color == GREEN)
    {
        GPIOA->ODR |= (1 << 7);
    }
    else if (color == BLUE)
    {
        GPIOA->ODR |= (1 << 8);
    }
}

/* Функция инициализации таймера TIM1 */
void timer_init()
{
    /* Включение тактирования TIM1 */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    /* Расчет предделителя частоты и кода переполнения таймера
       (максимальный код таймера).
       Пусть таймер счетчик переключается каждую 1 мкс или 1 МГц,
       тогда при частоте тактирования МК fmcu = 8 МГц, предделитель требуется
       на 8 - 8 МГц / 1 МГц = 8.
       Пусть переполнение происходит каждую 1 мс или 1 кГц,
       тогда код переполнения должен быть 1 МГц / 1 кГц = 1000 */
    /* Предделитель частоты */
    TIM1->PSC = 8;

    /* Максимальный код таймера (счет идет от 0 до 999) */
    TIM1->ARR = 999;

    TIM1->CCR1 = 250;

    /* Включение прерывания по переполнению */
    TIM1->DIER |= TIM_DIER_UIE;

    /* Включение прерывания по сравнению - канал 1 */
    TIM1->DIER |= TIM_DIER_CC1IE;

    /* Включить таймер */
    TIM1->CR1 |= TIM_CR1_CEN;

    /* Установка приоритета прерывания по переполнению таймера.
       В Cortex-M0 четыре уровня приоритета - 0-3.
       0 - наибольший приоритет, 3 - наименьший. */
    NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 0);

    /* Установка приоритета прерывания по сравнению. */
    NVIC_SetPriority(TIM1_CC_IRQn, 0);

    /* Разрешение прервания по переполнению */
    NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);

    /* Разрешение прервания прерывания по сравнению. */
    NVIC_EnableIRQ(TIM1_CC_IRQn);
}

/* Переменная для сохранения состояния светодиодов */
uint16_t led = 0x01; /* Начальное состояние - включен самый правый светодиод */
/* Переменная для сохранения цвета светодиодов */
uint16_t color = RED; /* Начальное состояние - красный цвет */
/* Переменная для подсчета количества переполнений */
uint16_t up_cnt = 0;
/* Переменная для сохранения яркости - код в канале выходного сравнения */
uint16_t brightness = 100;

/* Подпрограмма обработчик прерываний по переполнению таймера */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    /* Сброс флага вызвавшего прерывание */
    TIM1->SR &= ~TIM_SR_UIF;

    /* Подсчет количества переполнений таймера.
       Между каждым переполнением проходит по 1 мс */
    up_cnt++;

    /* Если прошло больше 250 мс */
    if (up_cnt > 250)
    {
        /* Подсчет заново */
        up_cnt = 0;
        /* Сдвиг маски светодиода на одну позицию влево */
        led = led << 1;
    }

    /* Если сдвиг произошел дальше 8 светодиода */
    if (led == 0x100)
    {
        /* Начинается сдвиг с первого светодиода */
        led = 0x01;

        /* Изменение цвета и яркости */
        if (color == RED)
        {
            brightness = 750;
            color = GREEN;
        }
        else if (color == GREEN)
        {
            brightness = 100;
            color = RED;
        }
    }

    /* Включение светодиода с нужным цветом */
    led_set(led, color);

    /* Запись в регистр выходного сравнения нового кода */
    TIM1->CCR1 = brightness;
}

/* Подпрограмма обработчик прерываний по выходному сравнению таймера */
void TIM1_CC_IRQHandler(void)
{
    /* Сброс флага вызвавшего прерывание */
    TIM1->SR &= ~TIM_SR_CC1IF;

    /* Выключение всех светодиодов */
    led_set(0, color);
}

/* Функция main - точка входа в программу */
int main(void)
{
    /* Инициализация светодиодов LED1-LED8 и управления цветом */
    leds_init();
    /* Инициализация таймера TIM1 */
    timer_init();

    /* Бесконечный цикл */
    while (1)
    {
        /* Основные действия происходят в подпрограммах обработчиках
           прерываний.
        */
    }
}
