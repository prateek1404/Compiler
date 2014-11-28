.MODEL TINY
 .DATA 
temp dw ?
y dw ?
x dw ?
fact dw ?
p dw ?
.CODE 
.STARTUP
MOV AX,5 
MOV x,AX 

MOV BX,y 
MOV CX,p 
SUB CX,BX 
JL LABEL0 
MOV BX,p 
MOV CX,x 
SUB BX,CX 
JLE LABEL1 
MOV AX,y 
POP DX
MOV BX,y 
IMUL BX 
MOV DX,AX 

MOV AX,y 
POP DX
MOV BX,y 
IMUL BX 
MOV DX,AX 

MOV AX,DX 
POP DX
MOV BX,DX 
IMUL BX 
MOV DX,AX 

MOV AX,y 
MOV BX,DX 
IMUL BX 
MOV DX,AX 

MOV AX,y 
MOV BX,DX 
IMUL BX 
MOV DX,AX 

MOV AX,y 
MOV BX,DX 
IMUL BX 
MOV DX,AX 

MOV AX,DX 
MOV y,AX 

LABEL1:
LABEL0:
MOV BX,y 
MOV CX,p 
SUB BX,CX 
JG LABEL2 
MOV BX,y 
MOV CX,1 
SUB BX,CX 
MOV DX,BX 

MOV AX,DX 
MOV y,AX 

LABEL2:
.EXIT 

getid proc near
MOV BX,0
MOV temp,BX
MOV cx,0
MOV ah,01h
INT 21h
MOV BL,al
cmp BL,'-'
jne lab1
MOV temp,1
lab2:
MOV ah,01h
int 21h
lab1:
MOV BL,al
cmp BL,13
je lab3
MOV ah,0
push ax
add cx,1
jmp lab2
lab3:
MOV BX,0
MOV bp,1
lab4:
pop si
sub si,'0'
MOV di,si
MOV ax,bp
imul di
add BX,ax
MOV ax,bp
MOV di,10
imul di
MOV bp,ax
sub cx,1
jnz lab4
MOV ax,temp
sub ax,0
je lab5
neg BX
lab5:
MOV dl,10
MOV ah,2
int 21h
MOV dl,13
MOV ah,2
int 21h
ret
getid endp

printid proc near
MOV si,0
MOV cx,10
lBL1:
MOV dx,0
MOV ax,BX
idiv cx
add dx,'0'
push dx
add si,1
MOV BX,ax
sub BX,0
jnz lBL1
lBL2:
pop dx
MOV ah,2
int 21h
sub si,1
jnz lBL2
MOV dl,10
MOV ah,2
int 21h
MOV dl,13
MOV ah,2
int 21h
ret
printid endp
end
