import nltk
from nltk.corpus import stopwords
from nltk.tokenize import TweetTokenizer
from collections import defaultdict
import os

class Analyzer:
    """Implements sentiment analysis."""

    def __init__(self, positives="positive-words.txt", negatives="negative-words.txt"):
        """Initialize Analyzer."""
        PATH = os.path.dirname(__file__)

        self.pos_file = os.path.join(PATH, positives)
        self.neg_file = os.path.join(PATH, negatives)

        self.stop_words = set(stopwords.words('english'))
        self.tknzr = TweetTokenizer(strip_handles=True, reduce_len=True)
        self.lancaster = nltk.SnowballStemmer(language='english')

        self._load_ranking()

    def _load_ranking(self):
        self.pos_set = set()
        self.neg_set = set()

        with open(self.pos_file, mode='r') as p_file:
            while True:
                word = p_file.readline()
                if not word:
                    break
                if len(word) < 2:
                    pass
                else:
                    word = word.rstrip('\n')
                    self.pos_set.add(word)

        with open(self.neg_file, mode='r') as n_file:
            while True:
                word = n_file.readline()
                if not word:
                    break
                if len(word) < 2:
                    pass
                else:
                    word = word.rstrip('\n')
                    self.neg_set.add(word)

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        freq = defaultdict(lambda: 0)
        punctuation = '!"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~'

        for twitt in text:
            tokens = self.tknzr.tokenize(twitt)
            normalized_step = [word.lower() for word in tokens if word.lower() not in self.stop_words]
            # normalized_step = [self.lancaster.stem(word.lower()) for word in tokens if word.lower() not in self.stop_words]

            normalized = [word for word in normalized_step if word not in punctuation and word.isalnum()]
            
            for word in normalized:
                freq[word] += 1

        positive, negative, neutral = 0.0, 0.0, 0.0

        for k, v in freq.items():
            if k in self.pos_set:
                positive += v
            elif k in self.neg_set:
                negative += v
            else:
                neutral += v

        return positive, negative, neutral
