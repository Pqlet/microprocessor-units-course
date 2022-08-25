; \file    main.s
; \author  Александр Смирнов
; \version 1.0.0
; \date    24.08.2022
; \brief   Программа на языке Ассемблер (ARM) для учебного стенда на базе
;          STM32F072RBT6 в среде разработки Keil uVision 5.
;          Примеры использования команд доступа к данным.

            ; ОЗУ
            ; Адрес начала: 0x20000000
            ; Объем: 0x4000 байт (16 Кбайт)
            ; ПЗУ
            ; Адрес начала: 0x08000000
            ; Объем: 0x20000 байт (128 Кбайт)

            ; Секция с программой
            AREA    PROGRAM, CODE, READONLY
            ALIGN
            ENTRY

Reset_Handler

            ; Команды перемещения между регистрами
            ; MOVS Rd, #imm8    ; Rd = #imm8, загрузить в Rd константу 1 байт (0 - 255)
            ; MOV  Rd, Rm       ; Rd = Rm, скопировать Rd в Rm
            ; MOVS Rd, Rm       ; Rd = Rm, скопировать Rd в Rm с обновлением APSR

            ; Загрузка констант в регистр
            MOVS R0, #0x10      ; Загрузить в R0 число 10 в шестнадцатиричной форме
            MOVS R1, #10        ; Загрузить в R1 число 10 в десятичной форме
            MOVS R2, #(2 * 10 - 4) ; Загрузить в R1 число 16
            MOVS R3, #'A'       ; Загрузить в R3 ASCII код символа A

            ; Копирование регистров
            MOV  R4, R0         ; Скопировать R0 в R4
            MOVS R5, R1         ; Скопировать R1 в R5 с обновлением APSR


            ; Команды LDR/STR
            ; Если требуется загрузить в регистр константу больше чем 1 байт,
            ; то необходимо сохранить эту константу в памяти программ
            ; (при компиляции программы) и используя команду LDR загрузить ее
            ; в регистр.
            ; Для упрощения этого используются псевдокоманда
            ; LDR Rd, =const     ; Rd = const
            LDR  R0, =0x20000000 ; Загрузить в R0 число 0x20000000

            ; Другая псевдо команда для загрузки константы
            ; LDR  Rd, label     ; Rd = M[label]
            LDR  R1, const1      ; Загрузить в R1 число по метке const1
const1      DCD  0x20000010      ; Вставить данные в код программы (слово)
const2      DCW  0x1234          ; Вставить данные в код программы (полуслово)
const3      DCB  1               ; Вставить данные в код программы (байт)

            ALIGN 4              ; Выравнивание. Вверху в текст программы
                                 ; принудительно вставлено 4 + 2 + 1 = 7 байт
                                 ; констант. Процессор Cortex-M0 требует,
                                 ; чтобы все команды располгались по четным
                                 ; адресам.

            ; Загрузка по PC, SP со смещением #imm8
            ; Rt - младшие регистры (R0-R7)
            ; LDR  Rt, [PC, #imm8] ; Rt = M[WordAlign(PC+4) + (#imm8 << 2)]
            ; LDR  Rt, [SP, #imm8] ; Rt = M[SP + (#imm8 << 2)]
            LDR  R4, [PC, #4]
            LDR  R5, [SP, #4]

            ; Загрузка по адресу Rn со смещением #imm5
            ; Rn - младшие регистры (R0-R7)
            ; LDRB  Rt, [Rn, #imm5] ; Rt = M[Rn + (#imm5)]
            ; LDRH  Rt, [Rn, #imm5] ; Rt = M[Rn + (#imm5 << 1)]
            ; LDR   Rt, [Rn, #imm5] ; Rt = M[Rn + (#imm5 << 2)]
            LDRB R4, [R0, #1]       ; Загрузить в R4 байт по адресу R0 + 1
            LDRH R4, [R0, #2]       ; Загрузить в R4 полуслово (2 байта) по адресу R0 + 2
            LDR  R4, [R0, #4]       ; Загрузить в R4 слово (4 байта) по адресу R0 + 4

            ; Загрузка по адресу Rn со смещением Rm
            ; Rn - младшие регистры (R0-R7)
            ; LDRB  Rt, [Rn, Rm] ; Rt = M[Rn + Rm]
            ; LDRH  Rt, [Rn, Rm] ; Rt = M[Rn + Rm]
            ; LDR   Rt, [Rn, Rm] ; Rt = M[Rn + Rm]
            LDRB R5, [R0, R2]   ; Загрузить в R5 байт по адресу R0 + R2
            LDRH R5, [R0, R2]   ; Загрузить в R5 полуслово (2 байта) по адресу R0 + R2
            LDR  R5, [R0, R2]   ; Загрузить в R5 слово (4 байта) по адресу R0 + R2

            ; LDRSH Rt, [Rn, Rm] ; Rt = M[Rn + Rm]
            ; LDRSB Rt, [Rn, Rm] ; Rt = M[Rn + Rm]
            LDRSB R6, [R0, R2]  ; Загрузить в R6 байт по адресу R0 + R2 с обновлением APSR
            LDRSH R6, [R0, R2]  ; Загрузить в R6 полуслово (2 байта) по адресу R0 + R2

            ; STRB Rt, [Rn, Rm]  ; M[Rn + Rm] = Rt
            ; STRH Rt, [Rn, Rm]  ; M[Rn + Rm] = Rt
            ; STR  Rt, [Rn, Rm]  ; M[Rn + Rm] = Rt
            STRB  R3, [R0, R2]
            STRH  R3, [R0, R2]
            STR   R3, [R0, R2]

            ; STRB Rt, [Rn, #imm5] ; M[Rn + (#imm5 << 2)] = Rt
            ; STRH Rt, [Rn, #imm5] ; M[Rn + (#imm5 << 1)] = Rt
            ; STR  Rt, [Rn, #imm5] ; M[Rn + (#imm5)] = Rt
            STRB  R3, [R0, #4]
            STRH  R3, [R0, #4]
            STR   R3, [R0, #4]

            ; STR  Rt, [SP, #imm8] ; M[SP + (#imm8) << 2)] = Rt
            STR   R2, [SP, #4]


            ; Команды обращения к стеку
            ; Адрес вершины стека находится в регистре SP/R13.
            ; Используется полный нисходящий стек.
            ; PUSH {Ra, Rb,...}         ; M[SP-4] = Ra, M[SP-8] = Rb, ...
            ; PUSH {Ra, Rb,..., LR}     ; M[SP-4] = Ra, M[SP-8] = Rb, ...
            ; POP  {Ra, Rb,...}         ; Ra = M[SP], Rb = M[SP+4], ...
            ; POP  {Ra, Rb,..., PC}     ; Ra = M[SP], Rb = M[SP+4], ...

            ; Обращение к стеку
            PUSH {R0, R1, R4-R7}  ; Поместить (скопировать) в стек регистры R0,R1,R6-R9
            POP  {R0-R6}          ; Извлечь из стека и поместить в R6-R11

            ; Обмен содержимого регистров R0 и R1 через стек
            PUSH {R0}
            PUSH {R1}
            POP  {R0}
            POP  {R1}


            ; Команды доступа к специальным регистрам (CONTROL, PRIMASK, xPSR и т.д.)
            ; MRS  Rd, SpecialReg; Rd = SpecialReg
            ; MSR  SpecialReg, Rd; SpecialReg = Rd
            MRS  R8, xPSR        ; Скопировать регистр xPSR в R8
            MRS  R9, PRIMASK     ; Скопировать регистр PRIMASK в R9
            MRS  R10, CONTROL    ; Скопировать регистр CONTROL в R10
            MSR  APSR, R8        ; Скопировать регистр R8 в APSR
            MSR  CONTROL, R9     ; Скопировать регистр R9 в CONTROL


Stop        B    Stop         ; Бесконечный цикл

            ALIGN 4

            ; Секция с таблицей векторов прерываний
            AREA    RESET, DATA, READONLY ; Указание на расположение векторов прерываний
            EXPORT  __Vectors   ; Метка __Vectors экспортируется из файла компоновщика
__Vectors
            DCD     0x20004000     ; Начало стека
            DCD     Reset_Handler  ; Вектор сброса

            END                 ; Команда транслятору об окончании текста программы
