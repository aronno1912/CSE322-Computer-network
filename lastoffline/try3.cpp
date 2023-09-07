#include<bits/stdc++.h>
#include <windows.h>
#define GREEN 10
#define RED 12
#define CYAN 11
#define WHITE 15

using namespace std;

string givenInputString,generatorpolynimial;
string columMajorString;
int m,rows,columns;
double probability;
vector<string>datablock;
vector<string>datablock2;
vector<double>randomprobabilities;

//here parameter is int.. chracter is converted to its ascii value

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

void insertRedundentBits(int r)
{
    int originalDataLength,numberOf1s;
    string str,trackingStringForParityCal; //for each row
    int temp,temp2;
    int pos;

    for(int i=0; i<rows; i++)
    {
        numberOf1s=0;
        originalDataLength=datablock[i].length();

        //taking a string with originaldatalenth+r length and fill each position with blank initially
        str.resize(r+originalDataLength);
        for(int j=0; j< originalDataLength+r ; j++)
        {
            str[j]=' ';
        }

        //determining redundent bit positions with #
        pos=1;
        for(int j=1; j<=r; j++)
        {
            str[pos-1]='#';
            pos=pos*2;
        }

        //fill up without the redundent bit  positions
        pos=0;
        for(int j=0; j<originalDataLength; j++)
        {
            while(str[pos]!= ' ')
            {
                pos++;
            }
            str[pos]=datablock[i][j]; //fill up with original data bits
        }

        //now fill the redundent bit positions with actual value
        originalDataLength+=r;
        temp2=1;
        int k;
        for(int j=1; j<=r; j++) //iterate for number of redundent bits in a row
        {
            numberOf1s=0;
            // calculating the value for the j-th parity bit, so you need to iterate over all the
            // data bit positions in the str string to calculate the parity bit's value.
            for(k=j+1; k<=originalDataLength; k++)
            {
                trackingStringForParityCal=BinaryConversion(k);
                reverse(trackingStringForParityCal.begin(),trackingStringForParityCal.end());
                // temstr e j jei bit e 1 ache oigula position e main str er bit value check korte hobe
                if(trackingStringForParityCal[j-1]=='1' && str[k-1]=='1')
                {
                    numberOf1s++;
                }

            }

            if(numberOf1s%2 ==0)  //(indicating an even number of '1's in the group).
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

    } //done with each row

}

string Modulo2Div(string dividend, string divisor)
{
    if(dividend.length() < divisor.length())
        return dividend;

    string remainder,tempstr;
    int dividendIndex ; //to keep track of the division

    int slength=divisor.length();
    //The remainder string is initialized with the initial portion of the dividend equal to the length of the divisor.
    // This sets up the initial state for the division.
    for(int i=0; i<slength; i++)
    {
        remainder.push_back(dividend[i]);
    }

    dividendIndex=slength;

    while(divisor.length()==remainder.length())
    {

        //Binary division is performed by XORing corresponding bits of remainder and divisor. This step calculates the remainder of the division.
        for(int i=0; i<slength; i++)
        {
            remainder[i] = ((remainder[i]-'0')^(divisor[i]-'0')) + '0'; //converts that character to integer and vice versa
        }

        //Leading zeros are removed from the remainder by copying only the part of the remainder starting from the first '1' encountered.
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
        while(dividendIndex < dividend.length() && remainder.length() < divisor.length())
        {
            remainder.push_back(dividend[dividendIndex]);
            dividendIndex++;
        }


    }
    // if the remainder is empty that means remainder is zero!!!!!!!!!!!!
    if(remainder.length()==0)
    {
        remainder="0";
    }

    return remainder;

}
// followed the implementation from book
void CRCchecksumComputation()
{
    string tempstr=columMajorString;
    string remainder;

    int degreePlusOne=generatorpolynimial.length();
    
    //append degree amount of zeros at the last
    for(int i=0; i<degreePlusOne-1; i++)
    {
        tempstr.push_back('0');
    }
    
    remainder=Modulo2Div(tempstr,generatorpolynimial);

   // to add 0 in front to make the remainder size=degree of the generator polynomial
    reverse(remainder.begin(),remainder.end());
    while(remainder.length()< degreePlusOne-1)
    {
        remainder.push_back('0'); 
    }
    reverse(remainder.begin(),remainder.end());

    //printing
    cout<< "data dits after appending CRC checksum <sent frame>: \n";
    cout << columMajorString;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CYAN);
    cout << remainder;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
    columMajorString =columMajorString+remainder;
}

// void CorrectErrorsAndPrintDataBlock(vector<string>& datablock2,  vector<bool>&flagiserror2) {
//     int rows = datablock2.size();
//     int columns = datablock2[0].size();

//     for (int i = 0; i < rows; i++) {
//         // Calculate parity bits based on received data bits
//         vector<char> calculatedParityBits;
//         for (int j = 0; j < columns; j++) {
//             if (flagiserror2[i][j]) {
//                 // This bit was received in error, exclude it from parity calculation
//                 continue;
//             }

//             calculatedParityBits.push_back(datablock2[i][j]);
//         }

//         // Compare calculated parity bits with received parity bits
//         bool errorFound = false;
//         int errorPosition = 0;

//         for (int j = 0; j < calculatedParityBits.size(); j++) {
//             char calculatedBit = calculatedParityBits[j];
//             char receivedBit = datablock2[i][columns - 1 - j]; // Parity bits are at the end

//             if (calculatedBit != receivedBit) {
//                 errorFound = true;
//                 errorPosition += pow(2, j);
//             }
//         }

//         // Correct the error if one was found
//         if (errorFound) {
//             datablock2[i][columns - 1 - errorPosition] = (datablock2[i][columns - 1 - errorPosition] == '0') ? '1' : '0';
//         }
//     }

//     // Print the corrected data block after removing check bits
//     cout << "Corrected data block after removing check bits:\n";
//     for (int i = 0; i < rows; i++) {
//         for (int j = 0; j < columns - log2(columns + 1); j++) { // Remove check bits
//             cout << datablock2[i][j];
//         }
//         cout << "\n";
//     }
// }

// void CorrectErrorsAndPrintDataBlock(vector<string>& datablock2, vector<vector<bool>>& flagiserror2)
// {
//     int rows = datablock2.size();
//     int columns = datablock2[0].size();
    
//     // Loop through each row in the data block
//     for (int i = 0; i < rows; i++)
//     {
//         int errorCount = 0;
//         int errorBit = -1; // Initialize errorBit to -1
        
//         // Check for errors in the row
//         for (int j = 0; j < columns; j++)
//         {
//             if (flagiserror2[i][j])
//             {
//                 errorCount++;
//                 errorBit = j; // Store the index of the first error bit
//             }
//         }
        
//         // If there is only one error in the row, correct it
//         if (errorCount == 1)
//         {
//             // Toggle the error bit
//             datablock2[i][errorBit] = (datablock2[i][errorBit] == '0') ? '1' : '0';
            
//             // Print the corrected bit in red
//             SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
//             cout << datablock2[i][errorBit];
//             SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
//         }
//         else
//         {
//             // Print the row as is (without corrections)
//             for (int j = 0; j < columns; j++)
//             {
//                 if (flagiserror2[i][j])
//                 {
//                     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
//                 }
//                 cout << datablock2[i][j];
//                 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
//             }
//         }
        
//         // Move to the next row
//         cout << endl;
//     }
// }







int main()
{
    int temp1,temp2,datastringlength,templen;
    string tempstr;

    cout<< "enter data string: ";
    getline(cin,givenInputString);

    cout<< "enter number of data bytes in a row <m>: ";
    cin>> m;

    cout<< "enter probability <p>: ";
    cin>> probability;

    cout<< "enter generator polynomial: ";
    cin>> generatorpolynimial;

    cout<< "\n\n\n";


    // TASK 1 : If the size of the data string is not a multiple of m, append the padding character (~) to the data string accordingly. Print the updated data string.

    while((givenInputString.size()% m) != 0)
    {
        givenInputString.push_back('~');
    }
    cout<< "data string after padding: " << givenInputString << endl;
    cout<< "\n" ;


    ///task 2: create data blocks of ascii values
    /***
     *  TASK 2 : Create the data block, which contains the 8-bit binary representation of the ascii codes of the 
    characters of the data string. Each row contains ascii codes of m characters. The first row shows the first 
    m characters, the second row shows the next m characters, and so on.
    */

    datastringlength=givenInputString.length();
    rows=datastringlength/m;
    columns=m; //each 8 bit indicates 1 column
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
        datablock[index]+=(BinaryConversion(givenInputString[i]));

        if((i+1)%columns ==0)   //it means that a new row needs to start. So, index is incremented to move to the next row in the datablock vector
        {
            index++;
        }
    }

    cout<< "data block <ascii code of m characters per row>:\n";

    for(int i=0; i<rows; i++)
    {
        cout<< datablock[i] << endl;
    }

    /** TASK 3:  Add check bits  in each row of the data block (according to page 195, 
    figure 3-7 of [1]). Print the updated data block          **/
    //add check bits
    //first calculate the number of redundant bits r .for 16 bit , 2^4=16... so, 2^0,2^1,2^2,2^3,2^4..total 5 redundent bits per row is needed

    int r = 0;
    while (pow(2, r) < (r + m*8 + 1))
    {
    r++;
    }
    cout<<endl<<"number of r bits is "<<r<<endl;
    insertRedundentBits(r);
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

    // TASK 4 : Serialize the above data block in column-major manner. Print the serialized data bits

    columMajorString="";
    columns=datablock[0].length();

    for(int i=0; i<columns; i++)
    {
        for(int j=0; j<rows; j++)
        {
            columMajorString.push_back(datablock[j][i]);
        }
    }

    cout<< "data bits after column-wise serialization:\n";
    cout<< columMajorString;
    cout<< "\n\n\n";

    /**
     * TASK 5 : Compute the CRC checksum of the above bit stream using the generator polynomial (according to 
    page 197-198, figure 3-8 of [1]). Append the checksum to the bit stream. This is the frame to be 
    transmitted. Print the frame.
     * 
     */
    CRCchecksumComputation();
    cout<< "\n\n\n";



    //TASK 6: Simulate the physical transmission by toggling each bit of the stream with a probability of p.


    cout<< "received frame:\n";
    int l=columMajorString.length();
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
        if(randomprobabilities[i] <= probability)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);

            if(columMajorString[i]=='0')
                columMajorString[i]='1';
            else
                columMajorString[i]='0';

            cout << columMajorString[i];
            flagiserror[i]=1;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
        }
        else
            cout<< columMajorString[i];
    }


    cout<< "\n\n\n";


    //Task 7 : verify the correctness. if the received frame is divisible by generator polynomial then no error

    tempstr=Modulo2Div(columMajorString,generatorpolynimial);

    cout<< "result of the CRC checksum matching: ";

    if(tempstr=="0")
        cout<< "no error\n\n\n";
    else
        cout<< "error detected\n\n\n";




    // TASK 8 : Remove the CRC checksum bits from the data stream and de-serialize it into the data-block in a 
    //column-major fashion. Print the data block. Note that, the erroneous bits must be shown in a different 
    //color (red). 

    //removing crc

    templen=generatorpolynimial.length();

    vector<bool>flagiserror2[rows];


    for(int i=1; i<=templen-1; i++)
    {
        columMajorString.pop_back();
    }

    //make the received data block

    templen=columMajorString.length();
    int idx=0;
    for(int i=0; i<templen; i++)
    {
        datablock2[idx].push_back(columMajorString[i]);
        if(flagiserror[i]==0)
            flagiserror2[idx].push_back(false);
        else
            flagiserror2[idx].push_back(true);

        idx++;
        if(idx==rows) //again start from row 1
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

    // Task 9: error correction if possible

    //CorrectErrorsAndPrintDataBlock(datablock2,flagiserror2);

    int rowss = datablock2.size();
    int columnss = datablock2[0].size();
    
    // Loop through each row in the data block
    for (int i = 0; i < rowss; i++)
    {
        int errorCount = 0;
        int errorBit = -1; // Initialize errorBit to -1
        
        // Check for errors in the row
        for (int j = 0; j < columnss; j++)
        {
            if (flagiserror2[i][j])
            {
                errorCount++;
                errorBit = j; // Store the index of the first error bit
            }
        }
        
        // If there is only one error in the row, correct it
        if (errorCount == 1)
        {
            // Toggle the error bit
            datablock2[i][errorBit] = (datablock2[i][errorBit] == '0') ? '1' : '0';
        }
    }
    string outputframe = "";
    cout << "Data block after removing  check bits\n";
    for (int i = 0; i < rowss; i++)
    {
         string binaryStr = ""; // Initialize a binary string to store the bits

    for (int j = 0; j < columnss; j++)
    {
        // Check if j+1 (1-based index) is a power of 2
        bool isCheckBit = ((j + 1) & (j + 1 - 1)) == 0;

        if (!isCheckBit)
        {   
             binaryStr += datablock2[i][j]; // Append the bit to the binary string
            if (flagiserror2[i][j] == true)
            {
                // SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
                cout << datablock2[i][j];
                //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
            }
            else
            {
                cout << datablock2[i][j];
            }
        }
    }

     // Convert the binary string to an ASCII character and append it to the output frame
    // char asciiChar = 0;
    // for (int k = 0; k < 8; k++) {
    //     asciiChar = (asciiChar << 1) + (binaryStr[k] - '0');
    // }
    // outputframe += asciiChar;
    outputframe += binaryStr;

    cout << "\n";
    }

    // Convert the binary string to ASCII characters
    string dataString = "";
    for (size_t i = 0; i < outputframe.length(); i += 8) {
    string binaryChar = outputframe.substr(i, 8);
    char asciiChar = 0;

    for (int j = 0; j < 8; j++) {
        asciiChar = (asciiChar << 1) + (binaryChar[j] - '0');
    }

    dataString += asciiChar;
    }
    cout << "\n";
    cout << "\n";
   
    cout << "Output Frame (Data String):\n" << dataString << endl;
   


    
  return 0;
}
