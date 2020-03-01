#include <math.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
int dataMagnitude[30];
double feature[7];
double feat_hid[10];
double output[4];
int classi;
const double w_hid[8][10] =
{
{-0.690056026100996,-1.1896308871617,-0.668169641627117,0.102510794333944,-1.30188867792374,-0.532102369558741,0.416240060434533,-0.304203944127927,0.193033323914609,-0.977922828570904},
{1.3472063728642,-0.386319740712148,-0.579496895882868,-0.804806997050516,-0.347721931710367,1.00378756046066,-0.267079245662994,-2.06173386659153,-1.22242146575749,-0.495638579930519},
{0.301295595856141,-0.112928622172582,-0.519074753280485,-0.0336675232917885,-0.946060435370446,-0.728717301109892,-0.367649935461035,1.31934016742614,0.426741307985384,-1.73924923045549},
{-0.494142430689561,0.138363931758513,0.0392637815584507,-0.338541149330589,1.16251708388129,-0.966932580533985,0.604008419345457,1.2410031749621,0.381413293482579,-1.48180717299885},
{1.21495607357982,-0.340907470583549,0.241187424277676,0.634534000842899,0.18830413025243,-0.174756881478196,0.645484997439865,-0.426002777708213,-0.0556715758288754,0.56980786487716},
{-0.774839304137415,-1.01264150236782,0.546061589283759,0.678199479430039,-0.01390599735751,-0.553663036230691,0.819976396784629,-0.0372195832827279,-1.22490704033163,2.48278418723536},
{0.960802607158311,0.982588823162141,-0.0493238837058579,0.372940714476021,-0.815781497945336,0.28608717386393,-0.00770628436959288,-0.135421441888791,0.768031026465186,-1.26167674560156},
{-0.772134091488537,-1.17524359886726,1.12074129701142,-0.466633414543076,-0.0195030952696478,0.00497346379530941,0.25145124406623,-0.00710647810474042,-0.295958305485319,0.396306863256281}
}, w_out[11][4] =
{
{-0.64504248285309,0.255239477595484,0.175543336600625,-0.0157304370428409},
{0.546879980364963,-0.257605703250662,-0.432342701538316,0.241158218248336},
{1.26796927550562,0.0252696259382295,-0.429434937993213,-0.809399036688476},
{-0.22427606142263,-0.362909708560685,0.888552932368041,-0.636722438950712},
{0.616075839683866,-1.63050541502816,0.950819585713974,-0.377349396213502},
{-1.68376642936902,0.907806778051288,0.330406753829062,0.874616102622018},
{0.441148468346557,0.659864507703713,-0.172291306754474,-0.425906027767321},
{-0.00394389863994468,1.99758118386645,-0.278111483051283,-1.45325798297063},
{0.548711742946787,0.741514182644565,-0.308764644972957,-1.02010359457099},
{-0.529636713161142,0.940245577489643,-0.558365948685524,0.241361002204607},
{-0.232716596853238,0.646856383003793,0.357277487522457,-0.035024673618233}
}, minmax[7][2] =
{
{},
{},
{},
{},
{},
{},
{}
};
double S, Sig, Sigma, Sigma1, Sigma2, MAV, RMS, VAR, SDT, WL, ZC, SSC, Xn, accum, accumi;
char analogpin = 0, N=20;


void setup()
{

	/* add setup code here */
	Serial.begin(9600);
	mySerial.begin(9600);
        
}

void loop()
{

//Serial.println((analogRead(analogpin))-511);  
  while(abs(analogRead(analogpin)-511)>30)
  {
  //Serial.println((analogRead(analogpin))-511);  
    
    //data acqusition
     
    for( int i = 0; i<30;i++)
    {
       //Xn = (analogRead(analogpin))-511;
      dataMagnitude[i]=((analogRead(analogpin))-511);
      delay(12.5);
    // Serial.println(dataMagnitude[i]);
    }
    
	S = 0;
        for (size_t i = 0; i < N; i++)
	{
		S = S + dataMagnitude[i];
	}
	Sig = 0;
	Sigma = 0;
	Sigma1 = 0;
	Sigma2 = 0;
	for (size_t i = 0; i < N; i++)
	{
		Sig = Sig + abs(dataMagnitude[i]);
		Sigma = Sigma + pow(dataMagnitude[i], 2);
		Sigma1 = Sigma1 + pow((dataMagnitude[i] - S / N), 2);
	}

	for (size_t i = 0; i < N - 1; i++)
	{
		Sigma2 = Sigma2 + abs(dataMagnitude[i + 1] - dataMagnitude[i]);
	}
        
        
        //featureextract
	MAV = (((1 / (double)N)*Sig-minmax[0][0])*2/(minmax[0][1]-minmax[0][0]))-1;


	RMS = (sqrt((1 / (double)N)*Sigma)-minmax[1][0])*2/(minmax[1][1]-minmax[1][0])-1;

	VAR = (((1 / (double)(N))*Sigma1)-minmax[2][0])*2/(minmax[2][1]-minmax[2][0])-1;

	SDT = (sqrt((1 / (double)(N)) * Sigma1)-minmax[3][0])*2/(minmax[3][1]-minmax[3][0])-1;

	
	WL = (Sigma2-minmax[4][0])*2/(minmax[4][1]-minmax[4][0])-1;

	ZC = 0;
	for (size_t i = 0; i < N - 1; i++)
	{
		if (dataMagnitude[i] == 0 && dataMagnitude[i] != dataMagnitude[i + 1])
		{
			ZC++;
		}
 
                else if ((dataMagnitude[i] < 0 && dataMagnitude[i+1] >0) || (dataMagnitude[i] > 0 && dataMagnitude[i+1] < 0))
                {
                    ZC++;
                }
	}
        ZC = (ZC-minmax[5][0])*2/(minmax[5][1]-minmax[5][0])-1;


	SSC = 0;
	for (size_t i = 0; i < N - 2; i++)
	{
		if (dataMagnitude[i]>dataMagnitude[i + 1] && dataMagnitude[i + 1]<dataMagnitude[i + 2])
		{
			SSC++;
		}
		else if (dataMagnitude[i]<dataMagnitude[i + 1] && dataMagnitude[i + 1]>dataMagnitude[i + 2])
		{
			SSC++;
		}
	}
       SSC = (SSC-minmax[6][0])*2/(minmax[6][1]-minmax[6][0])-1;

    
    double feature[] = { MAV, RMS, VAR, SDT, WL, ZC, SSC };
      
      //neural network construction
		//first-hidden layer
	for (int i = 0; i < 10; i++)
		{
            
      accum = w_hid[7][i];
                  
			for (int j = 0; j < 7; j++)
			{
				accum += (feature[j] * w_hid[j][i]);
			}
			feat_hid[i] = tanh(accum);
                }

		for (int i = 0; i < 4; i++)
		{
       
                   accumi = w_out[10][i];
			for (int j = 0; j < 10; j++)
			{
				accumi += (feat_hid[j] * w_out[j][i]);
			}
			output[i] = (accumi);
		}

		//Classify the output
		if (output[0] > output[1] && output[0] > output[2] && output[0] > output[3])
		{
			classi = 1;
		}
		else if (output[1] > output[0] && output[1] > output[2] && output[1] > output[3])
		{
			classi = 2;
		}
		else if (output[2] > output[1] && output[2] > output[0] && output[2] > output[3])
		{
			classi = 3;
		}
		else if (output[3] > output[1] && output[3] > output[2] && output[3] > output[0])
		{
			classi = 4;
		}
		else
		{
			classi = 0;
		}

mySerial.write(classi);		

//Serial.println(output[0],5);
//Serial.println(output[1],5);
//Serial.println(output[2],5);
//Serial.println(output[3],5);

Serial.println(millis());
//Serial.println(classi);
//Serial.println(feature[0]);
//Serial.println(feature[1]);
//Serial.println(feature[2]);
//Serial.println(feature[3]);
//Serial.println(feature[4]);
//Serial.println(feature[5]);
//Serial.println(feature[6]);


		for (size_t i = 0; i < 10; i++)
		{
			feat_hid[i] = 0;
		}
		for (size_t i = 0; i < 4; i++)
		{
			output[i] = 0;
		}
  };
  
  
  
  
}




