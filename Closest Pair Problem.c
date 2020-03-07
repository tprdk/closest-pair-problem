#include<stdio.h>
#include<stdlib.h>
#include<math.h>
struct n{
	int x;
	int y;
};
typedef struct n nokta;
void noktaYazdir(nokta* noktalar,int n){				//noktalar� yazd�r�yoruz
	int i;
	for(i=0;i<n;i++){
		printf("%d. nokta x == %d , y == %d\n",i,noktalar[i].x,noktalar[i].y);
	}
}
FILE* dosyaAc(char* isim){								//dosya a�ma i�leminin yap�ld��� yer
	FILE* dosya;
	dosya=fopen(isim,"r");
	if(dosya==NULL){
		printf("dosya acilamadi\n");
	}else{
		printf("dosya acildi\n");
		return dosya;
	}	
}
void sifirla(char *dizi,int n){							//textten okurken kulland���m�z diziyi s�f�rlad���m�z fonksiyon
	int i;
	for(i=0;dizi[i]=!'\0' && i<n;i++)
		dizi[i]='\0';
}
nokta duzelt(char *dizi,int n){							
	nokta nokta1;				//okuyaca��m�z noktalar� koordinat de�erleri halinde tuttu�umuz de�i�ken
	int i;						//indis
	int sayi1=0;				//okuyaca��m�z birinci say�
	int sayi2=0;				//okuyaca��mz� 2. say�
	int negatif=0;				//de�erin negatifligini kontrol ediyoruz
	for(i=0;dizi[i]!=' ' ;i++){							//ilk say�n�n olu�turuldu�u yer
		sayi1*=10;
		if(dizi[i] == '-')
			negatif=1;									//negatif sayi kontrolu
		else
		sayi1+=(dizi[i]-'0');	
	}
	if(negatif)
		sayi1*=-1;
	negatif=0;	
	i++;
	for(i;dizi[i]!=' ' && dizi[i]!='\0' && dizi[i]!='\n';i++){		//ikinci say�n�n olu�turulmas�
		sayi2*=10;
		if(dizi[i] == '-')
			negatif=1;
		else
			sayi2+=(dizi[i]-'0');	
	}
	if(negatif)
		sayi2*=-1;
	nokta1.x=sayi1;
	nokta1.y=sayi2;						//noktay� olu�ruturuyoruz
	return nokta1;						//noktay� geri d�nd�r�yoruz
}
nokta* noktalariOku(FILE *dosya,int *n,nokta *noktalar){		//dosyadan say�lar� okuyoruz
	noktalar=(nokta*)malloc(sizeof(nokta)*100);					//en fazla 100 eleman tutan bir noktalar dizisi
	char dizi[10];												//dosyadan okuyaca��m�z� yazd���m�z char dizisi
	fseek(dosya,0,SEEK_SET);
	while(!(feof(dosya))){
		sifirla(dizi,10);										//diziyi s�f�rl�yoruz
		fgets(dizi,10,dosya);									//sat�r sat�r en fazla 10 karakter okuyacak �ekilde okuyoruz
		noktalar[(*n)++]=duzelt(&dizi[0],10);					//duzeltten gelen noktay� noktalar dizisine ekliyoruz	
	}
	fclose(dosya);												//dosyay� kapat�yoruz
	return noktalar;			
}
void noktaSirala(nokta* noktalar,int ilk,int son){             //quick sort
	int sol=ilk+1;								   
	int sag=son;
	int temp;
	if(ilk < son){
		int pivot=ilk;
		while(sol <= sag){
			if(noktalar[sol].x > noktalar[pivot].x && noktalar[sag].x < noktalar[pivot].x){
				temp=noktalar[sol].x;
				noktalar[sol].x=noktalar[sag].x;
				noktalar[sag].x=temp;
				temp=noktalar[sol].y;
				noktalar[sol].y=noktalar[sag].y;
				noktalar[sag].y=temp;
				sol++;
				sag--;
			}
			if(noktalar[sol].x <= noktalar[pivot].x || noktalar[sag].x >= noktalar[pivot].x){
				if(noktalar[sol].x <= noktalar[pivot].x)
					sol++;
				if(noktalar[sag].x >= noktalar[pivot].x)
					sag--;
			}	
		}
		temp=noktalar[sol-1].x;
		noktalar[sol-1].x=noktalar[pivot].x;
		noktalar[pivot].x=temp;
		temp=noktalar[sol-1].y;
		noktalar[sol-1].y=noktalar[pivot].y;
		noktalar[pivot].y=temp;
		noktaSirala(noktalar,ilk,sol-2);
		noktaSirala(noktalar,sol,son);	
	}
}
int medyanBul(int ilkElemanIndis,int sonElemanIndis){		// medyan=n/2 
	return (ilkElemanIndis+sonElemanIndis+1)/2;
}
float uzaklikHesapla(nokta* noktalar,int indis1,int indis2){		// Karekok((x1-x2)^2+(y1-y2)^2) form�l� ile uzakligi hesapl�yoruz
	return sqrt((noktalar[indis1].x-noktalar[indis2].x)*(noktalar[indis1].x-noktalar[indis2].x)+(noktalar[indis1].y-noktalar[indis2].y)*(noktalar[indis1].y-noktalar[indis2].y));
}
float* brutForce(nokta* noktalar,int ilkElemanIndis,int sonElemanIndis){	//3 ve 3 ten k���k say�da noktay� brut olarak hesapl�yoruz
	int size=sonElemanIndis-ilkElemanIndis+1;						//dizinin size �n� hesaplad���m�z k�s�m		
	int i;															//indisler	
	int j;
	float dist;														//uzaklik de�i�keni
	float* min=(float*)malloc(sizeof(float)*3);						//min[0] = ilk nokta indisi     min[1]=ikinci nokta indisi
	min[2]=1000.0;													//min[2] = aralar�ndaki uzakl�k
	for(i=ilkElemanIndis;i<size+ilkElemanIndis-1;i++){				
		for(j=i+1;j<size+ilkElemanIndis;j++){
			dist=uzaklikHesapla(noktalar,i,j);						//iki nokta aras�ndaki uzaklik hesaplamas�
			if(min[2] > dist){										//e�er min uzakl�k bulunan uzakliktan b�y�kse de�i�triyoruz
				min[0]=i;
				min[1]=j;
				min[2]=dist;
			}
		}
	}
	return  min;
}
float* enYakinNoktalar(nokta* noktalar,int ilkElemanIndis,int sonElemanIndis){
	int size=sonElemanIndis-ilkElemanIndis+1;                          //size hesab�
	if(size <= 3){
		return brutForce(noktalar,ilkElemanIndis,sonElemanIndis);		//size <= 3 durumunda brutforce yap�yoruz
	}else{
		float* noktalar1=(float*)malloc(sizeof(float)*3);				//noktalar�n indislerini tuttu�umuz k�s�m	
		float* noktalar2=(float*)malloc(sizeof(float)*3);				//min in ayn�s�
		noktalar1=enYakinNoktalar(noktalar,ilkElemanIndis,medyanBul(ilkElemanIndis,sonElemanIndis));	// ikiye b�lme k�sm�
		noktalar2=enYakinNoktalar(noktalar,medyanBul(ilkElemanIndis,sonElemanIndis),sonElemanIndis);	
		printf("Soldaki en kisa mesafe 1. nokta x=%d y=%d   2. nokta x=%d y=%d    uzaklik=%f\n",noktalar[(int)noktalar1[0]].x,noktalar[(int)noktalar1[0]].y,noktalar[(int)noktalar1[1]].x,noktalar[(int)noktalar1[1]].y,noktalar1[2]);
		printf("Sagdaki en kisa mesafe 1. nokta x=%d y=%d   2. nokta x=%d y=%d    uzaklik=%f\n\n",noktalar[(int)noktalar2[0]].x,noktalar[(int)noktalar2[0]].y,noktalar[(int)noktalar2[1]].x,noktalar[(int)noktalar2[1]].y,noktalar2[2]);	
		if(noktalar1[2] < noktalar2[2])	
			return noktalar1;
		else														//daha k�sa uzunlukta olan noktalar� d�nd�r�yoruz
			return noktalar2;
	}
}
float* bulDikdortgen(nokta* noktalar,float* min,int n){
	int i,j;														//indisler
	int k=-1;														//dikd�rtgendeki noktalar� tutacak dizi indisi
	int indisler[20];												//dikd�rtgendeki noktalar� tutacak dizi
	int medyan=medyanBul(0,n-1);									//medyan	
	float dist;														//uzakl�k de�i�keni
	for(i=0;i<n-1;i++){												//burada dikd�rtgenin i�inde olan noktalar� buluyoruz								
		if(noktalar[i].x < noktalar[medyan].x+min[2] && noktalar[i].x > noktalar[medyan].x-min[2]){
			indisler[++k]=i;
		}
	}
	for(i=0;i<k;i++){
		for(j=i+1;j<=k;j++){						//dikd�rtgende daha k�sa mesafeli noktalar var m� kontrol ediyoruz
			dist=uzaklikHesapla(noktalar,indisler[i],indisler[j]);
			if(min[2] > dist){
				min[0]=i;
				min[1]=j;							//varsa yeni en yak�n noktalar�m�z bu oluyor
				min[2]=dist;
			}
		}
	}
	return min;
}
int main(){
	nokta *noktalar;
	float *min=(float*)malloc(sizeof(float)*3);
	char dosyaAdi[20];									//dosya adi
	FILE *dosya;
	int n=0;											//eleman sayisi
	printf("Acmak istediginiz dosyanin adini giriniz"); 
	scanf("%s",&dosyaAdi);
	dosya=dosyaAc(&dosyaAdi[0]);
	noktalar=noktalariOku(dosya,&n,noktalar);
	noktaSirala(noktalar,0,n-1);
	printf("\nDizinin siralanmis hali\n");
	noktaYazdir(noktalar,n);
	printf("\n");
	min=enYakinNoktalar(noktalar,0,n-1);				//recursive olarak en yak�n noktalar� hesapl�yoruz 
	min=bulDikdortgen(noktalar,min,n);					
	int koordinat1=(int)min[0];							//float de�eri int e �eviriyoruz
	int koordinat2=(int)min[1];							
	printf("En kisa mesafeli 1.nokta x=%d y=%d   2. nokta x=%d y=%d    uzaklik=%f",noktalar[koordinat1].x,noktalar[koordinat1].y,noktalar[koordinat2].x,noktalar[koordinat2].y,min[2]);

	free(min);
	free(noktalar);
	return 0;
}
