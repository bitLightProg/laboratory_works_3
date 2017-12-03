import random
n = 50
p = 0.7
el = 1
arr = []
arr.append(el)
arr.append(el)
for i in range(2,n+1):
    el = el*i
    arr.append(el)

#print(arr[n])
binom = []
for i in range(0,n+1):
    b = arr[n]/(arr[i]*(arr[n-i]))*(p**i)*((1-p)**(n-i))
    assert(b <= 1)
    binom.append(b)

N = 50
M = 1000*N
fout = open('out' + str(M) +'.txt','w')
for i in range(M):
    count = 0
    for j in range(n):
        p = random.random()
        if p < 0.7:
            count += 1
    fout.write(str(count)+'\n')

#sum = 0
#for i in binom:
#    sum += i
#print(sum)

#fout = open('out' + str(n) +'.txt','w')
#for i in binom:
#    fout.write(str(i))
#    fout.write('\n')
