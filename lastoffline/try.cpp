#include <bits/stdc++.h>
#include <windows.h>

using namespace std;

/*15 is white
    // 10 is green
    // 11 is cyan
    // 12 is red
    //15 is white
    for(int i = 0; i <= 16 ; i++)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
        cout<<"Color : "<<i<<endl;
    }*/

string data, genPol, CRCChecksum;
int byteCount;
double probability;
int rows, length;
vector <bool> corruptionCheck;

void getInput()
{
    cout << "\nEnter string to transmit : \n";
   // cin >> data;
    getline(cin, data);
    cout << "\nEnter number of bytes in a row of the data block : \n";
    cin >> byteCount;
    cout << "\nEnter the probability of each bit being toggled during the transmission : \n";
    cin >> probability;
    cout << "\nEnter generator polynomial, for calculating and verifying CRC checksum : \n";
    cin >> genPol;

    cout << data << " " << byteCount << " " << probability << " " << genPol << endl;
}

string getBitString(string in)
{
    string str = "";

    for(int k = 0; k < byteCount; k++)
    {
        int cInt = (int) in[k];

        //cout << "check integer value " << cInt << endl;

        for(int i = 0; i < 8; i++)
        {
            if(cInt%2 == 1) str.push_back('1');
            else str.push_back('0');
            cInt = cInt/2;
        }
    }

    reverse(str.begin(), str.end());

    return str;
}

string getCheckBitString(string in)
{
    string str;
    int len = in.length() + log2((double) in.length()) + 1;
    int flag = 1, k = 0, temp, position = 1;

   // cout << "length check " << len << endl;

    for(int i = 0; i < len; i++)
    {
        if((i+1)/flag == 1)
        //if((i+1)%flag == 0)
        {
            str.push_back('0');
            flag = flag*2;
            continue;
        }

        str.push_back(in[k]);

        if(in[k] == '1')
        {
            temp = i + 1;
            position = 1;
            while(temp)
            {
                if(temp&1)
                {
//                    if(str[position-1] == '1') str[position-1] = '0';
//                    else str[position-1] = '1';
                    str[position-1] = ((str[position-1] - '0')^1) + '0';
                }

                position = position * 2;
                temp = temp/2;
            }
        }

        k++;
    }

    flag = 1;

    for(int i = 0; i < len; i++)
    {
        if((i+1)/flag == 1)
        //if((i+1)%flag == 0)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            cout << str[i];
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            flag = flag*2;
            continue;
        }

        cout << str[i];
    }

    cout << endl;

    length = len;
    return str;
}

string Hamming(string in)
{
    string str = "";
    int len = in.length();
    int flag = 1, temp, position;

    for(int i = 0; i < len; i++)
    {
        str.push_back(in[i]);
    }

    for(int i = 0; i < len; i++)
    {
        if((i+1)/flag == 1)
        //if((i+1)%flag == 0)
        {
            flag = flag*2;
            continue;
        }

        else
        {
             if(str[i] == '1')
            {
                temp = i + 1;
                position = 1;
                while(temp)
                {
                    if(temp&1)
                    {
                        str[position-1] = ((str[position-1] - '0')^1) + '0';
                    }

                    position = position * 2;
                    temp = temp/2;
                }
            }
        }
    }

//    flag = 1;
//
//    for(int i = 0; i < len; i++)
//    {
//        if((i+1)/flag == 1)
//        //if((i+1)%flag == 0)
//        {
//            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
//            cout << str[i];
//            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
//            flag = flag*2;
//            continue;
//        }
//
//        cout << str[i];
//    }
//
//    cout << endl;

    return str;
}

string errorCorrection(string in)
{
    string str = "";
    int flag = 1, k = 0;

    for(int i = 0; i < length; i++)
    {
        //if((i+1)/flag == 1)
        if((i+1)%flag == 0)
        {
            if(in[i] == '1')
            {
                k += flag;
                flag = flag*2;
            }
        }
    }

    if(k != 0)
    {
        flag = 1;
        for(int i = 0; i < length; i++)
        {
            if((i+1)%flag != 0)
            {
                str.push_back(in[i]);
            }
        }

        str[k] = ((str[k] - '0')^1) + '0';
    }

    return str;
}

string getReceivedDataString(string in)
{
    string str = errorCorrection(in);
    int flag = 1, k = 0;

    for(int i = 0; i < length; i++)
    {
        //if((i+1)/flag == 1)
        if((i+1)%flag == 0)
        {
            flag = flag*2;
        }
        else
        {
            str.push_back(in[i]);
            //k++;
        }
    }

    return str;
}

string getCRCChecksum(string in)
{
    string checksum = "";

    int len = in.length() - genPol.length() + 1;

    for(int i = 0; i < len; i++)
    {
        if(in[i] == '0') continue;

        for(int j = 0; j < genPol.length(); j++)
        {
            in[i+j] = (in[i+j] - '0')^(genPol[j] - '0') + '0';
        }
    }

    for(int i = len; i < in.length(); i++)
    {
        checksum.push_back(in[i]);
    }

    return checksum;
}

string corruption(string in)
{
    double p = 100*probability;
    int temp;

    for(int i = 0 ; i < in.length() ; i++)
    {
        corruptionCheck[i] = false;

        temp = rand()%100;

        if(temp < p)
        {
            in[i] = (in[i] - '0')^1 + '0';
            corruptionCheck[i] = true;
        }
    }

    return in;
}

string getStr(string in)
{
    string str = "";
    int n = 0;
    char c;

    for(int i = 0; i < byteCount; i++)
    {
        int flag = 1;
        for(int j = 0; j < 8; j++)
        {
            if(in[i*8+j] == '1') n += flag;
            flag = flag*2;
        }

        c = (char) n;
        str.push_back(c);
    }

    return str;
}

int main()
{
    srand(time(NULL));

    freopen("in.txt", "r", stdin);

    getInput();

    length = data.length();
    rows = length/byteCount;
    int quotient = length%byteCount;

    if(quotient != 0)
    {
        for(int i = 0; i <= quotient; i++)
        {
            data.push_back('~');
        }

        rows++;
    }

    //cout << endl << data << endl;

    string dataBlock[rows];
    string dataReceived[rows];
    string bitBlock[rows];
    string checkBitBlock[rows];
    string receivedDataBlock[rows];
    string receivedBitBlock[rows];
    string dataToSend = "";
    string receivedData = "";
    string tempStr = "";

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < byteCount; j++)
        {
            dataBlock[i].push_back(data[i*byteCount+j]);
        }
    }

    cout << "data block (ascii code of m characters per row)\n";
    for(int i = 0; i < rows; i++)
    {
        //cout << dataBlock[i] << endl;
        bitBlock[i] = getBitString(dataBlock[i]);
        cout << bitBlock[i] << endl;
    }

    cout << "data block after adding check bits\n";
    for(int i = 0; i < rows; i++)
    {
        checkBitBlock[i] = getCheckBitString(bitBlock[i]);
        //cout << checkBitBlock[i] << endl;
    }

    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < rows; j++)
        {
            dataToSend.push_back(checkBitBlock[j][i]);
        }
    }

    cout << "data bits after column wise serialization\n";
    cout << dataToSend << endl;
    cout << dataToSend.length() << endl;

    string tempDataToSend = "";

    for(int i = 0; i < dataToSend.length(); i++)
    {
        tempDataToSend.push_back(dataToSend[i]);
    }

    for(int i = 1; i < genPol.length(); i++)
    {
        tempDataToSend.push_back('0');
    }

    //cout << tempDataToSend << endl;

    CRCChecksum = getCRCChecksum(tempDataToSend);

    cout << "\nChecksum : ";
    cout << CRCChecksum << endl;

    for(int i = 0; i < CRCChecksum.length(); i++)
    {
        dataToSend.push_back(CRCChecksum[i]);
    }

    cout << "data bits after appending checksum\n";

    for(int i = 0; i < dataToSend.length(); i++)
    {
        if(i >= length*rows)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
            cout << dataToSend[i];
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        }

        else
            cout << dataToSend[i];
    }

    cout << endl;

    tempDataToSend.clear();
    tempDataToSend = "";

    for(int i = 0; i < dataToSend.length(); i++)
    {
        tempDataToSend.push_back(dataToSend[i]);
    }

    corruptionCheck.resize(tempDataToSend.length());

    receivedData = corruption(tempDataToSend);

    cout << "received data bits\n";
    //cout << receivedData << endl;
    for(int i = 0; i < receivedData.length(); i++)
    {
        if(corruptionCheck[i])
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            cout << receivedData[i];
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        }

        else
            cout << receivedData[i];
    }

    cout << endl <<receivedData.length() << endl;

    string currentChecksum = getCRCChecksum(receivedData);

    cout << "received data bits checksum\n";
    cout << currentChecksum << endl;

    for(int i = 0; i < genPol.length() - 1; i++)
    {
        tempStr.push_back('0');
    }

    if(tempStr == currentChecksum)
    {
        cout << "\nNo error detected\n";
    }

    else
    {
        cout << "\nError detected\n";
    }

//    tempStr.clear();
//
//    for(int i = 0; i < length*rows; i++)
//    {
//        tempStr.push_back(receivedData[i]);
//    }
//
//    //cout << tempStr << endl;

    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < rows; j++)
        {
            receivedBitBlock[j].push_back(tempStr[rows*i + j]);
//            if(corruptionCheck[rows*i + j])
//            {
//                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
//                cout << tempStr[rows*i + j];
//                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
//            }
//
//            else
//                cout << tempStr[rows*i + j];
        }
    }

    cout << "data block after removing crc checksum bits : \n";

    for(int i = 0; i < rows; i++)
    {
        cout << receivedBitBlock[i] << endl;
        //cout << receivedBitBlock[i].size() << endl;
    }

    //cout << "Applying " << endl;

    for(int i = 0; i < rows; i++)
    {
        receivedBitBlock[i] = Hamming(receivedBitBlock[i]);
        //cout << receivedBitBlock[i] << endl;
    }

    cout << "data block after removing check bits : \n";

    for(int i = 0; i < rows; i++)
    {
        receivedDataBlock[i] = getReceivedDataString(receivedBitBlock[i]);
        cout << receivedDataBlock[i] << endl;
        //receivedBitBlock[i].clear();
    }

    for(int i = 0; i < rows; i++)
    {
        dataBlock[i] = getStr(receivedDataBlock[i]);
        //int n = (int) dataBlock[i];
        cout << dataBlock[i] << endl;
    }

    receivedData = "";
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < byteCount; j++)
        {
            receivedData.push_back(dataBlock[i][j]);
        }
    }

    cout << receivedData << endl;

    return 0;
}