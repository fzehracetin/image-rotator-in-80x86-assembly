#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "image_processing.cpp"

using namespace std;

void sagaDondur(short n, int resim);
void solaDondur(short n, int resim);

int main(void) {
	int M, N, Q, i, j, k;
	bool type;
	int efile, islem;
	char resimadi[100], sonek[10];
	printf("17011068 FATMA ZEHRA CETIN\n");
	do {
		printf("Islem yapilacak resmin yolunu (path) giriniz:\n-> ");
		scanf("%s", &resimadi);
		system("CLS");
		efile = readImageHeader(resimadi, N, M, Q, type);
	} while (efile > 1);
	printf("%s\n", resimadi);
	int** resim = resimOku(resimadi);

	short *resimdizi;
	resimdizi = (short*) malloc(N*M * sizeof(short));

	for (i = 0; i < N; i++) 
		for (j = 0; j < M; j++) 
			resimdizi[i*N + j] = (short) resim[i][j];

	int resimadres = (int) resimdizi;

	do {
		system("CLS");
		printf("\t     ISLEMLER\n");
		printf("------------------------------------\n");
		printf("1)  Resmi saga dondur\n");
		printf("2)  Resmi sola dondur\n");
		printf("0)  Cikis\n\n");
		printf("\"%s\" yolundaki resim icin yapilacak islemi seciniz\n-> ", resimadi);
		scanf("%d", &islem);
	} while (islem > 2 || islem < 0);

	switch (islem) {
		case 0:
			exit(0);
		case 1:
			sagaDondur(N, resimadres);
			strcpy(sonek, "_sag.pgm");
			break;
		case 2:
			solaDondur(N , resimadres);
			strcpy(sonek, "_sol.pgm");
			break;
		default:
			strcpy(sonek, "_orj.pgm");
			break;
	}

	for (k = 0; k < N * M; k++) {
		j = k % N;
		i = k / N;
		resim[i][j] = (int)resimdizi[k];
	}
	
	string::size_type pos = string(resimadi).find_last_of(".");
	resimadi[pos] = '\0';
	strcat(resimadi, sonek);
	resimYaz(resimadi, resim, N, M, Q);
	printf("\nIslem basariyla tamamlandi :)\n\"%s\" yolunda resim olusturuldu.\n\n", resimadi);
	system("PAUSE");
	return 0;
}

void sagaDondur(short n, int resim) {
	//KODUNUZU BURADAN BASLAYARAK YAZINIZ
	__asm {
		MOV EDI, resim
		XOR ESI, ESI
		XOR ECX, ECX
		XOR EAX, EAX
		XOR EBX, EBX
		
		BASLA1 :MOV EAX, 262144;matrisin transpozunu alir
				CMP ESI, EAX
				JNB CIKIS1
				MOV EAX, ESI;i yi tutma
				
				PUSH EBX
				XOR EBX, EBX
				MOV BX, n
				XOR EDX, EDX
				DIV EBX
				MOV EAX, EDX;mod islemi sonucu atilir
				MUL EBX;(i mod n)*n
				POP EBX 
				
				MOV ECX, EBX; s i tasima
				ADD ECX, EAX; s + (i mod n)*n

				PUSH ECX
				MOV ECX, ESI; i yi tasima
				SHL ECX, 1; 2 ile carpma 
				MOV AX, WORD PTR[EDI + ECX]; dizi[i]

				POP ECX
				PUSH EBX
				MOV EBX, ECX; s + (i mod n)*n
				SHL EBX, 1; 2 ile carpma 
				XCHG WORD PTR[EDI + EBX], AX

				MOV EBX, ESI; i yi tasima
				ADD EBX, EBX
				MOV WORD PTR[EDI + EBX], AX

				POP EBX
				INC ESI; i yi artirma
				MOV EAX, ESI
				
				PUSH EBX
				XOR EBX, EBX
				XOR EDX, EDX
				MOV BX, n
				DIV EBX; i / n
				POP EBX


				CMP EBX, EAX;s ve i/n
				JZ BASLA1
				MOV EBX, EAX
				ADD ESI, EBX
				JMP BASLA1
		
		CIKIS1:
		XOR ESI, ESI
		XOR ECX, ECX
		XOR EAX, EAX
		XOR EDX, EDX
		MOV EBX, 1

		
		BASLA2 : MOV EAX, 262144; n*n
				CMP ESI, EAX;  (i < n*n)
				JNB CIKIS2

				MOV EAX, EBX; s atilir
				PUSH ECX
				XOR ECX, ECX
				MOV CX, n
				MUL ECX; s*n
				POP ECX

				SUB EAX, ECX; s*n - sayi
				DEC EAX; s*n - sayi - 1
				SHL EAX, 1; 2 ile carpma 

				PUSH EBX
				XOR EBX, EBX
				MOV BX, WORD PTR[EDI + EAX]; dizi[s*n - sayi - 1]
				MOV EDX, ESI; i atilir
				ADD EDX, EDX; 2 ile carpma cabasi
				XCHG WORD PTR[EDI + EDX], BX; dizi[i] ile exchange
				MOV WORD PTR[EDI + EAX], BX; dizi[s*n - sayi - 1] yeni deger
				POP EBX

				INC ESI; i++
				INC ECX; say++

				XOR EAX, EAX
				MOV AX, n
				SHR AX, 1; n / 2
				CMP ECX, EAX; say ve n / 2
				JNE BASLA2

				XOR ECX, ECX; say = 0
				ADD ESI, EAX; i = i + n / 2
				MOV EAX, ESI; i cekilir
				PUSH ECX
				XOR ECX, ECX
				MOV CX, n
				XOR EDX, EDX
				DIV ECX; i / n
				INC EAX; i / n + 1
				POP ECX;say
				MOV EBX, EAX; s = i / n + 1

				JMP BASLA2
		CIKIS2 :
	}
	//KODUNUZU YAZMAYI BURADA BITIRINIZ
}

void solaDondur(short n, int resim) {
	//KODUNUZU BURADAN BASLAYARAK YAZINIZ
	__asm {
		MOV EDI, resim
		XOR ESI, ESI
		XOR ECX, ECX
		XOR EAX, EAX
		XOR EDX, EDX
		MOV EBX, 1

		BASLA3: MOV EAX, 262144; n*n
				CMP ESI, EAX;  (i < n*n)
				JNB CIKIS3

				MOV EAX, EBX; s atilir
				PUSH ECX
				XOR ECX, ECX
				MOV CX, n
				MUL ECX; s*n
				POP ECX

				SUB EAX, ECX; s*n - sayi
				DEC EAX; s*n - sayi - 1
				SHL EAX, 1; 2 ile carpma 

				PUSH EBX
				XOR EBX, EBX
				MOV BX, WORD PTR[EDI + EAX]; dizi[s*n - sayi - 1]
				MOV EDX, ESI; i atilir
				SHL EDX, 1; 2 ile carpma 
				XCHG WORD PTR[EDI + EDX], BX; dizi[i] ile exchange
				MOV WORD PTR[EDI + EAX], BX; dizi[s*n - sayi - 1] yeni deger
				POP EBX

				INC ESI; i++
				INC ECX; say++

				XOR EAX, EAX
				MOV AX, n
				SHR AX, 1; n / 2
				CMP ECX, EAX; say ve n / 2
				JNE BASLA3

				XOR ECX, ECX; say = 0
				ADD ESI, EAX; i = i + n / 2
				MOV EAX, ESI; i cekilir
				PUSH ECX
				XOR ECX, ECX
				MOV CX, n
				XOR EDX, EDX
				DIV ECX; i / n
				INC EAX; i / n + 1
				POP ECX;say
				MOV EBX, EAX; s = i / n + 1

				JMP BASLA3
				CIKIS3 :

			XOR ESI, ESI
			XOR ECX, ECX
			XOR EAX, EAX
			XOR EBX, EBX

			BASLA4 : MOV EAX, 262144;matrisin transpozunu alir
			CMP ESI, EAX
			JNB CIKIS4
			MOV EAX, ESI;i yi tutma

			PUSH EBX
			XOR EBX, EBX
			MOV BX, n
			XOR EDX, EDX
			DIV EBX
			MOV EAX, EDX;mod islemi sonucu atilir
			MUL EBX;(i mod n)*n
			POP EBX

			MOV ECX, EBX; s i tasima
			ADD ECX, EAX; s + (i mod n)*n

			PUSH ECX
			MOV ECX, ESI; i yi tasima
			SHL ECX, 1; 2 ile carpma
			MOV AX, WORD PTR[EDI + ECX]; dizi[i]

			POP ECX
			PUSH EBX
			MOV EBX, ECX; s + (i mod n)*n
			SHL EBX, 1; 2 ile carpma
			XCHG WORD PTR[EDI + EBX], AX

			MOV EBX, ESI; i yi tasima
			ADD EBX, EBX
			MOV WORD PTR[EDI + EBX], AX

			POP EBX
			INC ESI; i yi artirma
			MOV EAX, ESI

			PUSH EBX
			XOR EBX, EBX
			XOR EDX, EDX
			MOV BX, n
			DIV EBX; i / n
			POP EBX


			CMP EBX, EAX;s ve i / n
			JZ BASLA4
			MOV EBX, EAX
			ADD ESI, EBX
			JMP BASLA4

	CIKIS4 :


	}
	//KODUNUZU YAZMAYI BURADA BITIRINIZ
}