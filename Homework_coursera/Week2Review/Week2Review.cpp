
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

class Matrix{
public:
	Matrix(float density){
		//int num = 0, i, j, dense;

		srand(time(0));
		create_matrix(density);
	}

	const static int Size = 50;

	struct vertex{
		int node;
		// will be average distance on list.
		float distance;
	};

	// Display matrix on screen.
	void display_matrix(){
		for (int i = 0; i < Size; i++) {
			for (int j = 0; j < Size; j++) {
				// rows
				cout << matrix[j][i] << " ";
			}
			// columns
			cout << endl;
		}
		cout << endl;
	}
	void display_vertexs(){
		for (int i = 0; i < Size; i++){
			cout << vertexs[i].node << "\t" << vertexs[i].distance << endl;
		}
		cout << endl;
	}
	void display_average(){
		float av;
		for (int i = 0; i < Size; i++){
			av += vertexs[i].distance;
		}
		av /= Size;
		cout << av << endl;
	}
	// Finds and prints the shortest path.
	void dijkstra(){
		vector<float> distances;
		float distance;
		// loop variables
		int i, j;
		// i is row. j is column. k is vector
		for (i = 0; i < Size; i++){
			for (j = 0; j < Size; j++){
				distance = find_path(i, j);
				if (distance > 0)
					distances.push_back(distance);
				//else cout<<"empty"<<endl;
			}
			vertexs[i].distance = average(distances);
			//cout<<i<<" distance size: "<<distances.size()<<" average: "<<vertexs[i].distance<<endl;
			distances.clear();
		}
	}

private:
	// stores average distances.
	vector<vertex> vertexs;
	float matrix[Size][Size];
	void create_matrix(float density){
		int randDens, Density;
		float dense, output, random;
		Density = static_cast<int>(density*Size);

		int i, j;
		// Create mirrored matrix
		for (i = 0; i < Size; i++) {
			for (j = i + 1; j < Size; j++) {
				// density based on size of matrix.
				randDens = rand() % Size;

				// currently stores some negative values.
				dense = static_cast<int>(Density - randDens);

				// all instance where i = j set to zero
				// All negative values set to zero
				// output is at least 1
				random = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 9));

				(i >= j || dense < 0) ? output = 0 : output = random + 1;
				matrix[i][j] = matrix[j][i] = output;
			}

			// Add the nodes as vectors.
			vertexs.push_back({ i, 0 });
		}
	}
	// the algorithm
	float find_path(const int &I, const int &J){
		vector<vertex> cPath; // closed path
		vector<vertex> oPath; // open path
		cPath.push_back({ I, 0 }); // add initial

		float temp_distance;

		vertex temp;
		temp.node = 0;
		temp.distance = 1000;

		int loop = 0, i = I, j, k;

		// this loop for each possible nodes.
		while (loop < Size){
			// add non-zero distances to open-path
			for (j = 0; j < Size; j++){
				if (matrix[i][j] > 0){
					temp_distance = matrix[i][j] + cPath[i].distance;
					add_to_open_path(j, temp_distance, oPath, cPath);
				}
			}

			// find smallest positive distance
			for (j = 0; j < oPath.size(); j++){
				if (temp.distance > oPath[j].distance){
					temp = oPath[j];
				}
			}

			// check if at destination
			if (temp.node = J){
				return temp.distance;
			}

			// Change i to start at current short value.
			i = temp.node;

			// add minimum to closed path
			cPath.push_back({ temp.node, temp.distance });

			// erase min from open path
			for (j = 0; j < oPath.size(); j++){
				if (oPath[j].node == temp.node) {
					oPath.erase(oPath.begin() + j);
				}
			}
			loop++;

			temp.node = 0;
			temp.distance = 1000;
		}
		return 0.0;
	}
	void add_to_open_path(const int &j, const float &temp_distance, vector<vertex> &oPath, const vector<vertex> &cPath){
		int i;

		// if there is instance at closed path
		for (i = 0; i < cPath.size(); i++)
		{
			if (j == cPath[i].node) return;
		}

		// if there is instance at open path
		// and is longer than what's already there.
		for (i = 0; i < oPath.size(); i++)
		{
			if (j == oPath[i].node) {
				if (temp_distance > oPath[i].distance) return;
			}
		}

		// if there is no instance of j in closed path.
		// add to open path.
		oPath.push_back({ j, temp_distance });
	}
	// Average distance
	float average(const vector<float> &distances){
		float distance = 0;
		for (int i = 0; i < distances.size(); i++){
			distance += distances[i];
		}
		distance /= distances.size();
		return distance;
	}
};

// comment and uncomment functions as necessary.
int main() {
	Matrix M1(0.2);
	Matrix M2(0.4);

	//M1.display_matrix();
	//M2.display_matrix();

	M1.dijkstra();
	M2.dijkstra();

	M1.display_vertexs();
	M2.display_vertexs();

	//M1.display_average();
	//M2.display_average();

	return 0;
}
