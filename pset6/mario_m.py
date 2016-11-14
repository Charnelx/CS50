import argparse

def printBlock(n):
    for block in range(n):
        spaces = ' ' * (n - len(('#'  * block)))
        blocks = '#' * block
        print(spaces + blocks, end="")
        print(' ' * 2, end="")
        print(blocks)



if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-n', help='number of blocks', nargs=1)
    args = parser.parse_args()

    blocks = int(args.n[0])

    if 1 > blocks > 23:
        raise ValueError
    else:
        printBlock(blocks)