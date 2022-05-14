#include<vector>
#include<string>
#include<iostream>
using namespace std ;

vector<vector<int>> key ;

int mod26(int x)
{
	return x >= 0 ? (x%26) : 26-(abs(x)%26) ;
}

vector<vector<int>> getCofactor(vector<vector<int>> mat, int p, int q, int n)
{
    int i = 0, j = 0;
 	vector<vector<int>> temp((mat.size()-1),vector<int>(mat.size()-1));

    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            if (row != p && col != q)
            {
                temp[i][j++] = mat[row][col];
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
	return temp;
}


int determinantOfMatrix(vector<vector<int>> mat, int n)
{
	int D = 0; 

	if (n == 1)
		return mat[0][0];
	vector<vector<int>> Temp((mat.size()-1),vector<int>(mat.size()-1));

	int sign = 1;

	for (int f = 0; f < n; f++)
	{
		
	Temp=getCofactor(mat, 0, f, n);
		D += sign * mat[0][f]* determinantOfMatrix(Temp, n - 1);
		sign = -sign;
	}
	return mod26(D);
}


void generatekey(int n)
{
	int det;
	do
	{
		key.clear();
	for(int i=0; i<n; i++) {
	        vector<int> v1;
	for(int j=0; j<n; j++) {
			v1.push_back(rand()%26+1);
		} 
		key.push_back(v1);
	}
	det=determinantOfMatrix(key,n);

	}while(det==0 || det%2==0 || det%13==0 );
}


int findDetInverse(int R , int D = 26)
{
   int i = 0 ;
   int p[100] = {0,1};
   int q[100] = {0} ;

   while(R!=0)
   {
      q[i] = D/R ;
      int oldD = D ;
      D = R ;
      R = oldD%R ;
      if(i>1)
      {
         p[i] = mod26(p[i-2] - p[i-1]*q[i-2]) ;
      }
      i++ ;
   }
   if (i == 1) return 1;
   else return p[i] = mod26(p[i-2] - p[i-1]*q[i-2]) ;
}

vector<vector<int>> multiplyMatrices(vector<vector<int>> a , int a_rows , int a_cols , vector<vector<int>> b, int b_rows , int b_cols)
{
	vector<vector<int>> res(a_rows,vector<int>(b_cols));
	for(int i=0 ; i < a_rows ; i++)
   {
      for(int j=0 ; j < b_cols ; j++)
      {
         for(int k=0 ; k < b_rows ; k++)
         {
            res[i][j] += a[i][k]*b[k][j] ;
         }
         res[i][j] = mod26(res[i][j]) ;
      }
   }
   return res;
}

vector<vector<int>> adjoint(vector<vector<int>> A,int n)
{
	vector<vector<int>> adj(n,vector<int>(n));

	if (n == 1)
	{
		adj[0][0] = 1;
		return adj;
	}

	int sign = 1;
	vector<vector<int>> temp(n,vector<int>(n));

	for (int i=0; i<n; i++)
	{
		for (int j=0; j<n; j++)
		{
			temp=getCofactor(A, i, j, n);

			sign = ((i+j)%2==0)? 1: -1;

			adj[j][i] = (sign)*(determinantOfMatrix(temp, n-1));
		}
	}
	return adj;
}


vector<vector<int>> inverse(vector<vector<int>> A,int n)
{
	vector<vector<int>> inv(n,vector<int>(n));
	int det=determinantOfMatrix(A,n);
	int detInverse=findDetInverse(det);

	vector<vector<int>> adj(n,vector<int>(n));
	adj=adjoint(A, n);
	
	for(int i=0; i<n ; i++)
   {
      for(int j=0; j<n ; j++)
      {
         inv[i][j] = mod26(adj[i][j] * detInverse) ;
      }
   }
	return inv;
}
string encrypt(string pt, int n)
{
    int ptIter = 0  ;

    int row = (pt.length())/n; 
    vector<vector<int>> P(row,vector<int>(n)); 

    for(int i=0; i<row ; i++)
    {
        for(int j=0; j<n; j++)
        {
            P[i][j] = pt[ptIter++]-'a' ;
        }
    }

    vector<vector<int>> C;    
    C=multiplyMatrices(P, row , n, key,n , n);

    string ct = "" ;
    for(int i=0 ; i<row ; i++)
    {
        for(int j=0 ; j<n ;j++)
        {
            ct += (C[i][j] + 'a');
        }
    }
    return ct ;
}


string decrypt(string ct, int n)
{
    int ctIter = 0 ;

    int row = ct.length()/n;
    vector<vector<int>> C (row,vector<int>(n)); 

    for(int i=0; i<row ; i++)
    {
        for(int j=0; j<n; j++)
        {
            C[i][j] = ct[ctIter++]-'a' ;
        }
    }
    vector<vector<int>> P ;

    vector<vector<int>> inv (n,vector<int>(n));
    inv=inverse(key, n);

    P=multiplyMatrices(C, row , n, inv,n , n) ;
    string pt = "" ;
    for(int i = 0 ; i<row ; i++)
    {
        for(int j=0 ; j<n ; j++)
        {
            pt += (P[i][j] + 'a');
        }
    }
    return pt ;
}

string pt;
string ct;
string dt;

string opsionA(string &pt, string &ct, string &dt){

    cout << "Enter the text to be encrypted : " ;
	    getline(cin,pt);
        int n=2;
        for (int i = 2; i < 10; ++i)
        {
            if(pt.length()%i==0)
            {
                n=i;
                break;
            }
        }
        generatekey(n);
        cout<<"\nThe Key matrix used is :\n";
        for(int i=0; i<n; i++) {
            for(int j=0; j<n; j++) {
                cout<<key[i][j]<<' ';
            }
            cout<<'\n';
        }

        cout << "\nOriginal text  : " << pt << endl;

        ct = encrypt(pt, n) ;
        cout << "Encrypted text : " << ct << endl;
        return ct;
}

string opsionB(string &dt){

    cout << "Enter the text to be decrypted : " ;
	    getline(cin,ct);
        int n=2;
        for (int i = 2; i < 10; ++i)
        {
            if(ct.length()%i==0)
            {
                n=i;
                break;
            }
        }

        cout<<"\nThe Key matrix used is :\n";
        for(int i=0; i<n; i++) {
            for(int j=0; j<n; j++) {
                cout<<key[i][j]<<' ';
            }
            cout<<'\n';
        }

        cout << "\nOriginal text  : " << ct << endl;

        dt = decrypt(ct, n);
        cout << "Decrypted text : " << dt << "\n" << endl;
        return 0;
}

