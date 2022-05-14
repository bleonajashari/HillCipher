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
