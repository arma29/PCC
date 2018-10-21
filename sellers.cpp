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

int Sel::phi(char a, char b){
	return (int)(a != b);
}

std::vector<std::vector<int> > Sel::mtz(int n, int m){
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
	std::cout << "GOGO: [";
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



std::vector<int> Sel::sellers(std::string &txt,
                                  std::string &pat, int emax){

    std::vector<int> occ;
	int n = txt.length();
	int m = pat.length();

	//if(m > n) n = m;

	//std::vector<std::vector<int> > edit = mtz(n,m);
	int last = 1;
	int prev = 0;

	// print_edit(edit);



	// for (size_t j = 0; j < n; j++) {
	// 	/* code */
	// 	edit[last][0] = 0;
	// 	for (int i = 1; i < m+1; i++) {
	// 		/* code */
	// 		int arg1 = edit[prev][i-1]+phi(pat[i-1],txt[j]);
	// 		int arg2 = std::min( edit[prev][i]+1, edit[last][i-1]+1);
	// 		edit[last][i] = std::min(arg1,arg2);
    //
	// 		// std::cout << edit[last][i] << " = min ((" <<
	// 		// edit[prev][i-1] << "+" << phi(pat[i-1],txt[j]) <<
	// 		// ") ,"<< edit[prev][i]+1 << "," << edit[last][i-1]+1 <<
	// 		// ")"   << '\n';
	// 	}
    //
	// 	if(edit[last][m] <= emax) {
	// 		occ.push_back(j);
	// 	}
    //
	// 	last = (last+1)%2;
	// 	prev = (prev+1)%2;
    //
	// 	 // print_edit(edit);
    //
	// }

	std::vector<std::vector<int>> edit(2, std::vector<int>(n + 1));
        int old = 0, cur = 1;
        for(int i = 0; i <= n; ++i) {
            edit[old][i] = 0;
        }
        for(int i = 1; i <= m; ++i) {
            edit[cur][0] = i;
            for(int j = 1; j <= n; ++j) {
                // edit[cur][j] = std::min(edit[cur][j - 1] + 1, edit[old][j] + 1);
                // int b = 0;
                // if(pat[i - 1] != txt[j - 1])
                //     b = 1;
                // edit[cur][j] = std::min(edit[cur][j], b + edit[old][j - 1]);

				int arg1 = std::min( edit[cur][j-1]+1, edit[old][j]+1);
				int arg2 = edit[old][j-1]+phi(pat[i-1],txt[j-1]);
				//int arg2 = std::min( edit[old][j]+1, edit[cur][j-1]+1);
				edit[cur][j] = std::min(arg1,arg2);
            }
            std::swap(old, cur);
        }
        for(int j = 1; j <= n; ++j) {
            if(edit[old][j] <= emax) {
                occ.push_back(j);
            }
        }
	return occ;

}

// int main(int argc, char const *argv[]) {
//     std::string txt = "like lovi loveme like you do lov loveee you iove";
//     std::string pat = "love";
//     int emax = 2;
//     std::vector<int> occ = Sel::sellers(txt,pat,emax);
//     std::cout << "[";
//     for(auto i:occ)
//         std::cout << i << ",";
//     std::cout << "]"<< '\n';
//
//     return 0;
// }
