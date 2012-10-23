#include <fstream.h>
#include <iostream.h>
#include <math.h>
#include <stdio.h>
//#include <conio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

/*
#define BP_LEARNING (float)(0.5) // The learning coefficient.
#define x (int)(45)//inputs+neutral
#define n (int)(5)//neutral
#define cases (int)(490)//cases
#define y (int)(4)//layers
#define z (int)(3)//outputs
*/

class CBPNet
{
public:
    CBPNet();
    ~CBPNet()
    {}
    ;
    int dataini(void);
    int datarunini(void);
    float Train(int, int, int);
    float Run(int, int, int, float&);
    float Runh(int, int, int, float&);
    int Wofstream(void);
    int Wifstream(void);
    int wini(void);

    float BP_LEARNING;
    int x;
    int n;
    int cases;
    int casesrun;
    int y;
    int z;
    //private:
    // float m_W[x][y][x][y][z];
    float m_W[312500];//max x=50,y=5,z=5
    float Sigmoid(float);
};

CBPNet::CBPNet()
{}

int CBPNet::dataini()
{
    ifstream a_file("dataini.txt");
    float a[x-n+z][cases];
    for (int cc = 0; cc < cases; cc++)
    {
        for (int i = 0; i < x-n+z; i++)

            a_file >> a[i][cc];
    }
    a_file.close();
    float maxv[x-n+z], minv[x-n+z];
    for (int i = 0; i < x-n+z; i++)
    {
        maxv[i]=a[i][0];
        minv[i]=a[i][0];
    }

    for (int i = 0; i < x-n+z; i++)
    {
        for (int cc = 0; cc < cases; cc++)
        {

            if(a[i][cc]>maxv[i])
                maxv[i]=a[i][cc];
            if(a[i][cc]<minv[i])
                minv[i]=a[i][cc];
        }
    }


    ofstream onffile("dataini_inf.txt");
    for (int i = 0; i < x-n+z; i++)
        onffile << minv[i]  << " "<< maxv[i]<< " ";
    onffile.close();
    ofstream ofile("data.txt");

    for (int cc = 0; cc < cases; cc++)
    {
        for (int i = 0; i < x-n+z; i++)
        {
            a[i][cc]=(a[i][cc]-minv[i])/maxv[i]*0.9;
            ofile << a[i][cc] << " ";
        }
    }
    ofile.close();

    return 0;
}

int CBPNet::datarunini()
{
    ifstream a_file("datarunini.txt");
    float a[x-n+z][casesrun];
    for (int cc = 0; cc < casesrun; cc++)
    {
        for (int i = 0; i < x-n+z; i++)

            a_file >> a[i][cc];
    }
    a_file.close();
    float maxv[x-n+z], minv[x-n+z];
    for (int i = 0; i < x-n+z; i++)
    {
        maxv[i]=a[i][0];
        minv[i]=a[i][0];
    }

    for (int i = 0; i < x-n+z; i++)
    {
        for (int cc = 0; cc < casesrun; cc++)
        {

            if(a[i][cc]>maxv[i])
                maxv[i]=a[i][cc];
            if(a[i][cc]<minv[i])
                minv[i]=a[i][cc];
        }
    }

    ofstream onffile("datarunini_inf.txt");
    for (int i = 0; i < x-n+z; i++)
        onffile << minv[i] << " "<< maxv[i]<< " ";
    onffile.close();
    ofstream ofile("datarun.txt");
    for (int cc = 0; cc < casesrun; cc++)
    {
        for (int ii = 0; ii < x-n+z; ii++)
        {
            a[ii][cc]=(a[ii][cc]-minv[ii])/maxv[ii]*0.9;
            //    cout<<a[ii][cc]<<"   ";
            ofile << a[ii][cc]<< " ";
        }
    }
    ofile.close();

    return 0;
}

int CBPNet::wini()
{
    srand((unsigned)(time(NULL)));
    int count = 0;

    for (int i = 0; i < x; i++)
    {
        for (int j = 1; j < y; j++)
        {
            for (int k = 0; k < x; k++)
            {
                for (int zz = 0; zz < z; zz++)
                {
                    //        m_W[i][j][k][j - 1][zz] = (float)(rand()) / (32767 / 2) - 1;
                    *(m_W+j+y*i+x*y*k+x*y*x*(j-1)+x*y*x*y*zz) = (float)(rand()) / (10050 / 2) / 10e5 - 1;
                    count++;
                }
            }
        }
    }
    cout << count << "\n";
    return 0;
}


float CBPNet::Train(int dx, int dy, int dz)
{
    float out[dz];
    float a[x][y];
    float d[dz];
    for (int k = 0; k < x; k++)
    {
        for (int j = 0; j < y; j++)
        {
            a[k][j] = 0;
            if (j == 0)
                a[k][j] = 1;
        }
    }
    ifstream a_file("data.txt");

    for (int ex=0; ex<dy;ex++)
    {

        for (int i = 0; i < dx - dz; i++)
            a_file >> a[i][0];
        for (int zz = 0; zz < dz; zz++)
        {
            a_file >> d[zz];


            for (int j = 1; j < y - 1; j++)
            {
                for (int k = 0; k < x; k++)
                {
                    for (int i = 0; i < x; i++)
                    {

                        //            a[k][j] += m_W[k][j][i][j - 1][zz] *a[i][j - 1];
                        a[k][j] += *(m_W+j+y*k+x*y*i+x*y*x*(j-1)+x*y*x*y*zz)*a[i][j - 1];
                    }
                    a[x - 1][j] = 1;
                }
                for (int k = 0; k < x - 1; k++)
                {
                    a[k][j] = Sigmoid(a[k][j]);
                }
            }
            int j = y - 1;
            int k = 0;
            for (int i = 0; i < x; i++)
                //        a[k][j] += m_W[k][j][i][j - 1][zz] *a[i][j - 1];
                a[k][j] += *(m_W+j+y*k+x*y*i+x*y*x*(j-1)+x*y*x*y*zz) *a[i][j - 1];
            a[k][j] = Sigmoid(a[k][j]);
            out[zz] = a[k][j];
            float delta[x][y];
            for (int i = 0; i < x; i++)
            {
                for (int j = 0; j < y; j++)
                {

                    delta[i][j] = 0;

                }
            }

            delta[zz][y - 1] = (d[zz] - out[zz]) *out[zz]*(1-out[zz]);

            for (int i = 0; i < x; i++)
                //          m_W[0][y - 1][i][y - 2][zz] += BP_LEARNING * a[i][y - 2] *delta[zz][y - 1];
                *(m_W+y-1+y*0+x*y*i+x*y*x*(y-2)+x*y*x*y*zz) += BP_LEARNING * a[i][y - 2] *delta[zz][y - 1];
            j = y - 2;
            for (int i = 0; i < x; i++)
                delta[i][j] = a[i][j]*(1-a[i][j]) *delta[zz][y - 1];


            for (int k = 0; k < x; k++)
            {
                for (int i = 0; i < x; i++)
                {
                    //         m_W[k][j][i][j - 1][zz] += BP_LEARNING * a[i][j - 1] *delta[k][j];
                    *(m_W+j+y*k+x*y*i+x*y*x*(j-1)+x*y*x*y*zz) += BP_LEARNING * a[i][j - 1] *delta[k][j];
                }
            }
            if (y > 3)
            {
                for (int j = y - 3; j > 0; j--)
                {
                    for (int i = 0; i < x; i++)
                    {
                        for (int k = 0; k < x; k++)
                        {
                            //             delta[i][j] += m_W[k][j + 1][i][j][zz] *delta[k][j + 1];
                            delta[i][j] += *(m_W+j+1+y*k+x*y*i+x*y*x*(j)+x*y*x*y*zz) *delta[k][j + 1];
                        }
                    }
                    for (int i = 0; i < x; i++)
                    {
                        delta[i][j] *= a[i][j]*(1-a[i][j]);
                    }
                    for (int k = 0; k < x; k++)
                    {
                        for (int i = 0; i < x; i++)
                        {
                            //              m_W[k][j][i][j - 1][zz] += BP_LEARNING * a[i][j - 1] *delta[k][j];
                            *(m_W+j+y*k+x*y*i+x*y*x*(j-1)+x*y*x*y*zz) += BP_LEARNING * a[i][j - 1] *delta[k][j];
                        } //for i
                    } //for k
                } //for j
            } //if y>3
        } //zz
    } //ex
    a_file.close();
    return 0;
}

int CBPNet::Wofstream()
{
    ofstream W_file("W.txt");
    for (int i = 0; i < x; i++)
    {
        for (int j = 1; j < y; j++)
        {
            for (int k = 0; k < x; k++)
            {
                for (int zz = 0; zz < z; zz++)
                {
                    //W_file << m_W[i][j][k][j - 1][zz] << " ";
                    W_file << *(m_W+j+y*i+x*y*k+x*y*x*(j-1)+x*y*x*y*zz) << " ";
                }
            }
        }
    }
    W_file.close();
    return 0;
}

int CBPNet::Wifstream()
{
    ifstream W1_file("W.txt");
    for (int i = 0; i < x; i++)
    {
        for (int j = 1; j < y; j++)
        {
            for (int k = 0; k < x; k++)
            {
                for (int zz = 0; zz < z; zz++)
                {
                    W1_file >> *(m_W+j+y*i+x*y*k+x*y*x*(j-1)+x*y*x*y*zz);
                }
            }
        }
    }
    W1_file.close();
    return 0;
}

float CBPNet::Sigmoid(float num)
{
    return 1 / (1+exp( - num));
}

float CBPNet::Run(int dx, int dy, int dz, float& R)
{
    float maxv[dx], minv[dx];
    ifstream inffile("datarunini_inf.txt");
    for (int i = 0; i < dx; i++)
        inffile >> minv[i] >> maxv[i];
    inffile.close();

    R=0;
    float out[dz];
    float a[x][y];
    float d[dz];
    for (int k = 0; k < x; k++)
    {
        for (int j = 0; j < y; j++)
        {
            a[k][j] = 0;
            if (j == 0)
                a[k][j] = 1;
        }
    }
    ifstream a_file("datarun.txt");
    ofstream outfile("apotelesmata.txt");
    for (int ex=0; ex<dy;ex++)
    {

        for (int i = 0; i < dx - dz; i++)
            a_file >> a[i][0];
        for (int zz = 0; zz < dz; zz++)
        {
            a_file >> d[zz];


            for (int j = 1; j < y - 1; j++)
            {
                for (int k = 0; k < x; k++)
                {
                    for (int i = 0; i < x; i++)
                    {

                        a[k][j] += *(m_W+j+y*k+x*y*i+x*y*x*(j-1)+x*y*x*y*zz) *a[i][j - 1];
                    }
                    a[x - 1][j] = 1;
                }
                for (int k = 0; k < x - 1; k++)
                {
                    a[k][j] = Sigmoid(a[k][j]);
                }
            }
            int j = y - 1;
            int k = 0;
            for (int i = 0; i < x; i++)
                a[k][j] += *(m_W+j+y*k+x*y*i+x*y*x*(j-1)+x*y*x*y*zz) *a[i][j - 1];
            a[k][j] = Sigmoid(a[k][j]);
            out[zz] = a[k][j];
        }
        for (int i = 0; i < dx - dz; i++)
        {
            a[i][0]=a[i][0]*maxv[i]/0.9+minv[i];
            cout << a[i][0];
            outfile << a[i][0];
            if (i != dx - dz - 1)
                cout << ",";
            outfile <<" ";
        }
        cout << " = ";
        outfile << " = ";
        for (int k = 0; k < dz; k++)
        {
            out[k]=out[k]*maxv[dx-dz+k]/0.9+minv[dx-dz+k];
            cout << out[k];
            outfile << out[k];
            if (k != dz - 1)
                cout << ",";
            outfile <<" ";

        }
        cout << " ....delta= ";
        outfile <<" delta: ";
        for (int k = 0; k < dz; k++)
        {
            d[k]=d[k]*maxv[dx-dz+k]/0.9+minv[dx-dz+k];
            cout << out[k] - d[k];
            outfile << out[k] - d[k];
            R+=(out[k] - d[k])*(out[k] - d[k]);

            if (k != dz - 1)
                cout << ",";
            outfile <<" ";
        }
        cout << endl;
        outfile << endl;
    }
    R=R/casesrun;
    R=sqrt(R);
    cout << "Sqrt(Sdelta^2/casesrun) = " << R << endl;
    outfile.close();
    a_file.close();
    return 0;
}

float CBPNet::Runh(int dx, int dy, int dz, float& R)
{
    float maxv[dx], minv[dx];
    ifstream inffile("datarunini_inf.txt");
    for (int i = 0; i < dx; i++)
        inffile >> minv[i] >> maxv[i];
    inffile.close();

    R=0;
    float out[dz];
    float a[x][y];
    float d[dz];
    for (int k = 0; k < x; k++)
    {
        for (int j = 0; j < y; j++)
        {
            a[k][j] = 0;
            if (j == 0)
                a[k][j] = 1;
        }
    }
    ifstream a_file("datarun.txt");
    ofstream outfile("apotelesmata.txt");

    for (int i = 0; i < dx - dz; i++)
        a_file >> a[i][0];

    for (int zz = 0; zz < dz; zz++)
        a_file >> d[zz];

    for (int ex=0; ex<dy;ex++)
    {


        for (int zz = 0; zz < dz; zz++)
        {

            for (int j = 1; j < y - 1; j++)
            {
                for (int k = 0; k < x; k++)
                {
                    for (int i = 0; i < x; i++)
                    {

                        a[k][j] += *(m_W+j+y*k+x*y*i+x*y*x*(j-1)+x*y*x*y*zz) *a[i][j - 1];
                    }
                    a[x - 1][j] = 1;
                }
                for (int k = 0; k < x - 1; k++)
                {
                    a[k][j] = Sigmoid(a[k][j]);
                }
            }
            int j = y - 1;
            int k = 0;
            for (int i = 0; i < x; i++)
                a[k][j] += *(m_W+j+y*k+x*y*i+x*y*x*(j-1)+x*y*x*y*zz) *a[i][j - 1];
            a[k][j] = Sigmoid(a[k][j]);
            out[zz] = a[k][j];
        }
        for (int i = 0; i < dx - dz; i++)
        {
            cout << a[i][0]*maxv[i]/0.9+minv[i];
            outfile << a[i][0]*maxv[i]/0.9+minv[i];
            if (i != dx - dz - 1)
                cout << ",";
            outfile <<" ";
        }
        cout << " = ";
        outfile << " = ";
        for (int k = 0; k < dz; k++)
        {
            cout << out[k]*maxv[dx-dz+k]/0.9+minv[dx-dz+k];
            outfile << out[k]*maxv[dx-dz+k]/0.9+minv[dx-dz+k];

            for (int i = 1; i < dx - dz; i++)
                a[i-1][0]=a[i][0];
            a[dx-dz-1][0]=out[k];

            if (k != dz - 1)
                cout << ",";
            outfile <<" ";

        }
        /*       cout << " ....delta= ";
               outfile <<" delta: ";
               for (int k = 0; k < dz; k++)
               {
                   cout << out[k] - d[k];
                   outfile << out[k] - d[k];
                   R+=(out[k] - d[k])*(out[k] - d[k]);

                   if (k != dz - 1)
                       cout << ",";
                   outfile <<" ";
               }*/
        cout << endl;
        outfile << endl;
    }
    R=R/casesrun;
    R=sqrt(R);
    cout << "Sqrt(Sdelta^2/casesrun) = " << R << endl;
    outfile.close();
    a_file.close();
    return 0;
}
