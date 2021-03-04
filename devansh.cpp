#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<string.h>
using namespace std;

// #ifndef ONLINE_JUDGE
// #define freopen 
// #else 
// #define freopen 
// #endif

int cnt=0;
const int MOD =1000000007;
#define endl "\n"

int hashes[100000];
int power[30],stringhash[100000];
int ischar(char c)
{
	if(c>='a'&& c<='z')return 1;
	if(c>='A'&& c<='Z')return 1;
	return 0;
}

int notchar(string str,int ind)
{
    char c=str[ind-1];
    char d=str[ind+1];

    if(ischar(c) && ischar(d))return 0;
	return 1;
}

int modular_exponentiation(int base,int exponent,int MOD)
{
    int ans=1; 
	if(exponent % 2 == 0){ exponent/=2; (base*=base)%MOD; }
	else {(ans*=base)%MOD; exponent--;}
	return ans;
}

int precalculate()
{
  power[0]=1;
  for(int i=1;i<=20;i++)
  	{
  		power[i]=(power[i-1] * 1LL * 29) % MOD;
  	}
}

int calculate_hash(string word)
{
  //Formula : [Summation:(i=0 to n-1) (S[i]*p^i) ]  % MOD
  //cout<<"HI"<<endl;
  int p=29,value=0;

  for(int i=0;i<word.size();i++)
  {
  	int intvalue=word[i]-97+1;
  	if(word[i] == '\'')intvalue=27;
  	(value+=( intvalue * 1LL * power[i] ) % MOD ) % MOD;
  }
  //if(value==16)cout<<word<<" "<<value<<endl;
  return value;
}

int binarysearch(int value)
{
   int l=0,r=cnt-1;
   while(l<r)
   {
     int mid=(l+r)/2;
     if(value<hashes[mid])r=mid;
     else if(value==hashes[mid])return 1;
     else if(value>hashes[mid])l=mid+1;
   }
   return 0;
}

int merge_sorted_arrays(int l,int mid,int r)
{ 
   //make two arrays to store l..mid and mid+1..r
  // cout<<"\n\nMERGE SORTED ARRAYS "<<l<<" "<<mid<<" "<<r<<"\n\n";
  int n=mid-l+1,m=r-(mid+1)+1;
  int left[n],right[m];

  
  for(int i=l;i<=mid;i++)
  {
    left[i-l]=hashes[i];
  }
  for(int i=mid+1;i<=r;i++)
  {
    right[i-(mid+1)]=hashes[i];
  }

  //Risk hai to ishq hai



  int i=0,j=0,k=l;

  while(i<n && j<m)
  {
    if(left[i]<right[j])
    {
      hashes[k++]=left[i];
      i++;
    }
    else 
    {
      hashes[k++]=right[j];
      j++;
    }
  }

  while(i<n)hashes[k++]=left[i++];
  while(j<m)hashes[k++]=right[j++];
}

int merge_sort(int l,int r)
{
  int mid=(l+r)/2;
  if(l<r)
  {
    merge_sort(l,mid);
    merge_sort(mid+1,r);
    merge_sorted_arrays(l,mid,r);
  }
}

main()
{
 freopen("input.txt","r",stdin);
 freopen("output.txt","w",stdout);

 string line;
 vector<string>tokens; //to store tokens
 vector<string>sentences;
 int punctuation[128]={0}; //to store ASCII values of punctuations
 
 char store[32]={'!','"','#','$','%','&','\'','(',')','*','+',',','-','.','/',':',';','<','=','>','?','@','[','\\',']','^','_','`','{','|','}','~',};
 
 //Marking the punctuations
 for(int i=0;i<32;i++)
 {
 	char p=store[i];
 	punctuation[(int)p]=1; 
 }
 
 //Inputting and storing the sentences
 while(getline(cin,line))
 {
 	sentences.push_back(line);
 }


 //Remove punctuation, numbers from sentences and lowering the case
 for(int i=0;i<sentences.size();i++)
 {
   for(int j=0;j<sentences[i].size();j++)
   {
     char word=sentences[i][j];
     int ascii_word=(int)word;
     if(punctuation[ascii_word]==1 && notchar(sentences[i],j) )sentences[i][j]=' ';
     if(ascii_word >= 48 && ascii_word <= 57)sentences[i][j]=' ';
     if(sentences[i][j] >= 65 && sentences[i][j] <=90)
     sentences[i][j]=97+sentences[i][j]-65;
   }
 }
 
 // Removing numbers
 
 // for(int i=0;i<sentences.size();i++)
 // {
 //   for(int j=0;j<sentences[i].size();j++)
 //   {
 //     cout<<sentences[i][j];
 //   }
 //   cout<<endl;
 // }
  
  //Marking stopwords through string hashing
  //Using polynomial rolling hash function (string hashing)
  precalculate();
 // freopen("stopwords.txt","r",stdin);
  ifstream fin;
  ofstream fout;
  fin.open("stopwords.txt");
  fout.open("output1.txt");
  //freopen("output.txt","w",stdout);
  
  string stopword;
  cnt=0;
  while(fin)
  {
    int value=calculate_hash(stopword)%MOD;
    getline(fin,stopword);
    hashes[cnt++]=value;
  }

  vector<string>block;

  merge_sort(0,cnt-1);

 for(auto line: sentences)
 {
 	  stringstream tkn(line);
    string token;

  //Performing tokenization and not saving the tokens which are stopwords
  while(tkn>>token)
  {
  //cout<<token;
 	int value=calculate_hash(token);
 	// check if it's a stopword or not
 	// If it's not a stopword,then save it.
  int have=binarysearch(value);
 	if(have==0)tokens.push_back(token);
  if(have==1)block.push_back(token);
  }
}

 for(auto x:tokens)cout<<x<<endl;

}
