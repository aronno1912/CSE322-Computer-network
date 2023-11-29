#include<bits/stdc++.h>
#include <windows.h>
#define GREEN 10
#define RED 12
#define CYAN 11
#define WHITE 15

using namespace std;

string givenInputString;
string generatorpolynimial;
vector<string>datablock;
vector<string>datablock2;
string columMajorString;
int m,rows,columns;
double probability;


//This function takes an integer n as input and returns true if n is a power of 2, and false otherwise.
bool isPowerOfTwo(int n) {
    if (n <= 0) {
        return false;
    }
    return (n & (n - 1)) == 0; //it will be true if n is a power of 2...like 1000(8)...1000-1=0111... 1000 & 0111=0
}

string asciiToBinary(char character) {
    int asciiValue = static_cast<int>(character); // Convert the character to its ASCII value
    std::bitset<8> binaryValue(asciiValue); // Create a bitset with 8 bits

    return binaryValue.to_string();
}

string divisionModulo2(string dividend, string divisor)
{

    
    string remainder;
    int dividendIndex ; //to keep track of the division

    if(dividend.length() < divisor.length())
        return dividend;

    int x=divisor.length();
    //The remainder string is initialized with the initial portion of the dividend equal to the length of the divisor.
    // This sets up the initial state for the division.
    for(int i=0; i<x; i++)
    {
        remainder.push_back(dividend[i]);
    }

    dividendIndex=x;
    string tempstr;
    while(divisor.length()==remainder.length())
    {

        //Binary division is performed by XORing corresponding bits of remainder and divisor. This step calculates the remainder of the division.
        for(int i=0; i<x; i++)
        {
            remainder[i] = ((remainder[i]-'0')^(divisor[i]-'0')) + '0'; //converts that character to integer and vice versa
        }

        //Leading zeros are removed from the remainder by copying only the part of the remainder starting from the first '1' encountered.
         tempstr= "";
        for(int i=0; i<x; i++)
        {
            if(remainder[i]=='1')
            {
                for(int j=i; j<x; j++)
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

char binaryToChar(const string& binaryString) 
{
    if (binaryString.length() != 8) 
    {
        throw invalid_argument("Binary string must be exactly 8 bits long.");
    }

    std::bitset<8> binaryValue(binaryString);
    char character = static_cast<char>(binaryValue.to_ulong());

    return character;
}



//here parameter is int.. chracter is converted to its ascii value
string convertToBinary(int number)
{
    string binaryString="";

    while(number!=0)
    {
        binaryString.push_back((number % 2) + '0');
        number=number/2;
    }
    while(binaryString.size() < 8)
    {
        binaryString.push_back('0');
    }

    reverse(binaryString.begin(),binaryString.end());

    return binaryString;

}

int bitwiseXOR(int bit1, int bit2)
 {
    return (bit1 != bit2) ? 1 : 0;
}

// Hamming Code for error detection
void insertRedundentBits(int numOfRedundentBits)
{
    int originalDataLength,numberOf1s;
    string dataString,trackingStringForParityCal; //for each row
    int pos;
    int temp2;

    for(int i=0; i<rows; i++)
    {
        numberOf1s=0;
        originalDataLength=datablock[i].length();

        //taking a string with originaldatalenth+r length and fill each position with blank initially
        dataString.resize(numOfRedundentBits + originalDataLength);
        for(int j=0; j< originalDataLength+numOfRedundentBits ; j++)
        {
            dataString[j]=' ';
        }

        //determining redundent bit positions with #
        pos=1;
        for(int j=1; j<=numOfRedundentBits; j++)
        {
            dataString[pos-1]='#';
            pos=pos*2;
        }

        //fill up without the redundent bit  positions
        pos=0;
        for(int j=0; j<originalDataLength; j++)
        {
            while(dataString[pos]!= ' ')
            {
                pos++;
            }
            dataString[pos]=datablock[i][j]; //fill up with original data bits
        }

        //now fill the redundent bit positions with actual value
        originalDataLength +=numOfRedundentBits;
        temp2=1; //redundentbits
        int k;
        for(int j=1; j<=numOfRedundentBits; j++) //iterate for number of redundent bits in a row
        {
            numberOf1s=0;
            // calculating the value for the j-th parity bit, so you need to iterate over all the
            // data bit positions in the str string to calculate the parity bit's value.
            for(k=j+1; k<=originalDataLength; k++)
            {
                trackingStringForParityCal=convertToBinary(k);
                reverse(trackingStringForParityCal.begin(),trackingStringForParityCal.end());
                // temstr e j jei bit e 1 ache oigula position e main str er bit value check korte hobe
                // let for r1, need to check 1,3,5,7 ... data bits...
                if(trackingStringForParityCal[j-1]=='1' && dataString[k-1]=='1')
                {
                    numberOf1s++;
                }

            }

            if(numberOf1s%2 ==0)  //(indicating an even number of '1's in the group).
            {
                dataString[temp2-1]='0';
            }
            else
            {
                dataString[temp2-1]='1';
            }

            temp2=temp2*2;
        }

        datablock[i]=dataString;

    } //done with each row

}


// followed the implementation from book
void CRCchecksumComputation()
{
    string tempstr=columMajorString;
    int degreePlusOne=generatorpolynimial.length();
    
    //append degree amount of zeros at the last
    for(int i=0; i<degreePlusOne-1; i++)
    {
        tempstr.push_back('0');
    }
    string remainder;
    remainder=divisionModulo2(tempstr,generatorpolynimial);

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


/***
 * For all the parity bits we will check the number of 1’s in their respective bit positions.

For R1: bits 1, 3, 5, 7, 9, 11. We can see that the number of 1’s in these bit positions are 4 and that’s even so we get a 0 for this.

For R2: bits 2,3,6,7,10,11 . We can see that the number of 1’s in these bit positions are 5 and that’s odd so we get a 1 for this.

For R4: bits 4, 5, 6, 7 . We can see that the number of 1’s in these bit positions are 3 and that’s odd so we get a 1 for this.

For R8: bit 8,9,10,11 . We can see that the number of 1’s in these bit positions are 2 and that’s even so we get a 0 for this.

The bits give the binary number 0110 whose decimal representation is 6. Thus, bit 6 contains an error. To correct the error the 6th bit is changed from 1 to 0.
 * 
*/

// Function to correct errors based on parity checks
void correctErrors(string &data) 
{
    int n = data.size(); // Total number of bits in the data
    vector<int> parityPositions; // Stores positions of parity bits

    // Determine positions of parity bits and initialize variables
    int parityBit = 1;
    int dataIndex = 0; // Index for data bits (0-based)
    
    // Calculate positions of parity bits and initialize variables
    while (dataIndex < n) 
    {
        if ((dataIndex + 1) == parityBit) 
        {
            parityPositions.push_back(dataIndex); // Position of a parity bit
            parityBit *= 2; // Move to the next parity bit position
        }
        dataIndex++;
    }

    // Initialize parity check variables
    vector<int> parityValues(parityPositions.size(), 0);

    // Calculate parity values for each parity bit
    for (size_t i = 0; i < parityPositions.size(); i++)  //iterate for all checkbits(redundent bits)
    {
        int parityIndex = parityPositions[i];
        for (int j = parityIndex; j < n; j++)  //including parity bit
        {
            if ((j + 1) & (1 << i))  // checks whether the current bit at position j is associated with the current parity bit at position i
             {
                // Check if the current bit is associated with the parity bit
                if (data[j] == '1')  // If a bit is '1', it toggles the corresponding parity bit's value in the parityValues vector. need to find error position
                 {
                    parityValues[i] ^= 1; // Toggle the parity bit
                 }
            }
        }
    }
   
    // Determine the error bit position
    int errorBitPosition = 0;
    for (size_t i = 0; i < parityValues.size(); i++) 
    {
        errorBitPosition += parityValues[i] * (1 << i); //if parity values==1 then error exists...if parityval=0,no error..all okay
    }

    // Correct the error bit if an error is detected
    if (errorBitPosition > 0) {
        // Toggle the error bit
        data[errorBitPosition - 1] = (data[errorBitPosition - 1] == '0') ? '1' : '0';
    }
}






int main()
{
    int datastringlength,templen;
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
    int rowIndex=0;

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
        datablock[rowIndex]+=(convertToBinary(givenInputString[i]));

        if((i+1)%columns ==0)   //it means that a new row needs to start. So, index is incremented to move to the next row in the datablock vector
        {
            rowIndex++;
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
    //cout<<endl<<"number of r bits is "<<r<<endl;
    insertRedundentBits(r);
    cout<< "\n\n";

    //print after adding check bits
    cout<< "data block after adding check bits:\n";
    templen=datablock[0].length();
    int checkBit;

    for(int i=0; i<rows; i++)
    {

        checkBit=1; 
        for(int j=1; j<=templen; j++)
        {
            if(checkBit==j)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
                cout << datablock[i][j - 1];
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
                checkBit=checkBit*2;
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
    int cmStringLength=columMajorString.length();
    int trackErrorBits[cmStringLength];
    for(int i=0; i<cmStringLength; i++)
    {
        trackErrorBits[i]=0;
    }

    // construct a trivial random generator engine from a time-based seed:
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<double> dist(0.0, 1.0);
    vector<double>randomprobabilities;

    for (int i = 0; i < cmStringLength; i++)
        randomprobabilities.push_back(dist(generator));


    for(int i=0; i<cmStringLength; i++)
    {
        if(randomprobabilities[i] <= probability)
        {
            trackErrorBits[i]=1;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
            if(columMajorString[i]=='0')
                columMajorString[i]='1';
            else
                columMajorString[i]='0';

            cout << columMajorString[i];
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
        }
        else
            cout<< columMajorString[i];
    }
    cout<< "\n\n\n";


    //Task 7 : verify the correctness. if the received frame is divisible by generator polynomial then no error


    string result;
    result=divisionModulo2(columMajorString,generatorpolynimial);
    cout<< "result of the CRC checksum matching: ";

    if(result=="0")
        cout<< "no error\n\n\n";
    else
        cout<< "error detected\n\n\n";




    // TASK 8 : Remove the CRC checksum bits from the data stream and de-serialize it into the data-block in a 
    //column-major fashion. Print the data block. Note that, the erroneous bits must be shown in a different 
    //color (red). 

    //removing crc

    vector<bool>rowWiseErrorBitTrack[rows];
    for(int i=1; i<=generatorpolynimial.length()-1; i++)
    {
        columMajorString.pop_back();
    }

    //make the received data block

    templen=columMajorString.length();
    int idx=0;
    for(int i=0; i<templen; i++)
    {
        datablock2[idx].push_back(columMajorString[i]);
        if(trackErrorBits[i]==0)
            rowWiseErrorBitTrack[idx].push_back(false);
        else
            rowWiseErrorBitTrack[idx].push_back(true);

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
            if(rowWiseErrorBitTrack[i][j]==true)
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

    for (string &row : datablock2) {
        correctErrors(row);
    }

    int rowss = datablock2.size();
    int columnss = datablock2[0].size();

    string outputframe = "";
    cout << "Data block after removing  check bits\n";
    for (int i = 0; i < rowss; i++)
    {
         string binaryStr = ""; // Initialize a binary string to store the bits

    for (int j = 0; j < columnss; j++)
    {
        // Check if j+1 (1-based index) is a power of 2
        bool isCheckBit =isPowerOfTwo(j+1);

        if (!isCheckBit)
        {   
             binaryStr += datablock2[i][j]; // Append the bit to the binary string
                cout << datablock2[i][j];
            
        }
    }

    outputframe += binaryStr;

    cout << "\n";
    }

    // Convert the binary string to ASCII characters
    string dataString = "";
    for (size_t i = 0; i < outputframe.length(); i += 8) 
    {
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
