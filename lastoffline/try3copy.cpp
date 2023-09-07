#include<bits/stdc++.h>
#include <windows.h>

#define BLACK 0
#define GREEN 10
#define RED 12
#define CYAN 11
#define WHITE 15

using namespace std;

string datastring,generatorpolynimial,cm;
int m,rows,columns;
double p;
vector<string>datablock;
vector<string>datablock2;
vector<double>randomprobabilities;


string BinaryConversion(int num)
{
    string str="";

    while(num!=0)
    {
        str.push_back((num % 2) + '0');
        num=num/2;
    }

    while(str.size() < 8)
    {
        str.push_back('0');
    }

    reverse(str.begin(),str.end());

    return str;

}

void AddCheckBits(int r)
{
    int datalen,counter;
    string str,tempstr;
    int temp,temp2;

    for(int i=0; i<rows; i++)
    {
        counter=0;
        datalen=datablock[i].length();

        //taking temporary string
        str.resize(r+datalen);
        for(int j=0; j< datalen+r ; j++)
        {
            str[j]=' ';
        }

        //determining parity positions with *
        temp=1;
        for(int j=1; j<=r; j++)
        {
            str[temp-1]='*';
            temp=temp*2;
        }

        //fill up without the parity positions
        temp=0;
        for(int j=0; j<datalen; j++)
        {
            while(str[temp]!= ' ')
            {
                temp++;
            }

            str[temp]=datablock[i][j];
        }

        //filling with parity
        datalen+=r;
        temp2=1;
        int k;
        for(int j=1; j<=r; j++)
        {
            counter=0;

            for(k=j+1; k<=datalen; k++)
            {
                tempstr=BinaryConversion(k);
                reverse(tempstr.begin(),tempstr.end());

                if(tempstr[j-1]=='1' && str[k-1]=='1')
                {
                    counter++;
                }

            }

            if(counter%2 ==0)
            {
                str[temp2-1]='0';
            }
            else
            {
                str[temp2-1]='1';
            }

            temp2=temp2*2;
        }

        datablock[i]=str;

    }

}

string Modulo2Div(string dividend, string divisor)
{
    if(dividend.length() < divisor.length())
        return dividend;

    string remainder,tempstr;
    int temp;

    int slength=divisor.length();

    for(int i=0; i<slength; i++)
    {
        remainder.push_back(dividend[i]);
    }

    temp=slength;

    while(divisor.length()==remainder.length())
    {

        //division
        for(int i=0; i<slength; i++)
        {
            remainder[i] = ((remainder[i]-'0')^(divisor[i]-'0')) + '0';
        }

        //remove the leading zeros
        tempstr= "";
        for(int i=0; i<slength; i++)
        {
            if(remainder[i]=='1')
            {
                for(int j=i; j<slength; j++)
                {
                    tempstr.push_back(remainder[j]);
                }
                break;
            }
        }

        remainder=tempstr;

        //pull down the next bits
        while(temp < dividend.length() && remainder.length() < divisor.length())
        {
            remainder.push_back(dividend[temp]);
            temp++;
        }


    }


    if(remainder.length()==0)
    {
        remainder="0";
    }

    return remainder;

}

void CRCcalculation()
{
    string tempstr=cm;
    string remainder;

    int l=generatorpolynimial.length();

    for(int i=0; i<l-1; i++)
    {
        tempstr.push_back('0');
    }

    remainder=Modulo2Div(tempstr,generatorpolynimial);

    //add extra 0's
    reverse(remainder.begin(),remainder.end());

    while(remainder.length()< l-1)
    {
        remainder.push_back('0');
    }
    reverse(remainder.begin(),remainder.end());

    //printing
    cout<< "data dits after appending CRC checksum <sent frame>: \n";
    cout << cm;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CYAN);
    cout << remainder;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);

    cm =cm+remainder;
}




int main()
{
    //freopen("input.txt","r",stdin);

    int temp1,temp2,datastringlength,templen;
    string tempstr;

    cout<< "enter data string: ";
    getline(cin,datastring);

    cout<< "enter number of data bytes in a row <m>: ";
    cin>> m;

    cout<< "enter probability <p>: ";
    cin>> p;

    cout<< "enter generator polynomial: ";
    cin>> generatorpolynimial;

    cout<< "\n\n\n";


    ///task 1 : update the data string if needed
    while((datastring.size()% m) != 0)
    {
        datastring.push_back('~');
    }
    cout<< "data string after padding: " << datastring << endl;
    cout<< "\n" ;


    ///task 2: create data blocks

    datastringlength=datastring.length();
    rows=datastringlength/m;
    columns=m;
    int index=0;

    //initialize the strings in vector
    datablock.resize(rows);
    datablock2.resize(rows);

    for(int i=0; i<rows; i++)
    {
        datablock[i]="";
        datablock2[i]="";
    }

    for(int i=0; i<datastringlength; i++)
    {
        datablock[index]+=(BinaryConversion(datastring[i]));

        if((i+1)%columns ==0)
        {
            index++;
        }
    }

    cout<< "data block <ascii code of m characters per row>:\n";

    for(int i=0; i<rows; i++)
    {
        cout<< datablock[i] << endl;
    }


    ///task 3 :add check bits

    //first calculate the number of redundant bits r

    temp1=1;
    temp2=8*m+1;
    int r=0;
    while(true)
    {
        if(temp2<=temp1)
            break;
        temp1=temp1*2;
        r++;
    }

    AddCheckBits(r);
    cout<< "\n\n";

    //print after adding check bits
    cout<< "data block after adding check bits:\n";
    templen=datablock[0].length();
    int k;

    for(int i=0; i<rows; i++)
    {

        k=1;
        for(int j=1; j<=templen; j++)
        {
            if(k==j)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
                cout << datablock[i][j - 1];
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                k=k*2;
            }
            else
                cout << datablock[i][j - 1];

        }

        cout<< "\n";
    }

    cout<< "\n\n" ;
    ///task 4:Serialize the block in column major manner

    cm="";
    columns=datablock[0].length();

    for(int i=0; i<columns; i++)
    {
        for(int j=0; j<rows; j++)
        {
            cm.push_back(datablock[j][i]);
        }
    }

    cout<< "data bits after column-wise serialization:\n";
    cout<< cm;
    cout<< "\n\n\n";

    ///task 5: compute the CRC checksum
    CRCcalculation();
    cout<< "\n\n\n";
    ///task 6: simulate physical transmission

    cout<< "received frame:\n";
    int l=cm.length();
    int flagiserror[l];
    for(int i=0; i<l; i++)
    {
        flagiserror[i]=0;
    }
    // construct a trivial random generator engine from a time-based seed:
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);

    uniform_real_distribution<double> dist(0.0, 1.0);

    for (int i = 0; i < l; i++)
        randomprobabilities.push_back(dist(generator));


    for(int i=0; i<l; i++)
    {
        if(randomprobabilities[i] <= p)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);

            if(cm[i]=='0')
                cm[i]='1';
            else
                cm[i]='0';

            cout << cm[i];
            flagiserror[i]=1;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
        }
        else
            cout<< cm[i];
    }


    cout<< "\n\n\n";


    ///task 7: verify the correctness of the received frame

    tempstr=Modulo2Div(cm,generatorpolynimial);

    cout<< "result of the CRC checksum matching: ";

    if(tempstr=="0")
        cout<< "no error\n\n\n";
    else
        cout<< "error detected\n\n\n";




    ///task 8:remove the CRC checksum and make block again

    //removing crc

    templen=generatorpolynimial.length();

    vector<bool>flagiserror2[rows];


    for(int i=1; i<=templen-1; i++)
    {
        cm.pop_back();
    }

    //make the received data block

    templen=cm.length();
    int idx=0;


    for(int i=0; i<templen; i++)
    {
        datablock2[idx].push_back(cm[i]);
        if(flagiserror[i]==0)
            flagiserror2[idx].push_back(false);
        else
            flagiserror2[idx].push_back(true);

        idx++;
        if(idx==rows)
        {
            idx=0;
        }
    }


    cout<< "data block after removing CRC checksum bits:\n";

    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<columns; j++)
        {
            if(flagiserror2[i][j]==true)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
                cout<<datablock2[i][j];
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
            }
            else
                cout<<datablock2[i][j];
        }

        cout<< "\n";
    }


    cout<< "\n\n\n";

    return 0;
}