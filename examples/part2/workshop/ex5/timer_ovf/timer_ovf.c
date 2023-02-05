/**
 ******************************************************************************
 * \file    timer_ovf.c
 * \author  Александр Смирнов
 * \version 1.0.0
 * \date    20.04.2022
 * \brief   Программа на языке C для учебного стенда на базе
 *          STM32F072RBT6 в среде разработки Keil uVision 5.
 *          Подключение библиотек поддержки МК STM32F072RBT6 осуществляется
 *          средствами IDE Keil через менеджер пакетов Run-Time Environment (RTE).
 *          Разработать программу, обеспечивающую управление бегущим огнем
 *          на светодиодах D1-D8: один огонь вначале бежит справа налево
 *          с постоянной невысокой скоростью, затем слева направо. Процесс бесконечный.
 *          Программа работает в режиме 0 учебного стенда (S1 = 0, S2 = 0).
 ******************************************************************************
 */

/* Подключение заголовочного файла с макроопределениями всех регистром специальных
   функций МК STM32F072RBT6. */
#include <stm32f0xx.h>

/* Функция инициализации светодиодов D1-D8 и линий управления цветом */
void leds_init(void)
{
    /* Включение тактирования порта C */
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    /* Настройка на вывод линий PC0 - PC7 (D1 - D8) */
    GPIOC->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 |
                    GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 |
                    GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0;
}

/* Функция включения светодиодов */
void led_set(uint8_t led)
{
    /* Записываем в регистр данных порта C новое состояние светодиодов.
       Номер бита соответствует номеру светодиода: бит 0 - D1, бит 1 - D2 и
       так далее */
    GPIOC->ODR = led;
}

/* Функция инициализации таймера TIM1 */
void timer_init()
{
    /* Включение тактирования TIM1 */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    /* Расчет предделителя частоты и кода переполнения таймера
       (максимальный код таймера).
       Пусть таймер счетчик переключается каждую 1 мс или 1 кГц,
       тогда при частоте тактирования МК fmcu = 8 МГц, требуется предделитель
       на 8000. Prescaler = 8 МГц / 1 кГц = 8000.
       Пусть переполнение происходит каждую 250 мс или 1/4 Гц,
       тогда код переполнения должен быть 250 */
    /* Предделитель частоты */
    TIM1->PSC = 8000;

    /* Максимальный код таймера (счет идет от 0 до 249) */
    TIM1->ARR = 250 - 1;

    /* Включение прерывания по переполнению */
    TIM1->DIER |= TIM_DIER_UIE;

    /* Включение таймера */
    TIM1->CR1 |= TIM_CR1_CEN;

    /* Установка приоритета прерывания по переполнению таймера.
       В Cortex-M0 четыре уровня приоритета - 0-3.
       0 - наибольший приоритет, 3 - наименьший. */
    NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 0);

    /* Разрешение прерывания по переполнению */
    NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
}

/* Переменная для сохранения состояния светодиодов */
uint16_t led = 0x01; /* Начальное состояние - включен самый правый светодиод */
/* Переменная для направления бегущего огня. 0 - справа налево, 1 - слева направо */
uint8_t direction = 0;

/* Подпрограмма обработчик прерываний по переполнению таймера */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    /* Сброс флага вызвавшего прерывание */
    TIM1->SR &= ~TIM_SR_UIF;

    /* Если сдвиг влево */
    if (direction == 0)
    {
        /* Сдвиг светодиода на одну позицию влево */
        led = led << 1;

        /* Если сдвиг произошел дальше 8 светодиода */
        if (led == 0x80)
        {
            /* Меняется направление сдвига */
            direction = 1;
            /* Начинается сдвиг с последнего светодиода */
            led = 0x80;
        }
    }
    /* В противном случае вправо */
    else
    {
        /* Сдвиг светодиода на одну позицию вправо */
        led = led >> 1;
        /* Если сдвиг произошел дальше 8 светодиода */
        if (led == 0x01)
        {
            /* Меняется направление сдвига */
            direction = 0;
            /* Начинается сдвиг с первого светодиода */
            led = 0x01;
        }
    }

    /* Включение светодиода */
    led_set(led);
}

/* Функция main - точка входа в программу */
int main(void)
{
    /* Инициализация светодиодов D1-D8 */
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
