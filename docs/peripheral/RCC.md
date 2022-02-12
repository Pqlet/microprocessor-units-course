# Регистры сброса и управления тактированием - Reset and clock control (RCC)

В этом разделе дается подробное описание регистров RCC.

- [Регистры сброса и управления тактированием - Reset and clock control (RCC)](#регистры-сброса-и-управления-тактированием---reset-and-clock-control-rcc)
  - [Регистр включения тактирования периферийных модулей AHB](#регистр-включения-тактирования-периферийных-модулей-ahb)

## Регистр включения тактирования периферийных модулей AHB

AHB peripheral clock enable register (`RCC->AHBENR`)

Смещение адреса: `0x14`

Значение после сброса: `0x0000 0014`

> :bulb: **Примечание:**  Когда тактирование периферии выключено, значения периферийных регистров могут быть недоступны для чтения программой, и возвращаемое значение всегда равно 0x00.

![RCC](../../img/RCC_AHBENR.jpg)

Биты 31-25: Зарезервированы.

Бит 24 `TSCEN`: Включение тактирования контроллера сенсорной панели. Устанавливается и сбрасывается программно.

- 0: TCS выключение тактирования
- 1: TCS включение тактирования

Бит 22 `IOPFEN`: Включение тактирования порта ввода/вывода F. Устанавливается и сбрасывается программно.

- 0: выключение тактирования порта ввода/вывода F
- 1: включение тактирования порта ввода/вывода F

Бит 21 `IOPEEN`: Включение тактирования порта ввода/вывода E. Устанавливается и сбрасывается программно.

- 0: выключение тактирования порта ввода/вывода E
- 1: включение тактирования порта ввода/вывода E

Бит 20 `IOPDEN`: Включение тактирования порта ввода/вывода D. Устанавливается и сбрасывается программно.

- 0: выключение тактирования порта ввода/вывода D
- 1: включение тактирования порта ввода/вывода D

Бит 19 `IOPCEN`: Включение тактирования порта ввода/вывода C Устанавливается и сбрасывается программно.

- 0: выключение тактирования порта ввода/вывода C
- 1: включение тактирования порта ввода/вывода C

Бит 18 `IOPBEN`: Включение тактирования порта ввода/вывода B. Устанавливается и сбрасывается программно.

- 0: выключение тактирования порта ввода/вывода B
- 1: включение тактирования порта ввода/вывода B

Бит 17 `IOPAEN`: Включение тактирования порта ввода/вывода A. Устанавливается и сбрасывается программно.

- 0: выключение тактирования порта ввода/вывода A
- 1: включение тактирования порта ввода/вывода A

Биты 16-7: Зарезервированы.

Бит 6 `CRC`: Включение тактирования модуля CRC. Устанавливается и сбрасывается программно.

- 0: выключение тактирования модуля CRC
- 1: включение тактирования модуля CRC

Бит 5: Зарезервирован.

Бит 4 `FLITFEN`: Включение тактирования интерфейса программирования Flash памяти FLITF. Устанавливается и сбрасывается программно.

- 0: выключение тактирования интерфейса FLITF во время спящего режима
- 1: включение тактирования интерфейса FLITF во время спящего режима

Бит 3: Зарезервирован.

Бит 2 `SRAMEN`: Включение тактирования интерфейса памяти SRAM. Устанавливается и сбрасывается программно.

- 0: выключение тактирования интерфейса SRAM во время спящего режима
- 1: включение тактирования интерфейса SRAM во время спящего режима

Бит 1 `DMA2EN`: Включение тактирования модуля DMA2. Устанавливается и сбрасывается программно.

- 0: выключение тактирования модуля DMA2
- 1: включение тактирования модуля DMA2

Бит 0 `DMAEN`: Включение тактирования модуля DMA. Устанавливается и сбрасывается программно.

- 0: выключение тактирования модуля DMA
- 1: включение тактирования модуля DMA