
%include "rw32-2017.inc"

%macro WRITE_ARRAY 4
	push esi
	push ecx
	push eax
	mov esi,%1
   	mov ecx,%2
%%writeElement:
  	lods%3
   	call Write%4
   	cmp ecx,1
   	jbe %%skipComma
   	mov al,','
   	call WriteChar
%%skipComma:
   	loop %%writeElement

   	call WriteNewLine
   	pop eax
   	pop ecx
   	pop esi
%endmacro

%macro F13_CALL 3
	mov esi,%2
	mov ecx,%3
	call %1
%endmacro

%macro F2_CALL 5
	push %5
	push %4
	push %3
	push %2
	call %1
	add esp,16
%endmacro

section .data
    A1_1 dw -1024,768,0,768,1024
    A2_1 dw -1024,768,0,768,1024
    B2 dw -15414,-20864,-19625,-24859,21864
    A3_1 dw 2048,3840,3072,3840,4096

section .text

;--- Úkol 1 ---
;
;Naprogramujte funkci "getMax", která v poli 16bitových èísel se znaménkem nalezne maximum a vrátí jeho hodnotu.
;
;short getMax(ESI, ECX)
;  - vstup:
;    - ESI: ukazatel na zaèátek pole 16bitových prvkù se znaménkem
;    - ECX: poèet prvkù pole (32bitová hodnota bez znaménka)
;  - výstup:
;    - AX: hodnota 16bitového maxima se znaménkem     
;  - funkce musí zachovat obsah všech registrù, kromì registru EAX a pøíznakového registru

getMax:
    ; zde zadejte kód funkce
    ;push ebp
    ;mov ebp, esp
    
    ;mov ecx, [ebp+8]
    ;mov esi, [ebp+12]
    sub ecx, 1
    
    mov ax, [esi]
    for: 
     cmp [esi+ecx*2], ax
     jle pokr
     mov ax, [esi+ecx*2]
    pokr:     
     loop for        
    ;mov esp, ebp
    ;pop ebp
    ;ret 8
    ret


;--- Úkol 2 ---
;
;Naprogramujte funkci "subX", která od jednotlivých prvkù pole A odeète hodnotu x a jednotlivé výsledky uloží do pole B.
;
;Délka polí je dána parametrem N. Funkce musí být naprogramována s využitím pøedávání parametrù na zásobníku tak,
;že parametry funkce se ukládají na zásobník od posledního do prvního (zprava doleva),
;parametry ze zásobníku uklízí volající a výsledek funkce se vrací v registru EAX.
;
;int subX(short *pA, unsigned int N, short x, short *pB)
;  - vstup:
;    pA: ukazatel na pole A (pole A obsahuje 16bitové hodnoty se znaménkem)
;     N: poèet prvkù pole A (32bitové èíslo bez znaménka)
;     x: hodnota x (16bitové èíslo se znaménkem)
;    pB: ukazatel na pole B (pole B bude obsahovat 16bitové hodnoty se znaménkem)
;  - výstup:
;    - EAX =  0: v pøípadì, že nenastala žádná chyba  
;    - EAX = -1: v pøípadì, že je neplatný ukazatel *pA (tj. pA == 0)
;    - EAX = -2: v pøípadì, že je neplatný ukazatel *pB (tj. pB == 0)
;  - funkce musí zachovat obsah všech registrù, kromì registru EAX a pøíznakového registru

subX:
    ; zde zadejte kód funkce
    push ebp
    mov ebp, esp    
    
    mov ebx, [ebp+8]
    mov edx, [ebp+20]    
    
    mov ecx, 0
    smycka:
        mov ax, [ebx+ecx*2]
        sub ax, [ebp+16]
        mov [edx+ecx*2], ax
        inc ecx
        cmp ecx, [ebp+12]
        jl smycka
           
    mov eax, 0
    mov esp, ebp
    pop ebp
    ret

;
;--- Úkol 3 ---
;
;Naprogramujte funkci "sort", která sestupnì (od nejvìtšího k nejmenšímu) seøadí pole 16bitových prvkù bez znaménka A. 
;
;Ukázka algoritmu øazení v jazyce C:
;
;int *pA, i, j, N;
;for(i = 0; i < N; i++) {
;    for(j = i + 1; j < N; j++) {
;        if (pA[i] < pA[j]) { tmp = pA[i]; pA[i] = pA[j]; pA[j] = tmp; }      
;    }
; }
;
;void sort(unsigned short *pA, unsigned int N)
;  - vstup:
;    ESI: ukazatel na pole A (pole A obsahuje 16bitové hodnoty bez znaménka)
;    ECX: poèet prvkù pole A (32bitové èíslo bez znaménka)
;  - funkce musí zachovat obsah všech registrù, kromì registru EAX a pøíznakového registru     

sort:
    ; zde zadejte kód funkce
    push ebp
    mov ebp, esp
    
    
    mov eax, 0
    
    mov ecx, 0
    while:
    ;mov al, [pole+ecx]
    inc ecx
    mov edx, 0
    while2:
        ;mov al, [pole+edx]
        ;mov ah, [pole+edx+1]
        cmp al, ah
        jle pokra
        ;mov [pole+edx],ah
        ;mov [pole+edx+1],al
        mov al, ah
    pokra:
        inc edx
        mov ebx, 5
        sub ebx, ecx
        cmp edx, ebx
        jl while2
        
    cmp ecx, 4
    jl while

    
    mov ecx, 0
    fora:
        ;mov al, [pole+ecx]
        call WriteUInt8
        inc ecx
        cmp ecx, 5
        jl fora     
    
    mov esp, ebp
    pop ebp
    ret

main:
    mov ebp, esp; for correct debugging
    push ebp
    mov ebp,esp

    WRITE_ARRAY A1_1, 5, w, Int16
    F13_CALL getMax, A1_1, 5
    call WriteInt16
    call WriteNewLine

    WRITE_ARRAY A2_1, 5, w, Int16
    WRITE_ARRAY B2, 5, w, Int16
    F2_CALL subX, A2_1, 5, -10641, B2
    call WriteInt16
    call WriteNewLine
    call WriteFlags
    WRITE_ARRAY A2_1, 5, w, Int16
    WRITE_ARRAY B2, 5, w, Int16

    WRITE_ARRAY A3_1, 5, w, UInt16
    F13_CALL sort, A3_1, 5
    call WriteUInt16
    call WriteNewLine
    WRITE_ARRAY A3_1, 5, w, UInt16

    pop ebp
    ret