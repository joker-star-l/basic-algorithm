with open('hot100.md', 'r') as f:
    lines = f.readlines()
    count = 0
    ls = []
    for line in lines:
        if line.startswith('###'):
            no = int(line.split(' ')[1][:-1])
            ls.append(no)
            count += 1
    print(ls)
    print(count)
