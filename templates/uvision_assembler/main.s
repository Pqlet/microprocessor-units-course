; Шаблон программы на языке ARM ассемблер и проекта для IDE Keil uVision 5.

            ; Микроконтроллер STM32F072RBT6
            ; ОЗУ
            ; Адрес начала: 0x20000000
            ; Объем: 0x4000 байт (16 Кбайт)
            ; ПЗУ
            ; Адрес начала: 0x08000000
            ; Объем: 0x20000 байт (128 Кбайт)

            ; Секция с программой
            ; Обязательная секция
            AREA    PROGRAM, CODE, READONLY     ; Указание на расположение программы в памяти
            ENTRY                               ; Указание на точку входа в программу
            ALIGN                               ; Выровнять по слову (4 байта)

Reset_Handler
            ; Команды программы располагаются с этого места и до метки Stop

            ; НАЧАЛО ПРОГРАММЫ
            NOP
            NOP
            ; КОНЕЦ ПРОГРАММЫ

            ; Программа останавливается здесь
Stop
            B      Stop             ; Бесконечный цикл

            ALIGN                   ; Выровнять по слову (4 байта)

            ; Секция с таблицей векторов прерываний.
            ; Этот секция обязательна. Без нее программа не будет компилироваться.
            AREA    RESET, DATA, READONLY ; Указание на расположение векторов прерываний
            EXPORT  Vectors         ; Экспорт метки Vectors для компоновщика
Vectors
            DCD     0x20004000      ; Начало стека (последний адрес ОЗУ)
            DCD     Reset_Handler   ; Вектор сброса (начало программы)

            END                     ; Команда транслятору об окончании текста программы