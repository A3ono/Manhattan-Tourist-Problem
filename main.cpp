// Alexandra Medina

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

class Matrix
{
private:
	int* content;
	int nr, nc;
public:
	Matrix(int nrows, int ncolumns)
	{
		nr = nrows;
		nc = ncolumns;
		content = new int[nrows*ncolumns];
	}
	int& at(int i, int j)
	{
		return content[i*nc + j];
	}
	void PrintContent()
	{
		for (int i = 0; i < (nr*nc); i++)
			cout << content[i] << " ";
		cout << endl;
	}
	void SetZero()
	{
		for (int i = 0; i < (nr*nc); i++)
			content[i] = 0;
	}
	friend void PrintPath(Matrix&, Matrix, Matrix, int, int);
};

int FindBest(Matrix& HorizontalWeights, Matrix& VerticalWeights, Matrix& Nodes, Matrix& Path, int i, int j)
{
	if (Nodes.at(i, j) == 0)
	{
		if ((i == 0) && (j == 0))
			return Nodes.at(i, j);
		else if (i == 0)
		{
			if (Nodes.at(i, j - 1) == 0)
				Nodes.at(i, j - 1) = FindBest(HorizontalWeights, VerticalWeights, Nodes, Path, i, j - 1);
			Nodes.at(i, j) = Nodes.at(i, j - 1) + HorizontalWeights.at(i, j - 1);
			Path.at(i, j) = 0;
			return Nodes.at(i, j);
		}
		else if (j == 0)
		{
			if (Nodes.at(i - 1, j) == 0)
				Nodes.at(i - 1, j) = FindBest(HorizontalWeights, VerticalWeights, Nodes, Path, i - 1, j);
			Nodes.at(i, j) = Nodes.at(i - 1, j) + VerticalWeights.at(i - 1, j);
			Path.at(i, j) = 1;
			return Nodes.at(i, j);
		}
		else
		{
			if (Nodes.at(i, j - 1) == 0)
				Nodes.at(i, j - 1) = FindBest(HorizontalWeights, VerticalWeights, Nodes, Path, i, j - 1);
			if (Nodes.at(i - 1, j) == 0)
				Nodes.at(i - 1, j) = FindBest(HorizontalWeights, VerticalWeights, Nodes, Path, i - 1, j);
			if ((Nodes.at(i, j - 1) + HorizontalWeights.at(i, j - 1)) > (Nodes.at(i - 1, j) + VerticalWeights.at(i - 1, j)))
			{
				Nodes.at(i, j) = Nodes.at(i, j - 1) + HorizontalWeights.at(i, j - 1);
				Path.at(i, j) = 0;
			}
			else
			{
				Nodes.at(i, j) = Nodes.at(i - 1, j) + VerticalWeights.at(i - 1, j);
				Path.at(i, j) = 1;
			}
			return Nodes.at(i, j);
		}
	}
	else
		return Nodes.at(i, j);
}

void PrintPath(Matrix& Solution, Matrix Path, Matrix Nodes, int i, int j)
{
	int k = i, l = j;
	while ((i >= 0) && (j >= 0))
	{
		Solution.at(i, j) = Nodes.at(i, j);
		if (Path.at(i, j) == 0)
			j--;
		else
			i--;
	}
	cout << "The best path is:" << endl;
	for (int a = 0; a <= k; a++)
	{
		for (int b = 0; b <= l; b++)
			cout << Solution.at(a, b) << "\t";
		cout << endl;
	}
}


bool ComOptExists(char** begin, char** end, string option)
{
	return (std::find(begin, end, option) != end);
}


char* GetOpt(char** begin, char** end, string option)
{
	char** itr = std::find(begin, end, option);
	itr++;
	return *itr;
}

int main(int argc, char* argv[])
{
	
	
	if (ComOptExists(argv, argv + argc, "-f"))
	{
		char* FileName = GetOpt(argv, argv + argc, "-f");
		ifstream FileInput;
		FileInput.open(FileName, ifstream::in);
		if (!FileInput.is_open())
		{
			cout << "File '" << FileName << "' couldn't be opened" << endl;
			return 0;
		}
		else
		{
			int intbuf, bufsize, HorizontalSize = 0, counterh = 0, counterv = 0, NRows = 0, NColumns = 0;
			string buffer;
			string::iterator it;
			vector<string>::iterator iter;
			vector<string> Horizontal, Vertical;

			while (FileInput)
			{
				getline(FileInput, buffer);

				if (buffer.find('#') != buffer.npos) //find() returns npos member value on failure
				{
					buffer.clear();
					continue;
				}
				else if (buffer.empty())
					continue;

				for (it = buffer.begin(); it != buffer.end(); it++)
				{
					if (!isdigit(*it))
					{
						buffer.erase(it);
						it--;
					}
				}
				bufsize = (int)buffer.size();

				if (HorizontalSize == 0)
					HorizontalSize = bufsize;
				if (bufsize != HorizontalSize)
					NColumns = bufsize;

				if (bufsize == HorizontalSize)
				{
					NRows++;
					Horizontal.push_back(buffer);
				}
				else if (bufsize == NColumns)
					Vertical.push_back(buffer);
			}

			Matrix HorizontalWeights(NRows, NColumns - 1);
			Matrix VerticalWeights(NRows - 1, NColumns);
			Matrix Nodes(NRows, NColumns);
			Matrix Path(NRows, NColumns);
			Matrix Solution(NRows, NColumns);
			int i = 0, j = 0;

			for (i = 0; i < NRows; i++)
			{
				for (j = 0; j < (NColumns - 1); j++)
				{
					buffer = Horizontal[i].operator[](j);
					stringstream str(buffer);
					str >> intbuf;
					HorizontalWeights.at(i, j) = intbuf;
				}
			}
			for (i = 0; i < (NRows - 1); i++)
			{
				for (j = 0; j < NColumns; j++)
				{
					buffer = Vertical[i].operator[](j);
					stringstream str(buffer);
					str >> intbuf;
					VerticalWeights.at(i, j) = intbuf;
				}
			}
			Solution.SetZero();
			Nodes.SetZero();

			int a = FindBest(HorizontalWeights, VerticalWeights, Nodes, Path, NRows - 1, NColumns - 1);
			PrintPath(Solution, Path, Nodes, NRows - 1, NColumns - 1);
		}
	}
	//  else start with defaults
	else
	{
		cout << "Manhattan Tourist Problem" << endl;



		Matrix HorizontalWeights(3, 2);
		Matrix VerticalWeights(2, 3);
		Matrix Nodes(3, 3);
		Matrix Path(3, 3);
		Matrix Solution(3, 3);
		cout << "Enter 6 Horizontal Weights for grid: " << endl;
		
		cin >> HorizontalWeights.at(0, 0) >> HorizontalWeights.at(0, 1) >> HorizontalWeights.at(1, 0) >> HorizontalWeights.at(1, 1) >> HorizontalWeights.at(2, 0) >> HorizontalWeights.at(2, 1);

		cout << "Enter 6 Vertical Weights for grid: " << endl;

		cin >> VerticalWeights.at(0, 0) >> VerticalWeights.at(0, 1) >> VerticalWeights.at(0, 2) >> VerticalWeights.at(1, 0) >> VerticalWeights.at(1, 1) >> VerticalWeights.at(1, 2);

		Solution.SetZero();
		Nodes.SetZero();

		int a = FindBest(HorizontalWeights, VerticalWeights, Nodes, Path, 2, 2);

		PrintPath(Solution, Path, Nodes, 2, 2);
	}

	system("pause");
	return 0;
}

