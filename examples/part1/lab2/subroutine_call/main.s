; \file    main.s
; \author  Александр Смирнов
; \version 1.0.0
; \date    23.08.2022
; \brief   Программа на языке Ассемблер (ARM) для учебного стенда на базе
;          STM32F072RBT6 в среде разработки Keil uVision 5.
;          Примеры использования команд управления и контроля для вызова
;          подпрограмм.

            ; Секция с программой
            AREA    PROGRAM, CODE, READONLY
            ALIGN
            ENTRY

Reset_Handler

            ; Вызов простой подпрограммы simple_subroutine
            ; После вызова команды BL адрес, следующей за ней команды
            ; сохраняется в LR/R14,
            ; а в PC/R15 записывается адрес simple_subroutine
            BL   simple_subroutine


            ; Вызов подпрограммы subroutine
            BL   subroutine


Stop        B    Stop         ; Бесконечный цикл


            ; Простая подпрограмма
            ; В этой подпрограмме без дополнительных действий нельзя
            ; изменять регистры и делать вызовы других подпрограмм
simple_subroutine
            NOP               ; Команды с полезными действиями
            ; После вызова BX в PC/R15 записывается содержимое LR/R14
            BX   LR


            ; Подпрограмма с сохранением на стеке регистров
subroutine
            PUSH {R4-R6, LR}  ; Сохранение на стеке регистров R4, R5, R6, LR/R14

            MOVS R4, #0       ; Регистр R4 можно изменять, не боясь нарушить
                              ; работы вызвавшей программы,
                              ; так как R4 сохренен на стеке
            BL   simple_subroutine ; Можно вызывать подпрограммы, так как
                                   ; LR сохранен на стеке
            NOP               ; Команды с полезными действиями

            POP  {R4-R6, PC}  ; Восстановление из стека регистров R4, R5, R6 и
                              ; PC/R15. При восстановление LR в PC происходит
                              ; переход в вызывающую программу.


            ALIGN 4

; Секция с таблицей векторов прерываний
            AREA    RESET, DATA, READONLY ; Указание на расположение векторов прерываний
            EXPORT  Vectors         ; Экспорт метки Vectors для компоновщика
Vectors
            DCD     0x20004000      ; Начало стека (последний адрес ОЗУ)
            DCD     Reset_Handler   ; Вектор сброса (начало программы)

            END                     ; Команда транслятору об окончании текста программы
