
//K�t�phane Denklarasyonu
#include <iostream>
#include <mpi.h>//mpi k�t�phanesi
#include <fstream>//dosya i�lemleri i�in 
#include <sys/time.h>//zaman hesab� i�in 
#include <cstdlib>
#include <ctime>
#include <string>

//Fonksiyon Protetipleri=Program�m tek bir main fonksiyonu ile �al��maktad�r

#define TAG1 8  //tTAG1 makromuzu tan�mlad�k
#define TAG2 34 //TAG2 makromuzu tan�mlad�k
#define TAG3 16 //TAG3 makromuzu tan�mlad�k
#define TAG4 27 //TAG4 makromuzu tan�mlad�k
#define TAG5 40 //TAG5 makromuzu tan�mlad�k (bunlar� veri payla�t�r�rken ullan�caz hangi tagin hangi tage gitti�ini belirtmek i�in)

using namespace std;
//Main fonksiyonu 
int main(int argc, char* argv[]) {//main fonksiyonunu a�t�k
	  double startTime,endTime,elapsedTime;//zaman hesab� i�in de�i�kenlerimizi tan�mlad�k
	  int * goruntuMatrisiPointer;//filtrelenmemi� matrisin tutulca�� tek boyutlu pointer dizi(�zerinde filtre uygulucaz.)
	  int * yedekGoruntuMatrisiPointer;//bu diziyi yedeklice�imiz yine tek boyutlu bir pointer dizi(�zerinde filtre uygulam�caz direk g�ndericez)
	  int *lokalDiziPointer;//diziyi master bilgisayardan di�er bilgisayarlarar da��tt�ktan sonra o dizinin yede�ini alaca��m�z tek boyutlu bir pointer dizi(�zerinde filtre uygulam�caz)
	  int *lokalDiziPointer2;//Di�er bilglisayarlardan filtrelenmi� de�erleri sakl�ca��m�z yine tek boyutlu bir pointer dizi(filtre uygulam�caz bunun �zerinde)
      int *kopyagoruntuMatrisiPointer2;//filtrelenmemi� diziyi b�l��t�r�p masterdan di�erlerine att���m�zda di�erlerinin o de�erleri al�p kendi �zerine yazd��� tek boyutlu pointer dizi
	  int kismiSatirBoyutu,kismiToplamBoyut;//sat�rlar�n b�l�mlenmi� de�erlerini ve toplam boyutun b�l�mlenmi� de�erlerini saklad���m de�i�kenler
	  int geciciDegisken,geciciDegisken2;//biri master di�eri di�er bilgisayarlar i�in filtre uygularken kulland���m�z iki adet ge�ici de�i�ken 
	  int siralamaMatrisi[25];//yine biri master bilgisayar i�n s�ralaamay� saklad���m�z tek boyutlu normal dizi
	  int siralamaMatrisi2[25];//di�eride di�er bilgisayarlar i�in s�ralamay� saklad���m�z tek boyutlu normal dizi
      int indis;//yine programda belirli yerlerde indis atamalar� i�in tan�mlad���m bi indis de�i�keni.
      int kismiSatirBoyutu2,kismiToplamBoyut2;//kalan hesab� i�in de�i�kenler
      int kismiSatirKalan;//kalan hesab� i�in bir di�er de�i�ken
      int satirlar;//satirlar degiskeni
	  int sutunlar;//sutunlar degiskeni
	  int myRank;//myRank de�i�keni
	  int size;//size de�i�keni
	  MPI_Status status;//status sabiti
	  MPI_Init(&argc,&argv);//program� ilklendirdik
	  MPI_Comm_rank(MPI_COMM_WORLD,&myRank);//rank bilgisini (yani rank bilgisi hangi bilgisauyarda oldu�umuz) ald�k
	  MPI_Comm_size(MPI_COMM_WORLD,&size);//size bilgisini ald�k bilgisayarlar�n boyutu yani ka� adet oldu�u.
if(myRank==0)//e�er master bilgisayar�n i�indeysek.
{
	  	ifstream okunacakDosya;//okunacakdosya ad�nda i�inde veri alabildi�imiz bir dosya olu�turduk ifstream ile
	  	okunacakDosya.open(argv[1]);//dosyay� a�t�k;argv1 ile a�t�k ��nk� �al��t�r�rken komut sat�r�ndan girdi�imiz ilk parametre argv1 olucak  
	  	okunacakDosya>>satirlar;//satirlar de�i�kenine okunacakDosya ad�ndaki dosyan�n ilk verisini kaydettik
	    okunacakDosya>>sutunlar;//sutunlar de�i�kenine okunacakDosya ad�ndaki dosyan�n ikinci verisini kaydettik
	    goruntuMatrisiPointer = new int[satirlar*sutunlar];//goruntuMatrisiPointer � yukarda tan�mlam��t�k burdada tek boyutlu bir dinamik dizi haaline getirdik.
	    yedekGoruntuMatrisiPointer = new int[satirlar*sutunlar];//yine ayn� �ekilde yedekGoruntuMatrisiPointer � yukarda tan�mlam��t�k burdada tek boyutlu bir dinamik dizi haaline getirdik
	    int atanacakIndisler;//goruntu matrisinin i�ine ataca��m�z indisler i�in bir de�i�ken tan�mlad�k
	    int matrisinIndisi=0;//matris indisini 0 dan ba�latt�k
            for(int s1=0;s1<satirlar;s1++)//satirlarin hepsini dola�acak d��taki d�ng�y� tan�mlad�k
            {
    	      for(int s2=0;s2<sutunlar;s2++)//s�tunlar�n hepsini dola��cak i�teki d�ng�y� tan�mlad�k
    	      {
    	        okunacakDosya>>atanacakIndisler;//atanacak indisler de�i�kenini ald�k bunu sat�r ve s�tunun yaz�l� oldu�u sat�rdan de�ilde bir alt sat�rdan ba�lamas�
    	        //i�in yapt�k  bu her okundu�unda bir artarak devam edicek yani matristeki her eleman�n bir sa��ndaki indisi al�cak hep
    	        goruntuMatrisiPointer[matrisinIndisi]=atanacakIndisler;//o elemanlar�da matrisin 0 dan sat�r ve s�tun say�s� �arp�m�na kadar b�t�n indislerine at�cak
    	        yedekGoruntuMatrisiPointer[matrisinIndisi]=goruntuMatrisiPointer[matrisinIndisi];//Diziyi bir baa�ka dizide yedekliyoruz.
    	        matrisinIndisi++;//matrisin indisini 1 artt�r�yoruz her iterasyonda
	          }
            }
        okunacakDosya.close();//okuma i�lemlerimiz bittikten sonra dosyam�z� kapat�yoruz
        startTime=MPI_Wtime();//S�reyi hesaplamak i�in MPI_Wtime komutunu �a��r�yoruz.S�reyi hesaplamaya ba�l�cak
        if((satirlar*sutunlar)%size==0)//E�er matris sat�r ve s�tun say�s�na tam b�l�m�yorsa 
        {
		    kismiSatirBoyutu=(satirlar/size);//sat�r boyutunu b�lerek bir de�i�kende tutmam�z laz�mki her bilgisayar kendisinin filtre yapt��� sat�r say�s�n� bilmeli.
        	kismiToplamBoyut=(satirlar/size)*sutunlar;//ayn� zamanda toplamboyutunda b�l�nm�� de�erini bulmam gerekiyo ki master bilgisayardan di�erlerine bu de�eri atmam gerekicek.
        	//��nk� her bilgisayar kendisine ayr�lm�� k�smiboyutu bir dizide de�erleriyle birlikte(filtrelenmemi� matrisin) saklamas� gerekicek ki filtreleme yapabilsin her bilgisayar
        	for(int i=1;i<size;i++)//master bilgisayardan 1 den b�t�n di�er makinelere(ka� makine varsa art�k) veriyi g�ndermek i�in bir for a�t�k
        	{
        		MPI_Send(&kismiSatirBoyutu,1,MPI_INT,i,TAG2,MPI_COMM_WORLD);//k�smiSatirBoyutu bilgisi b�t�n bilgisayarlara g�nderildi
        		MPI_Send(&sutunlar,1,MPI_INT,i,TAG3,MPI_COMM_WORLD);//s�tunlar�n bilgisi b�t�n bilgisayarlara g�nderildi
				MPI_Send(&kismiToplamBoyut,1,MPI_INT,i,TAG1,MPI_COMM_WORLD);//k�smitoplam boyutta b�t�n bilgisayarlara g�nderildi
        	    MPI_Send(&yedekGoruntuMatrisiPointer[i*kismiToplamBoyut],kismiToplamBoyut,MPI_INT,i,TAG4,MPI_COMM_WORLD);//yedekledi�imiz matris de di�er bilgisayarlara g�nderildi
			}
			for(int s1=2;s1<kismiSatirBoyutu-1;s1++)//indisi 2 den ba�lat�p k�smiSat�rBoyutunun bir eksi�ine g�nderiyoruz b�ylece master bilgisayar ilk iki sat�r� ve sok iki sat�r� alm�cak
			{
				for(int s2=2;s2<sutunlar-1;s2++)//s�tunlar�nda 1 eksi�ine kadar gidiyoruz son iki s�tunu ve ilk iki s�tunu almamas� i�in he bide <= de�i < dikkat. d��taki fordada �lyle
				{
					 //s�ralamaMatrisinin 0 indisinden 24 . indisine yani toplam 25 indise filtreleme uygulaca��m�z indisleri yani komu�u de�erlerini at�yoruz. Detayl� a��klamas�n�
					 //ilk projede yapm��t�m 
					 siralamaMatrisi[0]=goruntuMatrisiPointer[(s1-2)*sutunlar+s2-2];
	   	             siralamaMatrisi[1]=goruntuMatrisiPointer[(s1-2)*sutunlar+s2-1];
	   	             siralamaMatrisi[2]=goruntuMatrisiPointer[(s1-2)*sutunlar+s2];
	   	             siralamaMatrisi[3]=goruntuMatrisiPointer[(s1-2)*sutunlar+s2+1];
	   	             siralamaMatrisi[4]=goruntuMatrisiPointer[(s1-2)*sutunlar+s2+2];
	   	             siralamaMatrisi[5]=goruntuMatrisiPointer[(s1-1)*sutunlar+s2-2];
	   	             siralamaMatrisi[6]=goruntuMatrisiPointer[(s1-1)*sutunlar+s2-1];
	   	             siralamaMatrisi[7]=goruntuMatrisiPointer[(s1-1)*sutunlar+s2];
	   	             siralamaMatrisi[8]=goruntuMatrisiPointer[(s1-1)*sutunlar+s2+1];
	   	             siralamaMatrisi[9]=goruntuMatrisiPointer[(s1-1)*sutunlar+s2+2];
	   	             siralamaMatrisi[10]=goruntuMatrisiPointer[(s1)*sutunlar+s2-2];
	   	             siralamaMatrisi[11]=goruntuMatrisiPointer[(s1)*sutunlar+s2-1];
	   	             siralamaMatrisi[12]=goruntuMatrisiPointer[(s1)*sutunlar+s2];
	   	             siralamaMatrisi[13]=goruntuMatrisiPointer[(s1)*sutunlar+s2+1];
	   	             siralamaMatrisi[14]=goruntuMatrisiPointer[(s1)*sutunlar+s2+2];
	   	             siralamaMatrisi[15]=goruntuMatrisiPointer[(s1+1)*sutunlar+s2-2];
	   	             siralamaMatrisi[16]=goruntuMatrisiPointer[(s1+1)*sutunlar+s2-1];
	   	             siralamaMatrisi[17]=goruntuMatrisiPointer[(s1+1)*sutunlar+s2];
	   	             siralamaMatrisi[18]=goruntuMatrisiPointer[(s1+1)*sutunlar+s2+1];
	   	             siralamaMatrisi[19]=goruntuMatrisiPointer[(s1+1)*sutunlar+s2+2];
	   	             siralamaMatrisi[20]=goruntuMatrisiPointer[(s1+2)*sutunlar+s2-2];
	   	             siralamaMatrisi[21]=goruntuMatrisiPointer[(s1+2)*sutunlar+s2-1];
	   	             siralamaMatrisi[22]=goruntuMatrisiPointer[(s1+2)*sutunlar+s2];
	   	             siralamaMatrisi[23]=goruntuMatrisiPointer[(s1+2)*sutunlar+s2+1];
	   	             siralamaMatrisi[24]=goruntuMatrisiPointer[(s1+2)*sutunlar+s2+2];
					 //bubble sort  algoritmas�yla k���kten b�y��e s�ralama yap�yoruz 	
					 for(int i=0;i<25;i++)
	   	             {
	   	     	      for(int j=i;j<25;j++)
	   	 	          {
	   	 		       if(siralamaMatrisi[i]>siralamaMatrisi[j])
	   	 		       {
	   	 		          geciciDegisken=siralamaMatrisi[i];
	   	 		          siralamaMatrisi[i]=siralamaMatrisi[j];
	   	 		          siralamaMatrisi[j]=geciciDegisken;
	   	 	           }
			          }
	   	 	
		              }	
					yedekGoruntuMatrisiPointer[s1*sutunlar+s2]=siralamaMatrisi[12];//ve ortandca de�eri ilgili indize at�yoruz
				  }
		 	}	
			lokalDiziPointer2 = new int[kismiToplamBoyut];//master bilgisayardan gelen filtrlenmi� verileri bir dizide tutmak i�in 2 numaral� lokalpointer adl� dinamik dizi olu�turuyoruz
			for(int i=1;i<size;i++)//makine say�s�na kadar gidip
			{
				MPI_Recv(lokalDiziPointer2,kismiToplamBoyut,MPI_INT,i,TAG5,MPI_COMM_WORLD,&status);//o filtrelenmi� de�erleri her makineden tek tek al�yoruz.
				indis=i*((satirlar/size)*sutunlar);//master 0 �n kendi kald��� yerden kopyal�ca��m�z i�in indise kal�nan yerlerin belirtildi�i bir de�er at�yoruz
				for(int s=0;s<kismiToplamBoyut;s++)//ve  0 dan  k�smitoplamboyut kadar dola��p
				{
					yedekGoruntuMatrisiPointer[indis+s]=lokalDiziPointer2[s];//her bilgisayar�n kald��� yerden(master 0 da dahil olmak �zere) indisleri yedekGoruntuMatrisiPointer 
					//adl� de�i�kene at�yoruz ekrana yazd�rca��m�z son filtrelenmi� matriste bu olucak
				}
			}
		  } 
		else if((satirlar*sutunlar)%size!=0)//e�er bilgisayarlara tam b�l�nm�yorsa
		{
			kismiSatirKalan=satirlar%size;//b�l�mden kalan� al�p bir de�i�kende tuttuk
			kismiSatirBoyutu=(satirlar/size);//sat�r boyutunu b�lerek bir de�i�kende tutmam�z laz�mki her bilgisayar kendisinin filtre yapt��� sat�r say�s�n� bilmeli.
			kismiSatirBoyutu2=(satirlar/size)+kismiSatirKalan;//kalan la b�l�m� toplad�k
        	kismiToplamBoyut=kismiSatirBoyutu*sutunlar;//ayn� zamanda toplamboyutunda b�l�nm�� de�erini bulmam gerekiyo ki master bilgisayardan di�erlerine bu de�eri atmam gerekicek.
        	kismiToplamBoyut2=((satirlar/size)+kismiSatirKalan)*sutunlar;//kalanla b�l�m�n toplam�ndan gelen de�eri sutun de�eeriyle �arpt�k
        	//��nk� her bilgisayar kendisine ayr�lm�� k�smiboyutu bir dizide de�erleriyle birlikte(filtrelenmemi� matrisin) saklamas� gerekicek ki filtreleme yapabilsin her bilgisayar
        	for(int i=1;i<size;i++)//master bilgisayardan 1 den b�t�n di�er makinelere(ka� makine varsa art�k) veriyi g�ndermek i�in bir for a�t�k
        	{
        		//normalde burada kalan�n dahil oldu�u k�smi bilgileri g�ndermem gerekiyodu ama �yle yap�nca hata all�yordum normal de�erleri g�nderdim di�er bilgisayarlara
        		MPI_Send(&kismiSatirBoyutu,1,MPI_INT,i,TAG2,MPI_COMM_WORLD);//k�smiSatirBoyutu bilgisi b�t�n bilgisayarlara g�nderildi
        		MPI_Send(&sutunlar,1,MPI_INT,i,TAG3,MPI_COMM_WORLD);//s�tunlar�n bilgisi b�t�n bilgisayarlara g�nderildi
				MPI_Send(&kismiToplamBoyut,1,MPI_INT,i,TAG1,MPI_COMM_WORLD);//k�smitoplam boyutta b�t�n bilgisayarlara g�nderildi
        	    MPI_Send(&yedekGoruntuMatrisiPointer[i*kismiToplamBoyut],kismiToplamBoyut,MPI_INT,i,TAG4,MPI_COMM_WORLD);//yedekledi�imiz matris de di�er bilgisayarlara g�nderildi
			}
			for(int s1=2;s1<kismiSatirBoyutu-1;s1++)//indisi 2 den ba�lat�p k�smiSat�rBoyutunun bir eksi�ine g�nderiyoruz b�ylece master bilgisayar ilk iki sat�r� ve sok iki sat�r� alm�cak
			{
				for(int s2=2;s2<sutunlar-1;s2++)//s�tunlar�nda 1 eksi�ine kadar gidiyoruz son iki s�tunu ve ilk iki s�tunu almamas� i�in he bide <= de�i < dikkat. d��taki fordada �lyle
				{
					 //s�ralamaMatrisinin 0 indisinden 24 . indisine yani toplam 25 indise filtreleme uygulaca��m�z indisleri yani komu�u de�erlerini at�yoruz. Detayl� a��klamas�n�
					 //ilk projede yapm��t�m 
					 siralamaMatrisi[0]=goruntuMatrisiPointer[(s1-2)*sutunlar+s2-2];
	   	             siralamaMatrisi[1]=goruntuMatrisiPointer[(s1-2)*sutunlar+s2-1];
	   	             siralamaMatrisi[2]=goruntuMatrisiPointer[(s1-2)*sutunlar+s2];
	   	             siralamaMatrisi[3]=goruntuMatrisiPointer[(s1-2)*sutunlar+s2+1];
	   	             siralamaMatrisi[4]=goruntuMatrisiPointer[(s1-2)*sutunlar+s2+2];
	   	             siralamaMatrisi[5]=goruntuMatrisiPointer[(s1-1)*sutunlar+s2-2];
	   	             siralamaMatrisi[6]=goruntuMatrisiPointer[(s1-1)*sutunlar+s2-1];
	   	             siralamaMatrisi[7]=goruntuMatrisiPointer[(s1-1)*sutunlar+s2];
	   	             siralamaMatrisi[8]=goruntuMatrisiPointer[(s1-1)*sutunlar+s2+1];
	   	             siralamaMatrisi[9]=goruntuMatrisiPointer[(s1-1)*sutunlar+s2+2];
	   	             siralamaMatrisi[10]=goruntuMatrisiPointer[(s1)*sutunlar+s2-2];
	   	             siralamaMatrisi[11]=goruntuMatrisiPointer[(s1)*sutunlar+s2-1];
	   	             siralamaMatrisi[12]=goruntuMatrisiPointer[(s1)*sutunlar+s2];
	   	             siralamaMatrisi[13]=goruntuMatrisiPointer[(s1)*sutunlar+s2+1];
	   	             siralamaMatrisi[14]=goruntuMatrisiPointer[(s1)*sutunlar+s2+2];
	   	             siralamaMatrisi[15]=goruntuMatrisiPointer[(s1+1)*sutunlar+s2-2];
	   	             siralamaMatrisi[16]=goruntuMatrisiPointer[(s1+1)*sutunlar+s2-1];
	   	             siralamaMatrisi[17]=goruntuMatrisiPointer[(s1+1)*sutunlar+s2];
	   	             siralamaMatrisi[18]=goruntuMatrisiPointer[(s1+1)*sutunlar+s2+1];
	   	             siralamaMatrisi[19]=goruntuMatrisiPointer[(s1+1)*sutunlar+s2+2];
	   	             siralamaMatrisi[20]=goruntuMatrisiPointer[(s1+2)*sutunlar+s2-2];
	   	             siralamaMatrisi[21]=goruntuMatrisiPointer[(s1+2)*sutunlar+s2-1];
	   	             siralamaMatrisi[22]=goruntuMatrisiPointer[(s1+2)*sutunlar+s2];
	   	             siralamaMatrisi[23]=goruntuMatrisiPointer[(s1+2)*sutunlar+s2+1];
	   	             siralamaMatrisi[24]=goruntuMatrisiPointer[(s1+2)*sutunlar+s2+2];
					 //bubble sort  algoritmas�yla k���kten b�y��e s�ralama yap�yoruz 	
					 for(int i=0;i<25;i++)
	   	             {
	   	     	      for(int j=i;j<25;j++)
	   	 	          {
	   	 		       if(siralamaMatrisi[i]>siralamaMatrisi[j])
	   	 		       {
	   	 		          geciciDegisken=siralamaMatrisi[i];
	   	 		          siralamaMatrisi[i]=siralamaMatrisi[j];
	   	 		          siralamaMatrisi[j]=geciciDegisken;
	   	 	           }
			          }
	   	 	
		              }	
					yedekGoruntuMatrisiPointer[s1*sutunlar+s2]=siralamaMatrisi[12];//ve ortandca de�eri ilgili indize at�yoruz
				  }
		 	}	
			lokalDiziPointer2 = new int[kismiToplamBoyut];//master bilgisayardan gelen filtrlenmi� verileri bir dizide tutmak i�in 2 numaral� lokalpointer adl� dinamik dizi olu�turuyoruz
			for(int i=1;i<size;i++)//makine say�s�na kadar gidip
			{
				MPI_Recv(lokalDiziPointer2,kismiToplamBoyut,MPI_INT,i,TAG5,MPI_COMM_WORLD,&status);//o filtrelenmi� de�erleri her makineden tek tek al�yoruz.
				indis=i*((satirlar/size)*sutunlar);//master 0 �n kendi kald��� yerden kopyal�ca��m�z i�in indise kal�nan yerlerin belirtildi�i bir de�er at�yoruz
				for(int s=0;s<kismiToplamBoyut;s++)//ve  0 dan  k�smitoplamboyut kadar dola��p
				{
					yedekGoruntuMatrisiPointer[indis+s]=lokalDiziPointer2[s];//her bilgisayar�n kald��� yerden(master 0 da dahil olmak �zere) indisleri yedekGoruntuMatrisiPointer 
					//adl� de�i�kene at�yoruz ekrana yazd�rca��m�z son filtrelenmi� matriste bu olucak
				}
			}
		  } 
			

		
        endTime=MPI_Wtime();	//hesaplama s�resini bitiriyoruz
		elapsedTime=endTime-startTime;//aradaki farkl� al�p bir de�i�kene at�yoruz
		cout<<"Paralel Programlama Suresi: "<<elapsedTime*1000<<endl;//o de�i�keni de ilgili formatta yazd�r�yoruz.
	    
		
	        char ciktiDosyasi[28];//Filtrelenmi� matrisin ��kt� halini char dizisi �eklinde tan�mlad�k �rn:GoruntuMatrisi1
    	    string ciktiDosyasiUzantisi= "_filtered.txt";//arg�man�n sonuna uzant� eklemek i�in de�i�ken tan�mlad�k
    	    for(int s1=0; s1<15; s1++)//0 dan 15 e kadar gidicek for 
	        {
	          ciktiDosyasi[s1]=argv[1][s1+22];// arg�man�n toplam (.txt hari�) 38 karakteri var biz sadece GoruntuMatrisi1 �rne�i �eklinde 15 karakterli bir de�er atad�k	
	        }
	        for(int s1=0; s1<14; s1++)//sonra o 15 de�erli karakterin sonuna 15 ten ba�layarak(15 ten alttaki forda ba�l�yor) (devam� altta ki a��klama sat�r�nda)
	        {
	          ciktiDosyasi[s1+15] = ciktiDosyasiUzantisi[s1];//uzant�y� atad�k art�k i�inde GoruntuMatrisi1_filtered.txt �rne�i �eklinde bir de�er olmu� oldu
	        } 								
		
	        ofstream yazilacakDosya;//yazilacak dosyay� ��kt� dosyas� olarak ayarlad�k
	        yazilacakDosya.open(ciktiDosyasi, ios::out);//burda char dizisini �evirme i�lemi ve yaz�lacak dosyay� a�ma i�lemi yap�yoruz
            //yazilacakDosya<<satirlar;
            //yazilacakDosya<<sutunlar;
	        for(int s1=0; s1<satirlar; s1++)//sat�rlara kadar giden bir d�ng�
            {
               for(int s2=0; s2<sutunlar; s2++)//s�tunlara kadar giden bir d�ng�
	           {
		           yazilacakDosya<<yedekGoruntuMatrisiPointer[s1*sutunlar+s2]<<"\t";//yaz�lacak matris de�erleri matrisin ilk sat�r ve ilk s�tunundan ba�l�cak
                  //dikkat et matrisin sat�r ve s�tun bilgisinin yazd��� sat�r� alm�cak onu almak isteseydik bu sat�r�n �st�ndeki 7.ve 8.s�tunu
                  //sat� ve s�tun say�s� bilgisini yazd�rd���m�z k�sm� a��klama sat�r�ndan kald�rmam�z gerekirdi
					
	           }
	           yazilacakDosya<<endl;//matris format�nda yazd�rmak i�in her sat�rdan sonra bir alta iniyoruz.
	        }	

	    yazilacakDosya.close();//yaz�lacak dosyay� kapatt�k 
	    delete [] goruntuMatrisiPointer;//dinamnik diziyi sildik(filtreleme yapt���m�z)
	    delete [] yedekGoruntuMatrisiPointer;//di�er bilgisayarlara veri g�ndermek i�in kulland���m�z dinamikdiziyide sildik.
	

}	

else//bu elseyi di�er bilgisayarlar i�in a��yoruz yani veriyi masterdan g�nderdik o veriyi alacak bilglsayarlar i�in a��yorruz
{
         	MPI_Recv(&kismiSatirBoyutu,1,MPI_INT,0,TAG2,MPI_COMM_WORLD,&status);//master bilgisayardan g�nderilen k�smiSatirBoyutu bilgisini ald�k
        	MPI_Recv(&sutunlar,1,MPI_INT,0,TAG3,MPI_COMM_WORLD,&status);//master bilgisayardan g�nderilen s�tun bilgisini ald�k 
	      	MPI_Recv(&kismiToplamBoyut,1,MPI_INT,0,TAG1,MPI_COMM_WORLD,&status);//ve yine masterdan gelen k�smiToplamBoyut bilgisini ald�k
           
		   
		    kopyagoruntuMatrisiPointer2=new int[kismiToplamBoyut];//gelen veriyi bir dinamik diziye kopyalamak i�in dinamik dizi tan�mlad�k
	        MPI_Recv(kopyagoruntuMatrisiPointer2,kismiToplamBoyut,MPI_INT,0,TAG4,MPI_COMM_WORLD,&status);//master bilgisayardan gelen diziyi kopyaMatrisiPointer2 ye at�yoruz
	        //bu gelen veri b�l�mlendirilmi� veri yani 50 ye 50 bi matrisse ilk 10 sat�r� gelicek her bilgisayaraa 10 ar sat�r gidicek yani
	        lokalDiziPointer=new int[kismiToplamBoyut];//birtane daha dinamik dizi olu�turuyoruz filtreledi�imiz de�erleri bunda sakl�caz.
	        for(int i=0;i<kismiToplamBoyut;i++)//bu tan�mlad���m�z diziyi k�smiToplamBouta kadar dola��yoruz
	        {
	        	lokalDiziPointer[i]=kopyagoruntuMatrisiPointer2[i];//i�ine master 0 dan ald���m�z ve i�ine de�er yazd���m�z kopyamatrisinin indislerini at�yoruz tek tek.
			}
				for(int s1=2;s1<kismiSatirBoyutu-1;s1++)//yine ayn� �ekilde di�er bilgisayarlar�n yapaca�� filte i�lemi i�in d�� for tan�ml�yoruz k�smiBoyutun bir eksi�ine kadar gidicek
				//ve 2 den ba�l�cak buda ilk iki sat�r ve son iki sat�r� almam�z� sa�l�cak bu nedenle resmin b�l�tlendirildi�i olu�turulan resimde biraaz belli olucazk
				//aradaki �izgilerden anla��ld��� gibi.
			    {
				 for(int s2=2;s2<sutunlar-1;s2++)//s�tunlar� da yine 2 den ba�lay�p bir eksi�ine kadar dola��yoruz s�tunda s�k�nt� yok s�tunlar�n b�l��t�r�ld��� belli olmucak
				 //��nk� sat�r odakl� bi b�lme i�lemi yapm��t�k
				 {
				     //filtrelemeyi yap�yoruz ve bu sefer 2 numaral� s�ralama matrisine at�yoruz de�erleri ilkiyle kar��mas�n diye.
					 siralamaMatrisi2[0]=kopyagoruntuMatrisiPointer2[(s1-2)*sutunlar+s2-2];
	   	             siralamaMatrisi2[1]=kopyagoruntuMatrisiPointer2[(s1-2)*sutunlar+s2-1];
	   	             siralamaMatrisi2[2]=kopyagoruntuMatrisiPointer2[(s1-2)*sutunlar+s2];
	   	             siralamaMatrisi2[3]=kopyagoruntuMatrisiPointer2[(s1-2)*sutunlar+s2+1];
	   	             siralamaMatrisi2[4]=kopyagoruntuMatrisiPointer2[(s1-2)*sutunlar+s2+2];
	   	             siralamaMatrisi2[5]=kopyagoruntuMatrisiPointer2[(s1-1)*sutunlar+s2-2];
	   	             siralamaMatrisi2[6]=kopyagoruntuMatrisiPointer2[(s1-1)*sutunlar+s2-1];
	   	             siralamaMatrisi2[7]=kopyagoruntuMatrisiPointer2[(s1-1)*sutunlar+s2];
	   	             siralamaMatrisi2[8]=kopyagoruntuMatrisiPointer2[(s1-1)*sutunlar+s2+1];
	   	             siralamaMatrisi2[9]=kopyagoruntuMatrisiPointer2[(s1-1)*sutunlar+s2+2];
	   	             siralamaMatrisi2[10]=kopyagoruntuMatrisiPointer2[(s1)*sutunlar+s2-2];
	   	             siralamaMatrisi2[11]=kopyagoruntuMatrisiPointer2[(s1)*sutunlar+s2-1];
	   	             siralamaMatrisi2[12]=kopyagoruntuMatrisiPointer2[(s1)*sutunlar+s2];
	   	             siralamaMatrisi2[13]=kopyagoruntuMatrisiPointer2[(s1)*sutunlar+s2+1];
	   	             siralamaMatrisi2[14]=kopyagoruntuMatrisiPointer2[(s1)*sutunlar+s2+2];
	   	             siralamaMatrisi2[15]=kopyagoruntuMatrisiPointer2[(s1+1)*sutunlar+s2-2];
	   	             siralamaMatrisi2[16]=kopyagoruntuMatrisiPointer2[(s1+1)*sutunlar+s2-1];
	   	             siralamaMatrisi2[17]=kopyagoruntuMatrisiPointer2[(s1+1)*sutunlar+s2];
	   	             siralamaMatrisi2[18]=kopyagoruntuMatrisiPointer2[(s1+1)*sutunlar+s2+1];
	   	             siralamaMatrisi2[19]=kopyagoruntuMatrisiPointer2[(s1+1)*sutunlar+s2+2];
	   	             siralamaMatrisi2[20]=kopyagoruntuMatrisiPointer2[(s1+2)*sutunlar+s2-2];
	   	             siralamaMatrisi2[21]=kopyagoruntuMatrisiPointer2[(s1+2)*sutunlar+s2-1];
	   	             siralamaMatrisi2[22]=kopyagoruntuMatrisiPointer2[(s1+2)*sutunlar+s2];
	   	             siralamaMatrisi2[23]=kopyagoruntuMatrisiPointer2[(s1+2)*sutunlar+s2+1];
	   	             siralamaMatrisi2[24]=kopyagoruntuMatrisiPointer2[(s1+2)*sutunlar+s2+2];
					 //s�ralad���m�z de�erleri yine bubble sort ile s�ral�yoruz .S�ralama yaparkende 2 numaral� gecicide�i�keni kulland�k.	
					 for(int i=0;i<25;i++)
	   	             {
	   	     	      for(int j=i;j<25;j++)
	   	 	          {
	   	 		       if(siralamaMatrisi2[i]>siralamaMatrisi2[j])
	   	 		       {
	   	 		          geciciDegisken2=siralamaMatrisi2[i];
	   	 		          siralamaMatrisi2[i]=siralamaMatrisi2[j];
	   	 		          siralamaMatrisi2[j]=geciciDegisken2;
	   	 	           }
			          }
	   	 	
		             }	
					lokalDiziPointer[s1*sutunlar+s2]=siralamaMatrisi2[12];//ve ortancas�n� bulup b�t�n indislere her iterasyonda yukarda oluiturdu�umuz dinamik dizinin i�ine at�yoruz.
					//o dizinin i�inde filtrelenmemi� de�erler vard� art�k filtrelenmi� de�erler olucak. 
				  }
				
		 	    }
			MPI_Send(lokalDiziPointer,kismiToplamBoyut,MPI_INT,0,TAG5,MPI_COMM_WORLD);//filtrelenmi� de�erleri master�nda yapt��� filtrelemeyle birle�tirmek i�in mastera g�nderiyoruz.	
	        delete [] kopyagoruntuMatrisiPointer2;//i�ine de�er master bilgisayardan ald���m�z de�erleri saklad���m�z dinamik diziyi siliyoruz
	        delete [] lokalDiziPointer;//ilk ba�ta filtrelenemi� de�erleri saklamak i�in kulland���m�z  ve sonra i�ini filtrelenmi� de�erle doldurdu�umuz dinamik dizimizi siliyoruz

}       
        
    	MPI_Finalize();//mp� i�lemlerini ilklendirdi�imiz gibi sonland�r�yoruz
		return 0;//program� bitiriyoruz
}
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
	

	
