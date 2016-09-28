call main
#stop
fact:
push ebp
mov ebp, esp
sub esp, 16

#show dword[ebp + 8]
xor ebx, ebx
mov eax, dword[ebp + 8]
cmp eax, 2
setl bl 
mov dword[ebp - 4], ebx 

cmp dword[ebp - 4], 0
je lbl_1

mov eax, 1
leave
ret

jmp lbl_2
lbl_1:
mov eax, dword[ebp + 8]
sub eax, 1
mov dword[ebp - 16], eax 
push dword[ebp - 16]
call fact
add esp, 4
mov dword[ebp - 12], eax 
mov eax, dword[ebp + 8]
imul eax, dword[ebp - 12]
mov dword[ebp - 8], eax 

mov eax, dword[ebp - 8]
leave
ret

lbl_2:


main:
push ebp
mov ebp, esp
sub esp, 8
push 12
call fact
add esp, 4
mov dword[ebp - 4], eax 

mov eax, dword[ebp - 4]
mov dword[ebp - 8], eax

#show dword[ebp  - 8]
leave
ret


