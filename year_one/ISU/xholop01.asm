
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

;--- �kol 1 ---
;
;Naprogramujte funkci "getMax", kter� v poli 16bitov�ch ��sel se znam�nkem nalezne maximum a vr�t� jeho hodnotu.
;
;short getMax(ESI, ECX)
;  - vstup:
;    - ESI: ukazatel na za��tek pole 16bitov�ch prvk� se znam�nkem
;    - ECX: po�et prvk� pole (32bitov� hodnota bez znam�nka)
;  - v�stup:
;    - AX: hodnota 16bitov�ho maxima se znam�nkem     
;  - funkce mus� zachovat obsah v�ech registr�, krom� registru EAX a p��znakov�ho registru

getMax:
    ; zde zadejte k�d funkce
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


;--- �kol 2 ---
;
;Naprogramujte funkci "subX", kter� od jednotliv�ch prvk� pole A ode�te hodnotu x a jednotliv� v�sledky ulo�� do pole B.
;
;D�lka pol� je d�na parametrem N. Funkce mus� b�t naprogramov�na s vyu�it�m p�ed�v�n� parametr� na z�sobn�ku tak,
;�e parametry funkce se ukl�daj� na z�sobn�k od posledn�ho do prvn�ho (zprava doleva),
;parametry ze z�sobn�ku ukl�z� volaj�c� a v�sledek funkce se vrac� v registru EAX.
;
;int subX(short *pA, unsigned int N, short x, short *pB)
;  - vstup:
;    pA: ukazatel na pole A (pole A obsahuje 16bitov� hodnoty se znam�nkem)
;     N: po�et prvk� pole A (32bitov� ��slo bez znam�nka)
;     x: hodnota x (16bitov� ��slo se znam�nkem)
;    pB: ukazatel na pole B (pole B bude obsahovat 16bitov� hodnoty se znam�nkem)
;  - v�stup:
;    - EAX =  0: v p��pad�, �e nenastala ��dn� chyba  
;    - EAX = -1: v p��pad�, �e je neplatn� ukazatel *pA (tj. pA == 0)
;    - EAX = -2: v p��pad�, �e je neplatn� ukazatel *pB (tj. pB == 0)
;  - funkce mus� zachovat obsah v�ech registr�, krom� registru EAX a p��znakov�ho registru

subX:
    ; zde zadejte k�d funkce
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
;--- �kol 3 ---
;
;Naprogramujte funkci "sort", kter� sestupn� (od nejv�t��ho k nejmen��mu) se�ad� pole 16bitov�ch prvk� bez znam�nka A. 
;
;Uk�zka algoritmu �azen� v jazyce C:
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
;    ESI: ukazatel na pole A (pole A obsahuje 16bitov� hodnoty bez znam�nka)
;    ECX: po�et prvk� pole A (32bitov� ��slo bez znam�nka)
;  - funkce mus� zachovat obsah v�ech registr�, krom� registru EAX a p��znakov�ho registru     

sort:
    ; zde zadejte k�d funkce
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