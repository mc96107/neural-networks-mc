#include <iostream>
#include "bpnet.h"

int main()
{
    float R, Rtmp, Rlim, Rcontlim;
    int Rcont;
    int i100;
    int profecies=0;
    int BPM_ITER;
    using namespace std;
    int sw;
    int chpoint=0;
    CBPNet bp;
    bp.BP_LEARNING=0.5;
    bp.x=3;
    bp.n=1;
    bp.cases=4;
    bp.casesrun=4;
    bp.y=4;
    bp.z=1;
    cout<<"input.txt: inputs+neutral, neutral, cases, casesrun, layers, outputs, BP_LEARNING"<<endl;
    //    system("PAUSE");
    ifstream infile("input.txt");
    infile>> bp.x >>bp.n>> bp.cases>>bp.casesrun>>bp.y>>bp.z>>bp.BP_LEARNING;
    infile.close();
    bp.dataini();
    bp.datarunini();
    bp.wini();
    ofstream Rfile("sqrtR2_C.txt");
    do
    {
        cout << "0: Train\n1: Run\n2: Both\n3: Load\n4: Save\n5: Analysis\n6: Run future\n7: Rlim\n8: Restart\n9: End\n10: Trace best starting vector" << endl;
        cin >> sw;
        switch (sw)
        {
            case 0:
            cout << "Numper of Itarations: ";
            cin >> BPM_ITER;
            for (float i = 0; i < BPM_ITER; i++)
            {
                bp.Train(bp.x-bp.n+bp.z,bp.cases,bp.z);
                cout << i/BPM_ITER*100 << "%" << endl;
            }
            break;
            case 1:
            bp.Run(bp.x-bp.n+bp.z,bp.casesrun,bp.z, R);
            break;
            case 2:
            cout << "Numper of Itarations: ";
            cin >> BPM_ITER;
            for (float i = 0; i < BPM_ITER; i++)
            {
                bp.Train(bp.x-bp.n+bp.z,bp.cases,bp.z);
                cout << i/BPM_ITER*100 << "%" << endl;
            }

            bp.Run(bp.x-bp.n+bp.z,bp.casesrun,bp.z, R);
            break;
            case 3:
            bp.Wifstream();
            break;
            case 4:
            bp.Wofstream();
            break;
            case 5:

            R=0;
            cout << "Numper of Itarations: ";
            cin >> BPM_ITER;
            cout << "Numper of checkpoints: ";
            cin >> chpoint;
            cout << "Limit for ((Rtmp-R)/R)^2(0 to run without R monitoring: ";
            cin >> Rcontlim;
            Rtmp=10000;
            for (int j=0;j<chpoint;j++)
            {
                for (float i = 0; i < BPM_ITER/chpoint; i++)
                {
                    bp.Train(bp.x-bp.n+bp.z,bp.cases,bp.z);
                    cout << (i+j*BPM_ITER/chpoint)/BPM_ITER*100 << "%" << endl;
                }
                bp.Run(bp.x-bp.n+bp.z,bp.casesrun,bp.z, R);
                Rfile << BPM_ITER/chpoint*(j+1) << " " << R << endl;
                if (Rcontlim!=0)
                {
                    if(((Rtmp-R)/R*(Rtmp-R)/R)<Rcontlim)
                    {
                        cout << "overfitting danger, continue? (Yes (1), No (0): ";
                        cin >> Rcont;
                        if (Rcont==0)
                            break;
                    }
                    if(R>Rtmp)
                    {
                        cout << "R increased, continue? (Yes (1), No (0): ";
                        cin >> Rcont;
                        if (Rcont==0)
                            break;
                    }
                    Rtmp=R;
                }
            }

            break;
            case 6:
            // read one input row and historicaly predict the series
            cout<< "profecies :";
            cin >> profecies;
            bp.Runh(bp.x-bp.n+bp.z,profecies,bp.z, R);
            break;

            case 7:
            R=0;
            Rlim=0;
            i100=0;
            cout << "R upper limit: ";
            cin >> Rlim;
            do
            {
                i100++;
                for(int iii=0;iii<100;iii++)
                    bp.Train(bp.x-bp.n+bp.z,bp.cases,bp.z);
                bp.Run(bp.x-bp.n+bp.z,bp.casesrun,bp.z, R);
            }
            while(R>Rlim);
            bp.Run(bp.x-bp.n+bp.z,bp.casesrun,bp.z, R);
            Rfile << i100*100 << " " << R << endl;
            cout << "Numper of Itarations: " << i100*100 <<endl;

            break;

            case 8:

            {
                ifstream infile("input.txt");
                infile>> bp.x >>bp.n>> bp.cases>>bp.casesrun>>bp.y>>bp.z>>bp.BP_LEARNING;
                infile.close();
                bp.wini();
            }
            break;


            case 10:
            {
                float Rtmp=1000;
                int xn=0;
                float countt=0;
                int Wcases,ncases,ycases;
                ifstream infile("input.txt");
                infile>> bp.x >>bp.n>> bp.cases>>bp.casesrun>>bp.y>>bp.z>>bp.BP_LEARNING;
                infile.close();
                BPM_ITER=20;
                xn=bp.x-bp.n;
                cout<< "Wcases: ";
                cin>> Wcases;
                cout<< "ncases: ";
                cin>> ncases;
                cout<< "ycases: ";
                cin>> ycases;
                cout<< "BPM_ITER: ";
                cin>> BPM_ITER;
                float counttotal=Wcases*ncases*ycases;
                for (int iii=0;iii<Wcases;iii++)
                {
                    for (int j=0;j<ncases;j++)
                    {
                        for (int k=0;k<ycases;k++)
                        {
                            bp.n=1+j;
                            bp.x=xn+bp.n;
                            bp.y=3+k;
                            bp.wini();
                            for (float i = 0; i < BPM_ITER; i++)
                            {
                                bp.Train(bp.x-bp.n+bp.z,bp.cases,bp.z);
                                //    cout << i/BPM_ITER*100 << "%" << endl;
                            }
                            bp.Run(bp.x-bp.n+bp.z,bp.casesrun,bp.z, R);
                            if(Rtmp>R)
                            {
                                Rtmp=R;
                                bp.Wofstream();
                                ofstream infileo("input.txt");
                                infileo<< bp.x <<endl<<bp.n<<endl<< bp.cases<<endl<<bp.casesrun<<endl<<bp.y<<endl<<bp.z<<endl<<bp.BP_LEARNING<<endl;
                                infileo.close();
                            }
                            countt++;
                            cout<<countt/counttotal*100<<"%"<<endl;
                        }
                    }
                }
                ifstream infile1("input.txt");
                infile1>> bp.x >>bp.n>> bp.cases>>bp.casesrun>>bp.y>>bp.z>>bp.BP_LEARNING;
                infile1.close();
                bp.wini();
                bp.Wifstream();
                bp.Run(bp.x-bp.n+bp.z,bp.casesrun,bp.z, R);
                cout<< bp.x <<endl<<bp.n<<endl<< bp.cases<<endl<<bp.casesrun<<endl<<bp.y<<endl<<bp.z<<endl<<bp.BP_LEARNING<<endl;
            }
            break;

            default:
            break;



        }
    }
    while (sw!=9);

    //  system("PAUSE");
    Rfile.close();
    return 0;
}
