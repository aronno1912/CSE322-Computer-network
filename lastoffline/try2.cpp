/********************************
Offline 5 : Error Correction and detection
Pg : 141, Tanenbaum; Ch: 3.2

/******************************/
#include <bits/stdc++.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define vi vector<int>

using namespace std;

vi data_frame[1000], rec_frame[1000] ;
vi serialized, received;
vi key , error_idx;

enum Color {
    FG_BLACK    = 30,
    FG_RED      = 31,
    FG_GREEN    = 32,
    FG_BROWN    = 33,
    FG_BLUE     = 34,
    FG_MAGENTA  = 35,
    FG_CYAN     = 36,
    FG_LIGHTGREY= 37,
    FG_DEFAULT  = 39,
    BG_RED      = 41,
    BG_GREEN    = 42,
    BG_BLUE     = 44,
    BG_DEFAULT  = 49
};

void binary_Convert(int n, int k,int l) 
{ 
    // ascii of binary number 
    //cout<<endl<<n<<endl;
    int binaryNum[1000]; 
    memset(binaryNum,0,sizeof(binaryNum));
    // counter for binary array 
    int i = 0; 
    while (n > 0) { 
        // storing remainder in binary array 
        binaryNum[i] = n % 2; 
        n = n / 2; 
        i++; 
    } 

    while(i < 8)
        i++;
    
    // printing binary array in reverse order 
    for (int j = i - 1; j >= 0; j--){
        cout << binaryNum[j]; 
        data_frame[k].push_back(binaryNum[j]);
        l++;
    } 

} 

bool getParity(vi data_row , int idx) 
{ 
    bool parity = 0;
    int count = 0; 
    for(int i = 0; i < data_row.size(); i++)
    {
        if( (i+1) & idx && data_row[i]== 1){
            count++;
        }
    }
    if(count %2 == 0)
        return false;
    else
        return true;
    
} 

vi XOR(vi a, vi b){
    vi tmp;
    int i =0;
    // cout<<"\n---------\nb: ";
    // for(int i = 0; i < b.size(); i++)
    //     cout<<b[i];
    // cout<<"\n***********\na: ";
    // for(int i = 0; i < a.size(); i++)
    //     cout<<a[i];
    // cout<<"\n---------\n";
    
    
    for(int i = 0; i < b.size(); i++)
    {
        if(a[i] == b[i])
            tmp.push_back(0);
        else
            tmp.push_back(1);
        // tmp.push_back(a[i]^b[i]);
    }
    return tmp;
}

vi Division(vi frame){
    int divisor_len = key.size();
    int zer_len = divisor_len;
    vi temp(frame.begin(), frame.begin()+divisor_len);
    // for(int i = 0; i < temp.size(); i++)
    //     cout<<temp[i];
    // cout<<"\n---------\n";
    
    while(divisor_len < frame.size()){
        if(temp[0] == 1){//First element 1
            vi tmp = XOR(key , temp);
            temp.clear();
            temp = tmp;
            temp.push_back(frame[divisor_len]);
        }
        else{
            vi zero(zer_len,0);
            vi tmp = XOR( zero, temp);
            temp.clear();
            temp = tmp;
            temp.push_back(frame[divisor_len]);
        }
        divisor_len++;
        if(temp[0] == 0){
            temp.erase(temp.begin(), temp.begin()+1);
        }
        // for(int i = 0; i < temp.size(); i++)
        //     cout<<temp[i];
        // cout<<"\n=========\n";
        
    }
    if(temp[0] == 1){
        vi tmp = XOR(key,temp);
        temp.clear();
        temp = tmp;
    }
    else
    {
        vi zero(zer_len,0);
        vi tmp = XOR(zero,temp);
        temp.clear();
        temp = tmp;
    }
    if(temp[0] == 0){
            temp.erase(temp.begin(), temp.begin()+1);
    }
    return temp; 
}

int Error_Detection(vi received){
    int count  = 0;
    vi remainder  =  Division(received);
    for(int i = 0; i < remainder.size(); i++)
    {
        if(remainder[i] != 0)
        {
            count++;
        }    
    }
    return count;
}

float jimsrand(void)
{
    // double mmm = RAND_MAX;
    // float x;                 /* individual students may need to change mmm */
    // x = rand() / mmm;        /* x should be uniform in [0,1] */
    // return (x);
    
    const int range_from  = 0;
    const int range_to    = 1;
    random_device rand_dev;
    mt19937 generator(rand_dev());
    //uniform_int_distribution<int>  distr(range_from, range_to);
    uniform_real_distribution<float> distr(range_from,range_to);
    return distr(generator);

}

int main(int argc, char const *argv[])
{

    string data,polynom;
    int m;
    float p;

    cout<<"enter data string: ";
    getline(cin,data);
    cout<<"enter number of data bytes in a row <m>: ";
    cin>>m;
    cout<<"enter probability <p>: ";
    cin>>p;
    cout<<"enter generator polynomial: ";
    cin>>polynom;

    // data = "Hamming Code";
    // data = "a";
    // m = 2;
    // p = 0.05;
    // polynom = "10101";

    if( p > 1.0 || m <= 0 ){
        cout<<"Invalid argument..";
        return 0;
    }

    /// Padding data string ----------
    while(data.length() %m != 0 ){
        data += "~";
    }
    for(int i = 0; i < polynom.length(); i++)
    {
        if(polynom[i] == '1')
            key.push_back(1);
        else
            key.push_back(0);
    }
    
    cout<<"\ndata string after padding: "<<data<<endl;
    int row_siz = 8*m;
    int col_siz = data.length()/m;
    int total_bits = data.length()*8;
    
    /// Printing data block -----------
    cout<<"\ndata block <ascii code of m characters per row>:\n";

    int k = 0;
    for(int i = 0; i < data.length(); i++)
    {
        for(int j = 0; j < m; j++)
        {
            binary_Convert(data[i],k,j*8);
            if(j != m-1)
                i++;
            if(i == data.length())
                break;
        }
        k++;
        cout<<endl;
    }
    /// Adding Check bits ------------
    cout<<"\ndata block after adding check bits:\n";
    
    for(int j = 0; j < col_siz; j++){
        int i =0, k=1;
        while(i< m*8){
            if(!(k == 0) && !(k & (k - 1))){
                data_frame[j].insert(data_frame[j].begin()+i ,-1);
                k++;i++;
                continue;
            }
            i++;k++;
        }
    }

    for(int i =0 ; i< col_siz ; i++){
        for(int j = 0; j < data_frame[i].size(); j++)
        {
            if(data_frame[i][j] == -1){
                data_frame[i][j] =  getParity(data_frame[i],j+1);
                cout << "\033[1;"<<FG_GREEN<<"m"<<data_frame[i][j]<<"\033[0m";
            }
            else
            {
                cout<<data_frame[i][j];
            }
            
        }
        cout<<endl;
    }
    row_siz = data_frame[0].size();

    /// Serialising data ------------
    cout<<"\ndata bits after column-wise serialization:\n";
    for(int i = 0; i < row_siz; i++)
    {
        for (int j = 0; j < col_siz; j++)
        {
            cout<<data_frame[j][i];
            serialized.push_back(data_frame[j][i]);
        }
    }
    vi temp = serialized;
    for(int i =0 ; i< polynom.length()-1 ; i++)
        temp.push_back(0);

    /// Calculate CRC checksum -----------
    cout<<"\n\ndata bits after appending CRC checksum <sent frame>:\n";
    vi remainder = Division(temp);
    for(int i = 0; i < serialized.size(); i++)
    {
        cout<<serialized[i];
    }
    for(int i = 0; i < remainder.size(); i++)
    {
        cout << "\033[1;"<<FG_CYAN<<"m"<<remainder[i]<<"\033[0m";
        serialized.push_back(remainder[i]);
    }

    /// Prnting Received frame ---------
    cout<<"\n\nreceived frame:\n";
    for(int i = 0; i < serialized.size(); i++)
    {
        if (jimsrand() < p ){
            error_idx.push_back(i);    
            serialized[i] == 1 ? serialized[i]=0:serialized[i]=1;
            cout << "\033[1;"<<FG_RED<<"m"<<serialized[i]<<"\033[0m";
        }
        else
            cout<<serialized[i];
        received.push_back(serialized[i]);
    }
    /// Error Detection -------
    cout<<"\n\nresult of CRC checksum matching: ";
    Error_Detection(received)!= 0 ? cout<<"error detected": cout<<"no error detected";

    /// Removing Checksum ----------
    for(int i =0 ; i< polynom.length()-1 ; i++)
        received.pop_back();

    /// Deserialization -------- Bits are only toggled, but block size remains same
    cout<<"\n\ndata block after removing CRC checksum bits:\n";
    int r = 0;
    int c = 0;
    vector<pair<int,int>> err_ij;
    for(int i = 0; i < received.size(); i++)
    {
        rec_frame[r].push_back(received[i]);
        r = (r+1)%col_siz;
        if(error_idx.size()  > 0 && i == error_idx.front()){
            err_ij.push_back({r,rec_frame[r].size()-1});
            //cout<<"Error at : "<<rec_frame[r].size()-1<<" "<<r<<endl;
            error_idx.erase(error_idx.begin() , error_idx.begin()+1);
        }
    }
    
    for(int i =0 ; i< col_siz ; i++){
        for(int j = 0; j < rec_frame[i].size(); j++)
        {
            if(rec_frame[i][j] != data_frame[i][j]){
                cout << "\033[1;"<<FG_RED<<"m"<<rec_frame[i][j]<<"\033[0m";
            }
            else{
                cout<<rec_frame[i][j];
            }
            
        }
        cout<<endl;
    }


    cout<<"\n\ndata block after removing check bits:\n";
    for(int i =0 ; i< col_siz ; i++){
        int k=1;
        vi parity,indexes;
        for(int j = 0; j < rec_frame[i].size(); j++)
        {
            if(!(k == 0) && !(k & (k - 1))){
               parity.push_back(getParity(rec_frame[i],j+1));
               indexes.push_back(j);
               //cout << "\033[1;"<<FG_GREEN<<"m"<<rec_frame[i][j]<<"\033[0m";
            }
            // else{
            //     cout<<rec_frame[i][j];
            // }
            k++;
        }
        //cout<<endl;
        int bin = 0;
        for(int l=0; l< parity.size(); l++){
            bin += (pow(2,l) * parity[l]);
        }
        //cout<<"Index :"<<bin<<endl;
        if(bin <= rec_frame[i].size() && bin != 0)
            rec_frame[i][bin-1] == 0?rec_frame[i][bin-1] = 1:rec_frame[i][bin-1] = 0;
        
        // cout<<"------------------------\n";
        // for(int j = 0; j < rec_frame[i].size(); j++)
        //     cout<<rec_frame[i][j];
        // cout<<endl;
        for(int l = 0; l < indexes.size(); l++)
        {
            rec_frame[i].erase(rec_frame[i].begin() + (indexes[l]-l));
        }
        
    }
    //cout<<"\n\n------------------\n\n";
    for(int i =0 ; i< col_siz ; i++){
        for(int j = 0; j < rec_frame[i].size(); j++)
        {
              cout<<rec_frame[i][j];
            
        }
        cout<<endl;
    }

    cout<<"\nOutput frame: ";
    string rec_data;
    for(int i =0 ; i< col_siz ; i++){
        for(int j = 0; j < m; j++)
        {
            int bin = 0,idx =0;
            for(int l= (j*8)+7; l >= j*8 ; l--){
                bin += (pow(2,idx) * rec_frame[i][l]);
                idx++;
            }
            rec_data.push_back(bin);
            //cout<<bin<<endl;
            
        }
    }
    cout<<rec_data<<endl;

    return 0;
}