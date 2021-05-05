
//Kütüphane Denklarasyonu
#include <iostream>
#include <mpi.h>//mpi kütüphanesi
#include <fstream>//dosya iþlemleri için 
#include <sys/time.h>//zaman hesabý için 
#include <cstdlib>
#include <ctime>
#include <string>

//Fonksiyon Protetipleri=Programým tek bir main fonksiyonu ile çalýþmaktadýr

#define TAG1 8  //tTAG1 makromuzu tanýmladýk
#define TAG2 34 //TAG2 makromuzu tanýmladýk
#define TAG3 16 //TAG3 makromuzu tanýmladýk
#define TAG4 27 //TAG4 makromuzu tanýmladýk
#define TAG5 40 //TAG5 makromuzu tanýmladýk (bunlarý veri paylaþtýrýrken ullanýcaz hangi tagin hangi tage gittiðini belirtmek için)

using namespace std;
//Main fonksiyonu 
int main(int argc, char* argv[]) {//main fonksiyonunu açtýk
	  double startTime,endTime,elapsedTime;//zaman hesabý için deðiþkenlerimizi tanýmladýk
	  int * goruntuMatrisiPointer;//filtrelenmemiþ matrisin tutulcaðý tek boyutlu pointer dizi(üzerinde filtre uygulucaz.)
	  int * yedekGoruntuMatrisiPointer;//bu diziyi yedekliceðimiz yine tek boyutlu bir pointer dizi(üzerinde filtre uygulamýcaz direk göndericez)
	  int *lokalDiziPointer;//diziyi master bilgisayardan diðer bilgisayarlarar daðýttýktan sonra o dizinin yedeðini alacaðýmýz tek boyutlu bir pointer dizi(üzerinde filtre uygulamýcaz)
	  int *lokalDiziPointer2;//Diðer bilglisayarlardan filtrelenmiþ deðerleri saklýcaðýmýz yine tek boyutlu bir pointer dizi(filtre uygulamýcaz bunun üzerinde)
      int *kopyagoruntuMatrisiPointer2;//filtrelenmemiþ diziyi bölüþtürüp masterdan diðerlerine attýðýmýzda diðerlerinin o deðerleri alýp kendi üzerine yazdýðý tek boyutlu pointer dizi
	  int kismiSatirBoyutu,kismiToplamBoyut;//satýrlarýn bölümlenmiþ deðerlerini ve toplam boyutun bölümlenmiþ deðerlerini sakladýðým deðiþkenler
	  int geciciDegisken,geciciDegisken2;//biri master diðeri diðer bilgisayarlar için filtre uygularken kullandýðýmýz iki adet geçici deðiþken 
	  int siralamaMatrisi[25];//yine biri master bilgisayar içn sýralaamayý sakladýðýmýz tek boyutlu normal dizi
	  int siralamaMatrisi2[25];//diðeride diðer bilgisayarlar için sýralamayý sakladýðýmýz tek boyutlu normal dizi
      int indis;//yine programda belirli yerlerde indis atamalarý için tanýmladýðým bi indis deðiþkeni.
      int kismiSatirBoyutu2,kismiToplamBoyut2;//kalan hesabý iþin deðiþkenler
      int kismiSatirKalan;//kalan hesabý için bir diðer deðiþken
      int satirlar;//satirlar degiskeni
	  int sutunlar;//sutunlar degiskeni
	  int myRank;//myRank deðiþkeni
	  int size;//size deðiþkeni
	  MPI_Status status;//status sabiti
	  MPI_Init(&argc,&argv);//programý ilklendirdik
	  MPI_Comm_rank(MPI_COMM_WORLD,&myRank);//rank bilgisini (yani rank bilgisi hangi bilgisauyarda olduðumuz) aldýk
	  MPI_Comm_size(MPI_COMM_WORLD,&size);//size bilgisini aldýk bilgisayarlarýn boyutu yani kaç adet olduðu.
if(myRank==0)//eðer master bilgisayarýn içindeysek.
{
	  	ifstream okunacakDosya;//okunacakdosya adýnda içinde veri alabildiðimiz bir dosya oluþturduk ifstream ile
	  	okunacakDosya.open(argv[1]);//dosyayý açtýk;argv1 ile açtýk çünkü çalýþtýrýrken komut satýrýndan girdiðimiz ilk parametre argv1 olucak  
	  	okunacakDosya>>satirlar;//satirlar deðiþkenine okunacakDosya adýndaki dosyanýn ilk verisini kaydettik
	    okunacakDosya>>sutunlar;//sutunlar deðiþkenine okunacakDosya adýndaki dosyanýn ikinci verisini kaydettik
	    goruntuMatrisiPointer = new int[satirlar*sutunlar];//goruntuMatrisiPointer ý yukarda tanýmlamýþtýk burdada tek boyutlu bir dinamik dizi haaline getirdik.
	    yedekGoruntuMatrisiPointer = new int[satirlar*sutunlar];//yine ayný þekilde yedekGoruntuMatrisiPointer ý yukarda tanýmlamýþtýk burdada tek boyutlu bir dinamik dizi haaline getirdik
	    int atanacakIndisler;//goruntu matrisinin içine atacaðýmýz indisler için bir deðiþken tanýmladýk
	    int matrisinIndisi=0;//matris indisini 0 dan baþlattýk
            for(int s1=0;s1<satirlar;s1++)//satirlarin hepsini dolaþacak dýþtaki döngüyü tanýmladýk
            {
    	      for(int s2=0;s2<sutunlar;s2++)//sütunlarýn hepsini dolaþýcak içteki döngüyü tanýmladýk
    	      {
    	        okunacakDosya>>atanacakIndisler;//atanacak indisler deðiþkenini aldýk bunu satýr ve sütunun yazýlý olduðu satýrdan deðilde bir alt satýrdan baþlamasý
    	        //için yaptýk  bu her okunduðunda bir artarak devam edicek yani matristeki her elemanýn bir saðýndaki indisi alýcak hep
    	        goruntuMatrisiPointer[matrisinIndisi]=atanacakIndisler;//o elemanlarýda matrisin 0 dan satýr ve sütun sayýsý çarpýmýna kadar bütün indislerine atýcak
    	        yedekGoruntuMatrisiPointer[matrisinIndisi]=goruntuMatrisiPointer[matrisinIndisi];//Diziyi bir baaþka dizide yedekliyoruz.
    	        matrisinIndisi++;//matrisin indisini 1 arttýrýyoruz her iterasyonda
	          }
            }
        okunacakDosya.close();//okuma iþlemlerimiz bittikten sonra dosyamýzý kapatýyoruz
        startTime=MPI_Wtime();//Süreyi hesaplamak için MPI_Wtime komutunu çaðýrýyoruz.Süreyi hesaplamaya baþlýcak
        if((satirlar*sutunlar)%size==0)//Eðer matris satýr ve sütun sayýsýna tam bölümüyorsa 
        {
		    kismiSatirBoyutu=(satirlar/size);//satýr boyutunu bölerek bir deðiþkende tutmamýz lazýmki her bilgisayar kendisinin filtre yaptýðý satýr sayýsýný bilmeli.
        	kismiToplamBoyut=(satirlar/size)*sutunlar;//ayný zamanda toplamboyutunda bölünmüþ deðerini bulmam gerekiyo ki master bilgisayardan diðerlerine bu deðeri atmam gerekicek.
        	//çünkü her bilgisayar kendisine ayrýlmýþ kýsmiboyutu bir dizide deðerleriyle birlikte(filtrelenmemiþ matrisin) saklamasý gerekicek ki filtreleme yapabilsin her bilgisayar
        	for(int i=1;i<size;i++)//master bilgisayardan 1 den bütün diðer makinelere(kaç makine varsa artýk) veriyi göndermek için bir for açtýk
        	{
        		MPI_Send(&kismiSatirBoyutu,1,MPI_INT,i,TAG2,MPI_COMM_WORLD);//kýsmiSatirBoyutu bilgisi bütün bilgisayarlara gönderildi
        		MPI_Send(&sutunlar,1,MPI_INT,i,TAG3,MPI_COMM_WORLD);//sütunlarýn bilgisi bütün bilgisayarlara gönderildi
				MPI_Send(&kismiToplamBoyut,1,MPI_INT,i,TAG1,MPI_COMM_WORLD);//kýsmitoplam boyutta bütün bilgisayarlara gönderildi
        	    MPI_Send(&yedekGoruntuMatrisiPointer[i*kismiToplamBoyut],kismiToplamBoyut,MPI_INT,i,TAG4,MPI_COMM_WORLD);//yedeklediðimiz matris de diðer bilgisayarlara gönderildi
			}
			for(int s1=2;s1<kismiSatirBoyutu-1;s1++)//indisi 2 den baþlatýp kýsmiSatýrBoyutunun bir eksiðine gönderiyoruz böylece master bilgisayar ilk iki satýrý ve sok iki satýrý almýcak
			{
				for(int s2=2;s2<sutunlar-1;s2++)//sütunlarýnda 1 eksiðine kadar gidiyoruz son iki sütunu ve ilk iki sütunu almamasý için he bide <= deði < dikkat. dýþtaki fordada ölyle
				{
					 //sýralamaMatrisinin 0 indisinden 24 . indisine yani toplam 25 indise filtreleme uygulacaðýmýz indisleri yani komuþu deðerlerini atýyoruz. Detaylý açýklamasýný
					 //ilk projede yapmýþtým 
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
					 //bubble sort  algoritmasýyla küçükten büyüðe sýralama yapýyoruz 	
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
					yedekGoruntuMatrisiPointer[s1*sutunlar+s2]=siralamaMatrisi[12];//ve ortandca deðeri ilgili indize atýyoruz
				  }
		 	}	
			lokalDiziPointer2 = new int[kismiToplamBoyut];//master bilgisayardan gelen filtrlenmiþ verileri bir dizide tutmak için 2 numaralý lokalpointer adlý dinamik dizi oluþturuyoruz
			for(int i=1;i<size;i++)//makine sayýsýna kadar gidip
			{
				MPI_Recv(lokalDiziPointer2,kismiToplamBoyut,MPI_INT,i,TAG5,MPI_COMM_WORLD,&status);//o filtrelenmiþ deðerleri her makineden tek tek alýyoruz.
				indis=i*((satirlar/size)*sutunlar);//master 0 ýn kendi kaldýðý yerden kopyalýcaðýmýz için indise kalýnan yerlerin belirtildiði bir deðer atýyoruz
				for(int s=0;s<kismiToplamBoyut;s++)//ve  0 dan  kýsmitoplamboyut kadar dolaþýp
				{
					yedekGoruntuMatrisiPointer[indis+s]=lokalDiziPointer2[s];//her bilgisayarýn kaldýðý yerden(master 0 da dahil olmak üzere) indisleri yedekGoruntuMatrisiPointer 
					//adlý deðiþkene atýyoruz ekrana yazdýrcaðýmýz son filtrelenmiþ matriste bu olucak
				}
			}
		  } 
		else if((satirlar*sutunlar)%size!=0)//eðer bilgisayarlara tam bölünmüyorsa
		{
			kismiSatirKalan=satirlar%size;//bölümden kalaný alýp bir deðiþkende tuttuk
			kismiSatirBoyutu=(satirlar/size);//satýr boyutunu bölerek bir deðiþkende tutmamýz lazýmki her bilgisayar kendisinin filtre yaptýðý satýr sayýsýný bilmeli.
			kismiSatirBoyutu2=(satirlar/size)+kismiSatirKalan;//kalan la bölümü topladýk
        	kismiToplamBoyut=kismiSatirBoyutu*sutunlar;//ayný zamanda toplamboyutunda bölünmüþ deðerini bulmam gerekiyo ki master bilgisayardan diðerlerine bu deðeri atmam gerekicek.
        	kismiToplamBoyut2=((satirlar/size)+kismiSatirKalan)*sutunlar;//kalanla bölümün toplamýndan gelen deðeri sutun deðeeriyle çarptýk
        	//çünkü her bilgisayar kendisine ayrýlmýþ kýsmiboyutu bir dizide deðerleriyle birlikte(filtrelenmemiþ matrisin) saklamasý gerekicek ki filtreleme yapabilsin her bilgisayar
        	for(int i=1;i<size;i++)//master bilgisayardan 1 den bütün diðer makinelere(kaç makine varsa artýk) veriyi göndermek için bir for açtýk
        	{
        		//normalde burada kalanýn dahil olduðu kýsmi bilgileri göndermem gerekiyodu ama öyle yapýnca hata allýyordum normal deðerleri gönderdim diðer bilgisayarlara
        		MPI_Send(&kismiSatirBoyutu,1,MPI_INT,i,TAG2,MPI_COMM_WORLD);//kýsmiSatirBoyutu bilgisi bütün bilgisayarlara gönderildi
        		MPI_Send(&sutunlar,1,MPI_INT,i,TAG3,MPI_COMM_WORLD);//sütunlarýn bilgisi bütün bilgisayarlara gönderildi
				MPI_Send(&kismiToplamBoyut,1,MPI_INT,i,TAG1,MPI_COMM_WORLD);//kýsmitoplam boyutta bütün bilgisayarlara gönderildi
        	    MPI_Send(&yedekGoruntuMatrisiPointer[i*kismiToplamBoyut],kismiToplamBoyut,MPI_INT,i,TAG4,MPI_COMM_WORLD);//yedeklediðimiz matris de diðer bilgisayarlara gönderildi
			}
			for(int s1=2;s1<kismiSatirBoyutu-1;s1++)//indisi 2 den baþlatýp kýsmiSatýrBoyutunun bir eksiðine gönderiyoruz böylece master bilgisayar ilk iki satýrý ve sok iki satýrý almýcak
			{
				for(int s2=2;s2<sutunlar-1;s2++)//sütunlarýnda 1 eksiðine kadar gidiyoruz son iki sütunu ve ilk iki sütunu almamasý için he bide <= deði < dikkat. dýþtaki fordada ölyle
				{
					 //sýralamaMatrisinin 0 indisinden 24 . indisine yani toplam 25 indise filtreleme uygulacaðýmýz indisleri yani komuþu deðerlerini atýyoruz. Detaylý açýklamasýný
					 //ilk projede yapmýþtým 
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
					 //bubble sort  algoritmasýyla küçükten büyüðe sýralama yapýyoruz 	
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
					yedekGoruntuMatrisiPointer[s1*sutunlar+s2]=siralamaMatrisi[12];//ve ortandca deðeri ilgili indize atýyoruz
				  }
		 	}	
			lokalDiziPointer2 = new int[kismiToplamBoyut];//master bilgisayardan gelen filtrlenmiþ verileri bir dizide tutmak için 2 numaralý lokalpointer adlý dinamik dizi oluþturuyoruz
			for(int i=1;i<size;i++)//makine sayýsýna kadar gidip
			{
				MPI_Recv(lokalDiziPointer2,kismiToplamBoyut,MPI_INT,i,TAG5,MPI_COMM_WORLD,&status);//o filtrelenmiþ deðerleri her makineden tek tek alýyoruz.
				indis=i*((satirlar/size)*sutunlar);//master 0 ýn kendi kaldýðý yerden kopyalýcaðýmýz için indise kalýnan yerlerin belirtildiði bir deðer atýyoruz
				for(int s=0;s<kismiToplamBoyut;s++)//ve  0 dan  kýsmitoplamboyut kadar dolaþýp
				{
					yedekGoruntuMatrisiPointer[indis+s]=lokalDiziPointer2[s];//her bilgisayarýn kaldýðý yerden(master 0 da dahil olmak üzere) indisleri yedekGoruntuMatrisiPointer 
					//adlý deðiþkene atýyoruz ekrana yazdýrcaðýmýz son filtrelenmiþ matriste bu olucak
				}
			}
		  } 
			

		
        endTime=MPI_Wtime();	//hesaplama süresini bitiriyoruz
		elapsedTime=endTime-startTime;//aradaki farklý alýp bir deðiþkene atýyoruz
		cout<<"Paralel Programlama Suresi: "<<elapsedTime*1000<<endl;//o deðiþkeni de ilgili formatta yazdýrýyoruz.
	    
		
	        char ciktiDosyasi[28];//Filtrelenmiþ matrisin çýktý halini char dizisi þeklinde tanýmladýk örn:GoruntuMatrisi1
    	    string ciktiDosyasiUzantisi= "_filtered.txt";//argümanýn sonuna uzantý eklemek için deðiþken tanýmladýk
    	    for(int s1=0; s1<15; s1++)//0 dan 15 e kadar gidicek for 
	        {
	          ciktiDosyasi[s1]=argv[1][s1+22];// argümanýn toplam (.txt hariç) 38 karakteri var biz sadece GoruntuMatrisi1 örneði þeklinde 15 karakterli bir deðer atadýk	
	        }
	        for(int s1=0; s1<14; s1++)//sonra o 15 deðerli karakterin sonuna 15 ten baþlayarak(15 ten alttaki forda baþlýyor) (devamý altta ki açýklama satýrýnda)
	        {
	          ciktiDosyasi[s1+15] = ciktiDosyasiUzantisi[s1];//uzantýyý atadýk artýk içinde GoruntuMatrisi1_filtered.txt örneði þeklinde bir deðer olmuþ oldu
	        } 								
		
	        ofstream yazilacakDosya;//yazilacak dosyayý çýktý dosyasý olarak ayarladýk
	        yazilacakDosya.open(ciktiDosyasi, ios::out);//burda char dizisini çevirme iþlemi ve yazýlacak dosyayý açma iþlemi yapýyoruz
            //yazilacakDosya<<satirlar;
            //yazilacakDosya<<sutunlar;
	        for(int s1=0; s1<satirlar; s1++)//satýrlara kadar giden bir döngü
            {
               for(int s2=0; s2<sutunlar; s2++)//sütunlara kadar giden bir döngü
	           {
		           yazilacakDosya<<yedekGoruntuMatrisiPointer[s1*sutunlar+s2]<<"\t";//yazýlacak matris deðerleri matrisin ilk satýr ve ilk sütunundan baþlýcak
                  //dikkat et matrisin satýr ve sütun bilgisinin yazdýðý satýrý almýcak onu almak isteseydik bu satýrýn üstündeki 7.ve 8.sütunu
                  //satý ve sütun sayýsý bilgisini yazdýrdýðýmýz kýsmý açýklama satýrýndan kaldýrmamýz gerekirdi
					
	           }
	           yazilacakDosya<<endl;//matris formatýnda yazdýrmak için her satýrdan sonra bir alta iniyoruz.
	        }	

	    yazilacakDosya.close();//yazýlacak dosyayý kapattýk 
	    delete [] goruntuMatrisiPointer;//dinamnik diziyi sildik(filtreleme yaptýðýmýz)
	    delete [] yedekGoruntuMatrisiPointer;//diðer bilgisayarlara veri göndermek için kullandýðýmýz dinamikdiziyide sildik.
	

}	

else//bu elseyi diðer bilgisayarlar için açýyoruz yani veriyi masterdan gönderdik o veriyi alacak bilglsayarlar için açýyorruz
{
         	MPI_Recv(&kismiSatirBoyutu,1,MPI_INT,0,TAG2,MPI_COMM_WORLD,&status);//master bilgisayardan gönderilen kýsmiSatirBoyutu bilgisini aldýk
        	MPI_Recv(&sutunlar,1,MPI_INT,0,TAG3,MPI_COMM_WORLD,&status);//master bilgisayardan gönderilen sütun bilgisini aldýk 
	      	MPI_Recv(&kismiToplamBoyut,1,MPI_INT,0,TAG1,MPI_COMM_WORLD,&status);//ve yine masterdan gelen kýsmiToplamBoyut bilgisini aldýk
           
		   
		    kopyagoruntuMatrisiPointer2=new int[kismiToplamBoyut];//gelen veriyi bir dinamik diziye kopyalamak için dinamik dizi tanýmladýk
	        MPI_Recv(kopyagoruntuMatrisiPointer2,kismiToplamBoyut,MPI_INT,0,TAG4,MPI_COMM_WORLD,&status);//master bilgisayardan gelen diziyi kopyaMatrisiPointer2 ye atýyoruz
	        //bu gelen veri bölümlendirilmiþ veri yani 50 ye 50 bi matrisse ilk 10 satýrý gelicek her bilgisayaraa 10 ar satýr gidicek yani
	        lokalDiziPointer=new int[kismiToplamBoyut];//birtane daha dinamik dizi oluþturuyoruz filtrelediðimiz deðerleri bunda saklýcaz.
	        for(int i=0;i<kismiToplamBoyut;i++)//bu tanýmladýðýmýz diziyi kýsmiToplamBouta kadar dolaþýyoruz
	        {
	        	lokalDiziPointer[i]=kopyagoruntuMatrisiPointer2[i];//içine master 0 dan aldýðýmýz ve içine deðer yazdýðýmýz kopyamatrisinin indislerini atýyoruz tek tek.
			}
				for(int s1=2;s1<kismiSatirBoyutu-1;s1++)//yine ayný þekilde diðer bilgisayarlarýn yapacaðý filte iþlemi için dýþ for tanýmlýyoruz kýsmiBoyutun bir eksiðine kadar gidicek
				//ve 2 den baþlýcak buda ilk iki satýr ve son iki satýrý almamýzý saðlýcak bu nedenle resmin bölütlendirildiði oluþturulan resimde biraaz belli olucazk
				//aradaki çizgilerden anlaþýldýðý gibi.
			    {
				 for(int s2=2;s2<sutunlar-1;s2++)//sütunlarý da yine 2 den baþlayýp bir eksiðine kadar dolaþýyoruz sütunda sýkýntý yok sütunlarýn bölüþtürüldüðü belli olmucak
				 //çünkü satýr odaklý bi bölme iþlemi yapmýþtýk
				 {
				     //filtrelemeyi yapýyoruz ve bu sefer 2 numaralý sýralama matrisine atýyoruz deðerleri ilkiyle karýþmasýn diye.
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
					 //sýraladýðýmýz deðerleri yine bubble sort ile sýralýyoruz .Sýralama yaparkende 2 numaralý gecicideðiþkeni kullandýk.	
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
					lokalDiziPointer[s1*sutunlar+s2]=siralamaMatrisi2[12];//ve ortancasýný bulup bütün indislere her iterasyonda yukarda oluiturduðumuz dinamik dizinin içine atýyoruz.
					//o dizinin içinde filtrelenmemiþ deðerler vardý artýk filtrelenmiþ deðerler olucak. 
				  }
				
		 	    }
			MPI_Send(lokalDiziPointer,kismiToplamBoyut,MPI_INT,0,TAG5,MPI_COMM_WORLD);//filtrelenmiþ deðerleri masterýnda yaptýðý filtrelemeyle birleþtirmek için mastera gönderiyoruz.	
	        delete [] kopyagoruntuMatrisiPointer2;//içine deðer master bilgisayardan aldýðýmýz deðerleri sakladýðýmýz dinamik diziyi siliyoruz
	        delete [] lokalDiziPointer;//ilk baþta filtrelenemiþ deðerleri saklamak için kullandýðýmýz  ve sonra içini filtrelenmiþ deðerle doldurduðumuz dinamik dizimizi siliyoruz

}       
        
    	MPI_Finalize();//mpý iþlemlerini ilklendirdiðimiz gibi sonlandýrýyoruz
		return 0;//programý bitiriyoruz
}
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
	

	
