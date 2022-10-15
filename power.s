.section .data

input_x_prompt	:	.asciz	"Please enter x: "
input_y_prompt	:	.asciz	"Please enter y: "
input_spec	:	.asciz	"%d"
result		:	.asciz	"x^y = %d\n"

.section .text

.global main

main:

	# prompt x
	ldr x0, =input_x_prompt
	bl printf
	bl get_input
	mov x19, x0
	
	# prompt y
	ldr x0, =input_y_prompt
	bl printf
	bl get_input
	mov x20, x0
	
	#call power
	mov x0, x19
	mov x1, x20
	bl get_power
	
	#print result
	mov x1, x2
	ldr x0, =result
	bl printf
	
	b exit
	
# based on get_input function of the add program from pa2 workshop
get_input:
	sub sp, sp, 8
	str x30, [sp]
	ldr x0, =input_spec
	sub sp, sp, 8
	mov x1, sp
	bl scanf

	ldrsw x0, [sp]
	ldr x30, [sp, 8]
	add sp, sp, 16
	ret


# pseudocode for power(x, y):
#    if x is 0
#        return 0
#    else if y less than 0
#        return 0
#    else if y is 0
#        return 1
#    else
#        return x * power(x, y - 1)

# returns the result of x^y in x2. Arguments x0 and x1 are preserved
get_power: 
	
	# if x = 0, return 0
	subs x9, x0, 0
	b.eq base_case_x
	
	# if y < 0, return 0
	subs x9, x1, 0
	b.lt negative_y
	
	# if y = 0, return 1
	sub x9, x1, 0
	cbz x9, base_case_y
	
	# if x = 1, return 1. Included here to avoid too many recursive calls
								# in case of 1^y and y is very large
	subs x9, x0, 1
	b.eq base_case_one
	
	# if y > 0, go to the recursive case
	subs x9, x1, 0
	b.gt recurse
	
	ret
	
	#return 0 if x = 0
	base_case_x:
		mov x2, 0
		ret
	
	
	#return 1 if y = 0
	base_case_y:
		mov x2, 1
		ret
	
	#return 0 if y is negative
	negative_y:
		mov x2, 0
		ret
	
	# return 1 if x is 1
	base_case_one:
		mov x2, 1
		ret
		
	#recursive case. 
	recurse:
		# save the return address and arguments		
		sub sp, sp, 24
		str x30, [sp]
		str x0, [sp, 8]
		str x1, [sp, 16]
		# call get_power with y-1
		sub x1, x1, 1
		bl get_power
		mul x2, x0, x2
		ldr x30, [sp]
		ldr x0, [sp, 8]
		ldr x1, [sp, 16]
		add sp, sp, 24
		ret
	
exit:
	mov x0, 0
	mov x8, 93
	svc 0
	ret
