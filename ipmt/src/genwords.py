import sys

def uniqueFreq(newWords, N):
    var = ""
    unique = sorted(set(newWords.split()))
    for i in unique:
        if(len(i) == N and not (i.endswith('.') or i.endswith(','))) :
            var += i + "\n"
    return var

def main():
    txt = open("../data/10kb.txt", "r")
    N = int(sys.argv[1])
    #print txt.read()
    ret =  uniqueFreq(txt.read(), N)
    file = open("../data/pat" + str(N) + ".txt", "w")
    file.write (ret)
    file.close()

if __name__ == "__main__":
    main()