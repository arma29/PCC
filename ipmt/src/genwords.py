import sys

def uniqueFreq(newWords, N):
    var = ""
    # unique = sorted(set(newWords.split()))
    unique = newWords.split()
    for i in unique:
        if(len(i) == N and not (i.endswith('.') or i.endswith(','))) :
            var += i + "\n"
    return var

def main():
    state = sys.argv[1]
    txt = open(state, "r")
    N = int(sys.argv[2])
    #print txt.read()
    ret =  uniqueFreq(txt.read(), N)
    file = open("../data/pat" + str(N) + ".txt", "w")
    file.write (ret)
    file.close()

if __name__ == "__main__":
    main()
