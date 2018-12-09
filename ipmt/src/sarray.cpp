#include <algorithm>
// #include <cstring>
// #include <string>
// #include <tuple>
// #include <vector>
// #include <iostream>
// #include <cstdio>
// #include <cstring>
// #include <fstream>
// #include <math.h>

#include "sarray.h"

// /**/
// char *read(std:: string filename) {
// 	FILE *file = fopen(filename.c_str(), "rb");
// 	if (file == NULL) {
// 		printf("Failed to open file: %s.\n", filename.c_str());
// 		exit(0);
// 	} else {
// 		fseek(file, 0, SEEK_END);
// 		int size = (int)ftell(file);
// 		rewind(file);
//
// 		char *data = new char[size + 1];
// 		data[size] = '\0';
// 		size_t sizeRead = fread(data, 1, size, file);
// 		if(sizeRead !=	size){
// 			std::cout << "unable to read" << '\n';
// 			exit(0);
// 		}
// 		fclose(file);
// 		return data;
// 	}
// }

/*teste de valor*/
// static int ceilLog2(int n) {
//  int k = 1;
//  int l = 0;
//  while (k < n) {
//      k <<= 1;
//      l += 1;
//  }
//  return l;
// }

int stepLog(int n){
	return (int)ceil(log2(n));
}

const int sigma = 256;  // ASCII characters

std::vector<int> buildHash(char *txt, int n) {
	// char *txt = read(filename);
	// int n = (int)strlen(txt);

	std::vector<int> hash = std::vector<int>(256);
	std::vector<int> count(256, 0);
	for (int i = 0; i < n; ++i) {
		unsigned char ch = txt[i];
		count[ch] = 1;
	}
	for (int i = 1; i < sigma; ++i) {
		count[i] += count[i - 1];
	}
	for (int i = 0; i < n; ++i) {
		std::cout << txt[i];
		unsigned char ch = txt[i];
		hash[ch] = count[ch] - 1;
		std::cout << hash[ch] << '\n';
	}

	return hash;
}

//Primeira linha com o texto
std::vector<std::vector<int> > initializeP(char *txt, int n){
	std::vector<std::vector<int> > P;

	std::vector<int> hash = buildHash(txt, n);

	int log2n = stepLog(n);

	P.assign(log2n + 1, std::vector<int>(n, -1));

	for (int i = 0; i < n; ++i) {
		unsigned char ch = txt[i];
		P[0][i] = hash[ch];
	}

	return P;
}

void sort_index(std::vector<std::vector<int> > &P,
                std::vector<std::tuple<int, int, int> > &V,
                int n,
                int k){

	sort(V.begin(), V.end());

	// Assign ranks
	// sort_index for P[k]
	int rank = 0;
	P[k][std::get<2>(V[0])] = rank;
	for (int i = 1; i < n; ++i) {
		if (std::get<0>(V[i]) != std::get<0>(V[i - 1]) ||
		    std::get<1>(V[i]) != std::get<1>(V[i - 1])) {
			rank += 1;
		}
		P[k][std::get<2>(V[i])] = rank;
	}

}


//Need to give the read(filename) and length
std::vector<std::vector<int> > SAr::build_P(char *txt,int n) {
	// char *txt = read(filename);
	// int n = (int)strlen(txt);

	//P[0][i] = letras
	std::vector<std::vector<int> > P = initializeP(txt,n);

	//TODO: rever o log
	int log2n = stepLog(n);
	std::cout << "log2n vale - "<< log2n << '\n';
	std::cout << "math vale - "<< (int)ceil(log2(n)) << '\n';


	for (int k = 1; k <= log2n; ++k) {
		//Array of Tuples
		std::vector<std::tuple<int, int, int> > V;
		int j = 1 << (k - 1);
		for (int i = 0; i < n; ++i) {
			if (i + j >= n) { // second block is empty string (rank -1)
				V.push_back(std::make_tuple(P[k - 1][i], -1, i));
			} else {
				V.push_back(std::make_tuple(P[k - 1][i], P[k - 1][i + j], i));
			}
		}
		sort_index(P,V,n,k);
	}

	return P;
}

std::vector<int> SAr::buildSArr(std::vector<std::vector<int> > P,int n) {
	std::vector<int> SArr;
	int log2n = stepLog(n);
	SArr.assign(n, -1);
	for (int i = 0; i < n; ++i) {
		SArr[P[log2n][i]] = i;
	}
	return SArr;
}

// computes lcp(txt[i:], txt[j:])
int lcpP(std::vector<std::vector<int> > P, int n,int i, int j) {  // computes lcp(txt[i:], txt[j:])
	int log2n = stepLog(n);
	if (i == j)
		return (n - i);
	//TIRA ELSE?
	else{
		int lcp = 0;
		for (int k = log2n; k >= 0 && i < n && j < n; k--) {
			if (P[k][i] == P[k][j]) {
				//exchange << -> += (pow(k,2))
				lcp += pow(2,k);
				i += pow(2,k);
				j += pow(2,k);
			}
		}
		return lcp;
	}
}

void fill_lcplr(std::vector<int> &Llcp,
                std::vector<int> &Rlcp,
                std::vector<int> SArr,
                std::vector<std::vector<int> > P,
                int n, int l, int r) {
	if (r - l > 1) {
		int h = (l + r) / 2;
		Llcp[h] = lcpP(P, n, SArr[l], SArr[h]);
		Rlcp[h] = lcpP(P, n, SArr[r], SArr[h]);
		fill_lcplr(Llcp, Rlcp, SArr, P, n, l, h);
		fill_lcplr(Llcp, Rlcp, SArr, P, n, h, r);
	}
}

//
void SAr::lcplr(std::vector<int> &Llcp,
           std::vector<int> &Rlcp,
           std::vector<int> SArr,
           std::vector<std::vector<int> > P,
           int n) {

	Llcp.assign(n, 0);
	Rlcp.assign(n, 0);
	fill_lcplr(Llcp, Rlcp, SArr, P, n, 0, n - 1);
}

int lcpPair(const char *u, const char *v) {
	int l = 0;
	while (u[l] != '\0' && v[l] != '\0' && u[l] == v[l]) {
		l += 1;
	}
	return l;
}

int succ(std::vector<int> Llcp,
         std::vector<int> Rlcp,
         std::vector<int> SArr,
         char *txt,int n, const char *pat) {      // succ is in (l, r]

	int m = (int)strlen(pat);

	int L = lcpPair(pat, txt + SArr[0]);
	int R = lcpPair(pat, txt + SArr[n - 1]);
	if (L == m || (unsigned char)txt[SArr[0] + L] >
	    (unsigned char)pat[L]) {              // txt[SArr[0]] >=m pat
		return 0;
	}
	if (R < m && (unsigned char)txt[SArr[n - 1] + R] <
	    (unsigned char)pat[R]) {             // txt[SArr[n - 1]] <m pat
		return n;
	}
	int l = 0, r = n - 1;
	while (r - l > 1) {
		int h = (l + r) / 2;
		int H = -1;
		if (L >= R) {
			if (Llcp[h] >= L) {
				// std::cout << "Comparing P=" << pat << " to" << txt[SArr[h]] << '\n';
				H = L + lcpPair(pat + L, txt + SArr[h] + L);
			} else {
				H = Llcp[h];
			}
		} else { // R > L
			if (Rlcp[h] >= R) {
				H = R + lcpPair(pat + R, txt + SArr[h] + R);
			} else {
				H = Rlcp[h];
			}
		}
		if (H == m || (unsigned char)pat[H] < (unsigned char)txt[SArr[h] + H]) {
			r = h;
			R = H;
		} else {
			l = h;
			L = H;
		}
	}
	return r;
}

int pred(std::vector<int> Llcp,
         std::vector<int> Rlcp,
         std::vector<int> SArr,
         char *txt,int n, const char *pat) {  // pred is in [l, r)

	int m = (int)strlen(pat);

	int L = lcpPair(pat, txt + SArr[0]);
	int R = lcpPair(pat, txt + SArr[n - 1]);

	if (R == m || (unsigned char)txt[SArr[n - 1] + R] <
	    (unsigned char)pat[R]) {              // txt[SArr[n - 1]] <=m pat
		return n - 1;
	}
	if (L < m && (unsigned char)txt[SArr[0] + L] >
	    (unsigned char)pat[L]) {             // txt[SArr[0]] >m pat
		return -1;
	}
	int l = 0, r = n - 1;
	while (r - l > 1) {
		int h = (l + r) / 2;
		int H = -1;
		if (L >= R) {
			if (Llcp[h] >= L) {
				H = L + lcpPair(pat + L, txt + SArr[h] + L);
			} else {
				H = Llcp[h];
			}
		} else { // R > L
			if (Rlcp[h] >= R) {
				H = R + lcpPair(pat + R, txt + SArr[h] + R);
			} else {
				H = Rlcp[h];
			}
		}
		if (H == m || (unsigned char)pat[H] > (unsigned char)txt[SArr[h] + H]) {
			l = h;
			L = H;
		} else {
			r = h;
			R = H;
		}
	}
	return l;
}

int SAr::search(std::vector<int> Llcp,
           std::vector<int> Rlcp,
           std::vector<int> SArr,
           char *txt, int n, const char *pat) {

	int L = succ(Llcp, Rlcp, SArr, txt, n, pat);
	int R = pred(Llcp, Rlcp, SArr, txt, n, pat);
	if (L > R)
		return 0;
	else{
		std::cout << "[";
		for(int i = L; i <= R; i++){
			std::cout << SArr[i] << ",";
		}
		std::cout << "]" << '\n';
		std::cout << "R vale - " << R << "L vale - " << L  << '\n';

		/*Comeca no SArr[R] e vai até pat+SArr[R]*/
		for (size_t i = SArr[R]; i < SArr[R]+(int)strlen(pat); i++) {
			/* code */
			std::cout << txt[i];
		}
		std::cout << '\n';
		return R - L + 1;
	}
}


void printP(std::vector<std::vector<int> > P){
	for (size_t i = 0; i < P.size(); i++) {
		std::cout << "[";
		for (size_t j = 0; j < P[i].size(); j++) {
			/* code */
			std::cout << P[i][j] << ",";
		}
		std::cout << "]"<< '\n';
	}
}

void printVec(std::vector<int> v){
	std::cout << "[";
	for (size_t i = 0; i < v.size(); i++) {
		/* code */
		std::cout << v[i] << ",";

	}
	std::cout << "]"<< '\n';
}

// int main(int argc, char const *argv[]) {
//
// 	// std::string txt = "like lovi loveme like you do lov loveee you iove";
// 	// std::string txt = "banana";
// 	std::string filename = argv[1];
//
// 	char *txt = read(filename);
// 	int n = (int)strlen(txt);
//
// 	std::vector<std::vector<int> > P = build_P(txt,n);
// 	printP(P);
// 	std::cout << '\n';
// 	/*Suffix array*/
// 	std::vector<int> SArr = buildSArr(P,n);
//
// 	/*LCP-LR array for binary trick*/
// 	std::vector<int> Llcp, Rlcp;
// 	lcplr(Llcp, Rlcp, SArr, P, n);
//
// 	printVec(SArr);
// 	printVec(Llcp);
// 	printVec(Rlcp);
//
// 	std::string pat = argv[2];
// 	char *p = new char[pat.size() + 1];
// 	pat.copy(p, std::string::npos, 0);
// 	p[pat.size()] = '\0';
//
// 	std::cout << "Search eh " <<
// 	search(Llcp, Rlcp, SArr, txt,n,p) << '\n';
//
// 	return 0;
// }