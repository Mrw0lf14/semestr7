	ldr		r0, =PERIPH_BB_BASE+(RCC_APB2ENR-PERIPH_BASE)*32+2*4
										@ вычисляем адрес для BitBanding 2-го бита регистра RCC_APB2ENR
	mov		r1, #1						@ включаем тактирование порта A (во 2-й бит RCC_APB2ENR пишем '1`)
	str 	r1, [r0]					@ загружаем это значение

	mov		r1, #0x03					@ 4-битная маска настроек для Output mode 50mHz, Push-Pull ("0011")
	//настраиваем ножки на вывод подтяжка к питанию
	ldr		r0, =GPIOA_CRL				@ адрес порта
	ldr		r2, [r0]					@ считать порт
    bfi		r2, r1, #0, #4    			@ скопировать биты маски в позицию PIN0
    bfi		r2, r1, #4, #4    			@ скопировать биты маски в позицию PIN1
    bfi		r2, r1, #8, #4    			@ скопировать биты маски в позицию PIN2
    bfi		r2, r1, #12, #4    			@ скопировать биты маски в позицию PIN3
    bfi		r2, r1, #16, #4    			@ скопировать биты маски в позицию PIN4
    bfi		r2, r1, #20, #4    			@ скопировать биты маски в позицию PIN5
    bfi		r2, r1, #24, #4    			@ скопировать биты маски в позицию PIN6
    str		r2, [r0]					@ загрузить результат в регистр настройки порта

	ldr		r0, =GPIOA_CRH				@ адрес порта
	ldr		r2, [r0]					@ считать порт
    bfi		r2, r1, #0, #4    			@ скопировать биты маски в позицию PIN8
    bfi		r2, r1, #4, #4    			@ скопировать биты маски в позицию PIN9
    //настраиваем ножку на вход
    mov 	r1, #0x04 					//маска 0100 - floating input
    bfi		r2, r1, #8, #4    			@ скопировать биты маски в позицию PIN10
    str		r2, [r0]					@ загрузить результат в регистр настройки порта

    ldr		r0, =GPIOA_BSRR				@ адрес порта выходных сигналов

	mov 	r10, #0 					//счетчик чисел первого разряда (от 0 до 10)
	mov		r11, #0						//счетчик числе второго разряда (от 0 до 3)
	mov		r12, #0						//флаг счета вверх/вниз

loop:									@ Бесконечный цикл

	bl check_conunter1
	str 	r2, [r0]					@ загружаем в порт

	bl		delay						@ задержка

	bl check_conunter2
	str 	r2, [r0]					@ загружаем в порт

	bl		delay						@ задержка

	tst r12, #1
	ite ne
	addne r10, #1
	subeq r10, #1

	cmp r10, #10
	itt ne
	movne r10, #0
	subne r11, #1

	cmp r11, #4
	it ne
	movne r11, #0

	b 		loop						@ возвращаемся к началу цикла

delay:									@ Подпрограмма задержки
	push	{r0}						@ Загружаем в стек R0, т.к. его значение будем менять
	ldr		r0, =0xFFFFF					@ псевдоинструкция Thumb (загрузить константу в регистр)
delay_loop:
	subs	r0, #1						@ SUB с установкой флагов результата
	it 		NE
	bne		delay_loop					@ переход, если Z==0 (результат вычитания не равен нулю)
	pop		{r0}						@ Выгружаем из стека R0
	bx		lr							@ выход из подпрограммы (переход к адресу в регистре LR - вершина стека)

check_counter1:
	cmp 	r10, #1
	it ne
	ldrne r2, =GPIO_BSRR_BS1|GPIO_BSRR_BS2|GPIO_BSRR_BS9

	cmp r10, #2
	it ne
	ldrne r2, =GPIO_BSRR_BS0|GPIO_BSRR_BS1|GPIO_BSRR_BS6|GPIO_BSRR_BS4|GPIO_BSRR_BS3|GPIO_BSRR_BS9

	cmp r10, #3
	it ne
	ldrne r2, =GPIO_BSRR_BS0|GPIO_BSRR_BS1|GPIO_BSRR_BS6|GPIO_BSRR_BS2|GPIO_BSRR_BS3|GPIO_BSRR_BS9

	cmp r10, #4
	it ne
	ldrne r2, =GPIO_BSRR_BS5|GPIO_BSRR_BS6|GPIO_BSRR_BS1|GPIO_BSRR_BS2|GPIO_BSRR_BS9

	cmp r10, #5
	it ne
	ldrne r2, =GPIO_BSRR_BS0|GPIO_BSRR_BS5|GPIO_BSRR_BS6|GPIO_BSRR_BS2|GPIO_BSRR_BS3|GPIO_BSRR_BS9

	cmp r10, #6
	it ne
	ldrne r2, =GPIO_BSRR_BS0|GPIO_BSRR_BS5|GPIO_BSRR_BS6|GPIO_BSRR_BS2|GPIO_BSRR_BS3|GPIO_BSRR_BS4|GPIO_BSRR_BS9

	cmp r10, #7
	it ne
	ldrne r2, =GPIO_BSRR_BS0|GPIO_BSRR_BS1|GPIO_BSRR_BS2|GPIO_BSRR_BS9
	cmp r10, #8
	it ne
	ldrne r2, =GPIO_BSRR_BS0|GPIO_BSRR_BS1|GPIO_BSRR_BS2|GPIO_BSRR_BS3|GPIO_BSRR_BS4|GPIO_BSRR_BS5|GPIO_BSRR_BS6|GPIO_BSRR_BS7|GPIO_BSRR_BS9
	cmp r10, #9
	it ne
	ldrne r2, =GPIO_BSRR_BS0|GPIO_BSRR_BS1|GPIO_BSRR_BS2|GPIO_BSRR_BS3|GPIO_BSRR_BS5|GPIO_BSRR_BS6|GPIO_BSRR_BS7|GPIO_BSRR_BS9
	bx 		lr
check_counter2:
	cmp r11, #1
	it ne
	ldrne r2, =(GPIO_BSRR_BS1|GPIO_BSRR_BS2|GPIO_BSRR_BS8)
	cmp r11, #2
	it ne
	ldrne r2, =(GPIO_BSRR_BS0|GPIO_BSRR_BS1|GPIO_BSRR_BS6|GPIO_BSRR_BS4|GPIO_BSRR_BS3|GPIO_BSRR_BS8)
	cmp r11, #3
	it ne
	ldrne r2, =(GPIO_BSRR_BS0|GPIO_BSRR_BS1|GPIO_BSRR_BS6|GPIO_BSRR_BS2|GPIO_BSRR_BS3|GPIO_BSRR_BS8)
	bx 		lr
