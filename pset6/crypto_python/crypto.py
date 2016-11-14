import passlib.hash as crypto


'''
Function encripts words from dictionary with given
argument as salt. Hashes saved to file in format:
%word%:%salt%:%hash%
'''
def encrypto(salt):
    with open('dictionary.txt', 'r') as f:
        while True:
            line = f.readline()
            if not line:
                break
            crypt1 = crypto.des_crypt.encrypt(line.strip('\n'), salt=salt)
            with open('hashes.txt', 'a') as fw:
                fw.write('{0}:{1}:{2}\n'.format(line.strip('\n'), salt, crypt1))

'''
Function makes equality check given hash with
hash-dictionary. If match found - print password.
'''
def decrypto(word):
    with open('hashes.txt', 'r') as f:
        while True:
            line = f.readline().strip('\n')
            if not line:
                break
            try:
                _word, _salt, _hash = line.split(':')
            except:
                print('Parsing error on word: ',line)
                continue

            if _hash == word:
                print('Hash: {0}\nWord: {1}'.format(word, _word))
                break


if __name__ == '__main__':
    # encrypto('50')
    decrypto('50YHuxoCN9Jkc')
    decrypto('50QvlJWn2qJGE')   #NOPE
    decrypto('50CPlMDLT06yY')
    decrypto('50WUNAFdX/yjA')
    decrypto('50C6B0oz0HWzo')
    decrypto('50fkUxYHbnXGw')   #rofl
    decrypto('50C6B0oz0HWzo')
    decrypto('50nq4RV/NVU0I')   #ABC
    decrypto('50vtwu4ujL.Dk')   #haha
    decrypto('50i2t3sOSAZtk')   #lol
