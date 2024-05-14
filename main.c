#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


void cevap_anahtari_uret(char cevap_anahtari[ ], int S){
    srand(time(0)); // Rastgele sayi alma fonksiyonu
    char cevaplar[6]={'A','B','C','D','E','\0'}; // S�navda olusabilecek cevaplar dizisi olusturuldu
    int secim;
    for(int i=0;i<S;i++){
        secim=rand()%5; // 0 ila 5 arasi rastgele sayi uretildi ve secime atandi (dizi icerisinden rastgele indis secebilmek icin)
        cevap_anahtari[i]=cevaplar[secim];  // Cevaplar dizisindeki indise gore cevap anahtar� olusturuldu.
    }
}

void cevap_anahtari_yazdir(char cevap_anahtari[ ], int S){
    printf("\nCevap Anahtari :\n");
    for(int i=0;i<S;i++){
        printf("%d:%c |",i+1,cevap_anahtari[i]); // Cevap anahtari yazdirma islemi.
    }
    printf("\n");
}

void sinavi_uygula(char ogrenci_cevaplari[ ][100], char cevap_anahtari[ ], int N, int S, double B,double D){
    srand(time(0));
    int sayi,sayi2;
    char harf;
    char cevaplar [6]={'A','B','C','D','E'}; // Ogrencilerin isaretleyebilecegi cevaplar dizisi
    for(int i=0;i<N;i++){
        for(int j=0;j<S;j++){
            sayi=rand()%100;
            /*
            kullan�c�dan girdi olarak al�nan b ve d degerlerinin oranlar�yla bir rastgele dogru yanl�s veya bos cevap uretmek icin girilen
            degeri 100 ile carpt�k. Rastgele uret�len 0 ila 100 aras� say�dan ��kan deger 0 ila dogru cevap ihtimali �arp� 1000 ise dogru yapm��
            olarak kabul ettik ve cevap anahtar� �le ayn� deger� g�rd�k. Eger say� Dogru cevap *100 ila bu say�ya b�s cevap*100 deger�n�n eklenmes�
            �le olan say�n�n aras�nda ise bos b�rakma �ht�mal� olarak dusunup ogrenc�n�n cevab�na 'X' girildi. Aksi halde ise yanl�� olma durumuna girip
            while durumuna ald�k bu sayede yanl�s deger girmene kadar donguyu devam ett�rd�k.
            */
            if(0<=sayi && sayi<D*100){ //Dogru olma ihtimali
                ogrenci_cevaplari[i][j]=cevap_anahtari[j];
            }
            else if(D*100<=sayi && sayi<(D+B)*100){ //Bos olma ihtimali
                ogrenci_cevaplari[i][j]='X';
            }
            else{ //Yanlis olma ihtimali
                harf=ogrenci_cevaplari[j];
                while(2<4){
                    sayi2=rand()%5;
                    if(cevaplar[sayi2]==harf)
                        continue;
                    else{
                        ogrenci_cevaplari[i][j]=cevaplar[sayi2];
                        break;
                    }
                }
            }
        }
    }
}

void ogrenci_cevabini_yazdir(char ogrenci_cevaplari[ ][100], int ogrenci_ID, int S){
    printf("\n%d. ogrenicinin cevaplari :\n",ogrenci_ID+1);
    for(int i=0;i<S;i++){
        printf("%d:%c |",i+1,ogrenci_cevaplari[ogrenci_ID][i]);
        //Cevaplari yazd�rd�k
    }
}

void ogrencileri_puanla(char ogrenci_cevaplari[ ][100], char cevap_anahtari[ ],double HBN[ ],int N,int S){
    double dogru=0,yanlis=0;
    double puan;
    for(int i=0;i<N;i++){
        for(int j=0;j<S;j++){
            if(ogrenci_cevaplari[i][j]==cevap_anahtari[j])
                dogru++; //cevap dogru ise de�i�keni artt�rd�k
            else if(ogrenci_cevaplari[i][j]=='X')
                continue; // bos ise bir �ey yapt�kmadk
            else
                yanlis++; // yanlis ise de yanlis degisken�n� artt�rd�k
        }
        double eksi=yanlis/4.0; //her yanl�s 0.25 dogruyu goturur
        dogru-=eksi;
        puan=(100/S)*dogru; // puan� bulduk
        if(puan<0)
            puan=0;
        HBN[i]=puan;
        puan=0; yanlis=0;dogru=0;
        // diger i�lemlerde hata almamak icin dedgerleri 0 lad�k.

    }
}

double sinif_ortalamasi_hesapla(double HBN[ ], int N){
    double toplam=0.0;
    for(int i=0;i<N;i++){
        toplam+=HBN[i]; // Ogrencilerin notlarinin toplamini bulduk
    }
    return (double)toplam/N; // Ogrencilerin not ortalamasini dondurduk.
}

double standart_sapma_hesapla(double ortalama, double HBN[ ], int N){
    double topla=0.0;
    for(int g=0;g<N;g++){
        topla+=pow(HBN[g]-ortalama,2);  //Standart sapma formulunu pow(us) ve sqrt(karekok) fonksiyonlarini kullanarak bulduk.
    }
    topla/=N;
    double std=sqrt(topla);
    if(std==0){
        printf("\nStandart sapma 0 oldugunda t skor hesaplamada payda 0 olacagindan hesaplanamaz.\n");
        exit(0);
        return;
    }
    return std; // Buldugumuz standart sapmayi dondurduk.
}


void T_skoru_hesapla(double ortalama, double HBN[ ], int N, double std, double T_skoru[]){
   double skor;
   for(int k=0;k<N;k++){
       skor=60+10*((HBN[k]-ortalama)/std); // T skoru formulune bagli kalarak her bir ogrencinin t skorunu bulduk.
       T_skoru[k]=skor; // Bulunan t skorlarini diziye aktarma.
   }
}

void harf_notu(double a,double b,double T_skoru[],char harfnotu[][2],int ogrenci_ID){
    /*
    parametre olarak alinan a ve b degerlerine gore tablodaki sinif duzeyine bagli kalarak degismeli bir sekilde not harflendirmesi yapan fonksiyon
    */
    if(T_skoru[ogrenci_ID]<a){
        harfnotu[ogrenci_ID][0]='F';
        harfnotu[ogrenci_ID][1]='F';
    }
    else if(a<=T_skoru[ogrenci_ID] && T_skoru[ogrenci_ID]<=b){
        harfnotu[ogrenci_ID][0]='F';
        harfnotu[ogrenci_ID][1]='D';
    }
    else if(a+5<=T_skoru[ogrenci_ID] && T_skoru[ogrenci_ID]<=b+5){
        harfnotu[ogrenci_ID][0]='D';
        harfnotu[ogrenci_ID][1]='D';
    }
    else if(a+10<=T_skoru[ogrenci_ID] && T_skoru[ogrenci_ID]<=b+10){
        harfnotu[ogrenci_ID][0]='D';
        harfnotu[ogrenci_ID][1]='C';
    }
    else if(a+15<=T_skoru[ogrenci_ID] && T_skoru[ogrenci_ID]<=b+15){
        harfnotu[ogrenci_ID][0]='C';
        harfnotu[ogrenci_ID][1]='C';
    }
    else if(a+20<=T_skoru[ogrenci_ID] && T_skoru[ogrenci_ID]<=b+20){
        harfnotu[ogrenci_ID][0]='C';
        harfnotu[ogrenci_ID][1]='B';
    }
    else if(a+25<=T_skoru[ogrenci_ID] && T_skoru[ogrenci_ID]<=b+25){
        harfnotu[ogrenci_ID][0]='B';
        harfnotu[ogrenci_ID][1]='B';
    }
    else if(a+30<=T_skoru[ogrenci_ID] && T_skoru[ogrenci_ID]<=b+30){
        harfnotu[ogrenci_ID][0]='B';
        harfnotu[ogrenci_ID][1]='A';
    }
    else{
        harfnotu[ogrenci_ID][0]='A';
        harfnotu[ogrenci_ID][1]='A';
    }
}

int main()
{
    int ogrenci_sayisi,soru_sayisi;
    double b_ihtimal,d_ihtimal;
    printf("Ogrenci sayisini giriniz(Max:100):");
    scanf("%d",&ogrenci_sayisi);
    printf("Soru sayisini giriniz(Max:100):");
    scanf("%d",&soru_sayisi);
        if(ogrenci_sayisi>100 || soru_sayisi>100){
            printf("Yanlis parametreler girildi.Programi tekrar baslatin!!!!");
            exit(0);
    }
    printf("Bos birakma ihtimalini giriniz(0.0~1.0):");
    scanf("%lf",&b_ihtimal);
    printf("Dogru cevap verme ihtimalini giriniz(0.0~1.0):");
    scanf("%lf",&d_ihtimal);
    /*Girdi alma islemlerini burada aldik.*/
    char cevap_anahtari[soru_sayisi];
    cevap_anahtari_uret(cevap_anahtari,soru_sayisi);
    cevap_anahtari_yazdir(cevap_anahtari,soru_sayisi);
    char ogrenci_cevaplari[ogrenci_sayisi][100];
    sinavi_uygula(ogrenci_cevaplari,cevap_anahtari,ogrenci_sayisi,soru_sayisi,b_ihtimal,d_ihtimal);
    //Cevap anahtari ve uretme fonksiyonlarini cagirdik.
    for(int k=0;k<ogrenci_sayisi;k++){
        ogrenci_cevabini_yazdir(ogrenci_cevaplari,k,soru_sayisi); // Her bir ogrencinin cevaplarini yazdirdik.
    }
    double toplam=0.0,ortalama;
    double puanlar[ogrenci_sayisi]; // Her bir ogrencinin puanlarini tutacak dizi.
    ogrencileri_puanla(ogrenci_cevaplari,cevap_anahtari,puanlar,ogrenci_sayisi,soru_sayisi);
    ortalama=sinif_ortalamasi_hesapla(puanlar,ogrenci_sayisi);
    printf("\n\nSinifin ortalamasi : %.2lf\t",ortalama);

    double std=standart_sapma_hesapla(ortalama,puanlar,ogrenci_sayisi);
    printf("Standart sapma: %.2lf\n",std);
    printf("Sinif duzeyi: ");

    double a,b;
    /*
    Tabloya gore harf notu hesaplama fonksiyonu icin her duzeye uygun a ve b degerleri atandi ve sinif duzeyleri yazildi.
    */
    if(80<ortalama && ortalama<=100){
        printf("Ustun basari");
        a=32.0; b=36.99;
    }
    else if(70<ortalama && ortalama<=80){
        printf("Mukemmel");
        a=34.0; b=38.99;
    }
    else if(62.5<ortalama && ortalama<=70){
        printf("Cok iyi");
        a=36.0; b=40.99;
    }
    else if(57.5<ortalama && ortalama<=62.5){
        printf("Iyi");
        a=38.0; b=42.99;
    }
    else if(52.5<ortalama && ortalama<=57.5){
        printf("Ortanin ustu");
        a=40.0; b=44.99;
    }
    else if(47.5<ortalama && ortalama<=52.5){
        printf("Orta");
        a=42.0; b=46.99;
    }
    else if(42.5<ortalama && ortalama<=47.5){
        printf("Zayif");
        a=44.0; b=48.99;
    }
    else if(ortalama<=42.5){
        printf("Kotu");
        a=46.0; b=50.99;
    }

    printf("\n\n");

    double t_skor[ogrenci_sayisi];
    T_skoru_hesapla(ortalama,puanlar,ogrenci_sayisi,std,t_skor);
    char harf_notuu[ogrenci_sayisi][2];
    for(int i=0;i<ogrenci_sayisi;i++){
        harf_notu(a,b,t_skor,harf_notuu,i);
        //Sinif duzeyine gore atama yapilan a ve b degerleri harf notu fonksiyonuna parametre olarak gonderildi ve harf notu matrisine her ogrencinin
        //harf notu atandi.
    }
    printf("Ogrenci Notlari :\n");

    for(int gk=0;gk<ogrenci_sayisi;gk++){
        printf("\n%d. ogrencinin HBN: %.2lf, T-skor: %.2lf,Harf Notu: ",gk+1,puanlar[gk],t_skor[gk]);
            for(int i=0;i<2;i++)
                  printf("%c",harf_notuu[gk][i]);
                  //Buraya kadar fonksiyonlarla buldugumuz tum degerleri yazdirma islemi.
    }
    return 0;
}
