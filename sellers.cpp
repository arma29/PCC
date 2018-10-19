#include "sellers.h"

struct c_unique {
	int current;
	c_unique(){
		current = -1;
	}
	int operator()() {
		return ++current;
	}
} UniqueNumber;

int Sellers::phi(char a, char b){
	return (int)(a != b);
}

std::vector<std::vector<int> > Sellers::mtz(int n, int m){
	std::vector<std::vector<int> > mtz;

	std::vector<int> array_n(n+1);
	std::vector<int> array_m(m+1);

	std::generate (array_n.begin(),
	               array_n.end(), UniqueNumber);

	std::generate (array_m.begin(),
	               array_m.end(), UniqueNumber);

	mtz.push_back(array_n);
	mtz.push_back(array_m);

	return mtz;
}

void print_edit(std::vector<std::vector<int> > edit){
	std::cout << "[";
	for(auto i : edit) {
		std::cout << "[";
		for (size_t j = 0; j < i.size(); j++) {
			/* code */
			std::cout << i[j] << ",";
		}
		std::cout << "],";
	}
	std::cout << "]" << '\n';
}



std::vector<int> Sellers::sellers(std::string txt,
                                  std::string pat, int emax){
	int n = txt.length();
	int m = pat.length();

	std::vector<std::vector<int> > edit = mtz(n,m);
	int last = 1;
	int prev = 0;

	// print_edit(edit);

	std::vector<int> occ;

	for (size_t j = 0; j < n; j++) {
		/* code */
		edit[last][0] = 0;
		for (int i = 1; i < m+1; i++) {
			/* code */
			int arg1 = edit[prev][i-1]+phi(pat[i-1],txt[j]);
			int arg2 = std::min( edit[prev][i]+1, edit[last][i-1]+1);
			edit[last][i] = std::min(arg1,arg2);

			// std::cout << edit[last][i] << " = min ((" <<
			// edit[prev][i-1] << "+" << phi(pat[i-1],txt[j]) <<
			// ") ,"<< edit[prev][i]+1 << "," << edit[last][i-1]+1 <<
			// ")"   << '\n';
		}

		if(edit[last][m] <= emax) {
			occ.push_back(j);
		}

		last = (last+1)%2;
		prev = (prev+1)%2;

		// print_edit(edit);

	}
	return occ;

}

// int main(int argc, char const *argv[]) {
//     Sellers sel;
//     std::string txt = "like lovi loveme like you do lov loveee you iove";
//     std::string pat = "love";
//     int emax = 2;
//     std::vector<int> occ = sel.sellers(txt,pat,emax);
//     std::cout << "[";
//     for(auto i:occ)
//         std::cout << i << ",";
//     std::cout << "]"<< '\n';
//
//     return 0;
// }
